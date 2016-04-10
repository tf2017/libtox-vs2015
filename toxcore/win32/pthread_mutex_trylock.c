/*
 * pthread_mutex_trylock.c
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
pthread_mutex_trylock(pthread_mutex_t * mutex)
{
	pthread_mutex_t mx;
	int kind;
	int result = 0;

	mx = *mutex;
	kind = mx->kind;

	if (0 == _InterlockedCompareExchange(&mx->lock_idx, 1, 0))
	{
		if (kind != PTHREAD_MUTEX_NORMAL)
		{
			mx->recursive_count = 1;
			mx->ownerThread = pthread_self();
		}
	}
	else
	{
		if (kind == PTHREAD_MUTEX_RECURSIVE && mx->ownerThread == pthread_self())
		{
			mx->recursive_count++;
		}
		else
		{
			result = EBUSY;
		}
	}

	return (result);
}
