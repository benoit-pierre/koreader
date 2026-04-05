-- Automatically generated with ffi-cdecl.

local ffi = require("ffi")

ffi.cdef[[
static const int PTHREAD_CREATE_DETACHED = 1;
typedef struct {
  uint32_t flags;
  void *stack_base;
  size_t stack_size;
  size_t guard_size;
  int32_t sched_policy;
  int32_t sched_priority;
  char __reserved[16];
} pthread_attr_t;
typedef long int pthread_t;
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
]]
