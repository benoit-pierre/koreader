-- Automatically generated with ffi-cdecl.

local ffi = require("ffi")

ffi.cdef[[
static const int PTHREAD_CREATE_DETACHED = 1;
typedef union {
  char __size[36];
  long int __align;
} pthread_attr_t;
typedef long unsigned int pthread_t;
int pthread_attr_destroy(pthread_attr_t *) __attribute__((nothrow));
int pthread_attr_init(pthread_attr_t *) __attribute__((nothrow));
int pthread_attr_setdetachstate(pthread_attr_t *, int) __attribute__((nothrow));
int pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void *), void *restrict) __attribute__((nothrow));
]]
