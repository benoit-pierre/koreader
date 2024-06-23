#include "iconv.h"

iconv_t iconv_open(const char *tocode, const char *fromcode) {
    return miniconv_open(tocode, fromcode);
}

size_t iconv(iconv_t cd,
             const char **restrict inbuf, size_t *restrict inbytesleft,
             char **restrict outbuf, size_t *restrict outbytesleft) {
    return miniconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
}

int iconv_close(iconv_t cd) {
    return miniconv_close(cd);
};
