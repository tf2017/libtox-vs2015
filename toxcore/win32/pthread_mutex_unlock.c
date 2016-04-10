/*
 * pthread_mutex_unlock.c
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

#include <intrin.h>
#include "pthread.h"
#include "pthread_win32.h"

int
pthread_mutex_unlock(pthread_mutex_t * mutex)
{
	int result = 0;
	int kind;
	pthread_mutex_t mx;

	mx = *mutex;

	/*
	 * If the thread calling us holds the mutex then there is no
	 * race condition. If another thread holds the
	 * lock then we shouldn't be in here.
	 */
	kind = mx->kind;

	if (kind == PTHREAD_MUTEX_NORMAL)
	{
		LONG idx;

		idx = _InterlockedExchange(&mx->lock_idx, 0);
		if (idx != 0)
		{
			if (idx < 0)
			{
				/*
				 * Someone may be waiting on that mutex.
				 */
				if (SetEvent(mx->event) == 0)
				{
					result = EINVAL;
				}
			}
		}
	}
	else
	{
		if (mx->ownerThread == pthread_self())
		{
			if (kind != PTHREAD_MUTEX_RECURSIVE || 0 == --mx->recursive_count)
			{
				mx->ownerThread = 0;

				if (_InterlockedExchange(&mx->lock_idx, 0) < 0L)
				{
					/* Someone may be waiting on that mutex */
					if (SetEvent(mx->event) == 0)
					{
						result = EINVAL;
					}
				}
			}
		}
		else
		{
			result = EPERM;
		}
	}

	return (result);
}
