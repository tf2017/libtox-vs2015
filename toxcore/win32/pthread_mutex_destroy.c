/*
 * pthread_mutex_destroy.c
 *
 * Description:
 * This translation unit implements mutual exclusion (mutex) primitives.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 *
 *      Contact Email: rpj@callisto.canberra.edu.au
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "pthread.h"
#include "pthread_win32.h"

int
pthread_mutex_destroy(pthread_mutex_t * mutex)
{
	int result = 0;
	pthread_mutex_t mx;

	 /*
	  * Check to see if we have something to delete.
	  */
	mx = *mutex;

	result = pthread_mutex_trylock(&mx);

	/*
	 * If trylock succeeded and the mutex is not recursively locked it
	 * can be destroyed.
	 */
	if (0 == result || ENOTRECOVERABLE == result)
	{
		if (mx->kind != PTHREAD_MUTEX_RECURSIVE || 1 == mx->recursive_count)
		{
			/*
			 * FIXME!!!
			 * The mutex isn't held by another thread but we could still
			 * be too late invalidating the mutex below since another thread
			 * may already have entered mutex_lock and the check for a valid
			 * *mutex != NULL.
			 */
			*mutex = NULL;

			result = (0 == result) ? pthread_mutex_unlock(&mx) : 0;

			if (0 == result)
			{
				if (!CloseHandle(mx->event))
				{
					*mutex = mx;
					result = EINVAL;
				}
				else
				{
					free(mx);
				}
			}
			else
			{
				/*
				 * Restore the mutex before we return the error.
				 */
				*mutex = mx;
			}
		}
		else /* mx->recursive_count > 1 */
		{
			/*
			 * The mutex must be recursive and already locked by us (this thread).
			 */
			mx->recursive_count--;	/* Undo effect of pthread_mutex_trylock() above */
			result = EBUSY;
		}
	}

	return (result);
}
