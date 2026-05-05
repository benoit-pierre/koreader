-- Automatically generated with ffi-cdecl.

local ffi = require("ffi")

ffi.cdef[[
static const int PTHREAD_CREATE_DETACHED = 2;
struct _opaque_pthread_attr_t {
  long int __sig;
  char __opaque[56];
};
typedef struct _opaque_pthread_attr_t __darwin_pthread_attr_t;
typedef struct _opaque_pthread_attr_t pthread_attr_t;
typedef struct _opaque_pthread_t *pthread_t;
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void *), void *restrict);
]]
