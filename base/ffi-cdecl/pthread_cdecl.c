#include <pthread.h>

#include "ffi-cdecl.h"

cdecl_const(PTHREAD_CREATE_DETACHED);

#if defined(__ANDROID__)
cdecl_type(pthread_attr_t);
#elif defined(__APPLE__)
cdecl_struct(_opaque_pthread_attr_t);
cdecl_type(__darwin_pthread_attr_t);
cdecl_type(pthread_attr_t);
#else
# if defined(__have_pthread_attr_t)
cdecl_union(pthread_attr_t);
# endif
cdecl_type(pthread_attr_t);
#endif

cdecl_type(pthread_t);

cdecl_func(pthread_attr_destroy);
cdecl_func(pthread_attr_init);
cdecl_func(pthread_attr_setdetachstate);
cdecl_func(pthread_create);
