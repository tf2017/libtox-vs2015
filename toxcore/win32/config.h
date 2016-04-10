#pragma once

#define WIN32_LEAN_AND_MEAN
#define _INC_ERRNO
#undef  VANILLA_NACL

typedef int ssize_t;

#if _MSC_VER <= 1800
#define snprintf _snprintf
#endif

#define C99_DYNARRAY( eltype, arrname, arrsize ) size_t sizeof_##arrname = sizeof(eltype) * (arrsize); \
		eltype * arrname = _alloca( sizeof_##arrname )
#define C99_sizeof( dynarrname ) sizeof_##dynarrname
