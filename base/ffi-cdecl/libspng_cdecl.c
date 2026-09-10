#include <spng.h>

cdecl_const(SPNG_CTX_ENCODER);
cdecl_const(SPNG_DECODE_TRNS);
cdecl_const(SPNG_ENCODE_FINALIZE);

cdecl_enum(spng_color_type);
cdecl_enum(spng_format);
cdecl_enum(spng_option);

cdecl_type(spng_ctx);

cdecl_func(spng_ctx_new);
cdecl_func(spng_ctx_free);

cdecl_struct(spng_ihdr);
cdecl_func(spng_get_ihdr);
cdecl_func(spng_set_ihdr);

cdecl_func(spng_set_png_buffer);
cdecl_func(spng_set_png_file);

cdecl_func(spng_set_option);

cdecl_func(spng_decoded_image_size);
cdecl_func(spng_decode_image);

cdecl_func(spng_encode_image);

cdecl_func(spng_strerror);
