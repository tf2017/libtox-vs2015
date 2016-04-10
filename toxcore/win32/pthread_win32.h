/*
 * pthread_win32.h
 * (former implement.h)
 *
 * Definitions that don't need to be public.
 *
 * Keeps all the internals out of pthread.h
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 *
 *      Contact Email: Ross.Johnson@homemail.com.au
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

#if !defined(_PTHREAD_WIN32_H)
#define _PTHREAD_WIN32_H

#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0400
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>

struct pthread_mutexattr_t_
{
	int kind;
};

struct pthread_mutex_t_
{
	LONG lock_idx; /* Provides exclusive access to mutex state via the Interlocked* mechanism.
					  0: unlocked/free.
					  1: locked - no other waiters.
					 -1: locked - with possible other waiters.
				   */
	int recursive_count; /* Number of unlocks a thread needs to perform
					        before the lock is released (recursive
					        mutexes only). */
	int kind;			 /* Mutex type. */
	DWORD ownerThread;
	HANDLE event;		 /* Mutex release notification to waiting threads. */
};

#endif /* _PTHREAD_WIN32_H */
