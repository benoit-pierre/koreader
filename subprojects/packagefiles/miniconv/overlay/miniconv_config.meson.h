#ifndef MINICONV_CONFIG_H
#define MINICONV_CONFIG_H

#include <stddef.h>

#mesondefine __BIG_ENDIAN__
#mesondefine HAVE_STDINT_H

#ifdef __cplusplus
	#define MINICONV_INLINE static inline
#elif defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
	#define MINICONV_INLINE static __inline
#else
	#define MINICONV_INLINE static
#endif

#ifndef MINICONV_PUBLIC
	#define MINICONV_PUBLIC
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#if defined(UINT16_MAX) && defined(UINT32_MAX)

	typedef uint16_t miniconv_uint16_t;
	typedef uint32_t miniconv_uint32_t;

#else

	typedef unsigned short miniconv_uint16_t;
	typedef char miniconv_verify_uint16_t_size[sizeof(miniconv_uint16_t) == 2 ? 1 : -1];

	typedef unsigned int miniconv_uint32_t;
	typedef char miniconv_verify_uint32_t_size[sizeof(miniconv_uint32_t) == 4 ? 1 : -1];

#endif

#endif
