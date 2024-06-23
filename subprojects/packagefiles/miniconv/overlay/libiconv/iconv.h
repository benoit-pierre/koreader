#ifndef ICONV_H
#define ICONV_H

#include "miniconv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef miniconv_t iconv_t;

iconv_t iconv_open(const char *tocode, const char *fromcode);
size_t iconv(iconv_t cd,
             const char **restrict inbuf, size_t *restrict inbytesleft,
             char **restrict outbuf, size_t *restrict outbytesleft);
int iconv_close(iconv_t cd);

#ifdef __cplusplus
}
#endif

#endif
