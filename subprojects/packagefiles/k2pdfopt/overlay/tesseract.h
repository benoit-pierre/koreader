#pragma once

#include <allheaders.h>

#ifdef __cplusplus
extern "C" {
#endif

void *tess_capi_init(char *datapath,char *language,int ocr_type,FILE *out,
                     char *initstr,int maxlen,int *status);
int tess_capi_get_ocr(void *api,PIX *pix,char *outstr,int maxlen,FILE *out);
void tess_capi_end(void *api);

const char* tess_capi_get_init_language(void *api);
int tess_capi_get_word_boxes(void *api, PIX *pix, BOXA **out_boxa, int is_cjk, FILE *out);

#ifdef __cplusplus
}
#endif
