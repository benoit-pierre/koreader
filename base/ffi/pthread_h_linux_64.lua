-- Automatically generated with ffi-cdecl.

local ffi = require("ffi")

ffi.cdef[[
static const int PTHREAD_CREATE_DETACHED = 1;
union pthread_attr_t {
  char __size[56];
  long int __align;
};
typedef union pthread_attr_t pthread_attr_t;
typedef long unsigned int pthread_t;
int pthread_attr_destroy(pthread_attr_t *) __attribute__((nothrow, leaf));
int pthread_attr_init(pthread_attr_t *) __attribute__((nothrow, leaf));
int pthread_attr_setdetachstate(pthread_attr_t *, int) __attribute__((nothrow, leaf));
int pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void *), void *restrict) __attribute__((nothrow));
]]
