-- Automatically generated with ffi-cdecl.

require "ffi/posix_h"

require("ffi").cdef[[
static const unsigned SPNG_CTX_ENCODER = 2;
static const unsigned SPNG_DECODE_TRNS = 1;
static const unsigned SPNG_ENCODE_FINALIZE = 2;
enum spng_color_type {
  SPNG_COLOR_TYPE_GRAYSCALE = 0,
  SPNG_COLOR_TYPE_TRUECOLOR = 2,
  SPNG_COLOR_TYPE_INDEXED = 3,
  SPNG_COLOR_TYPE_GRAYSCALE_ALPHA = 4,
  SPNG_COLOR_TYPE_TRUECOLOR_ALPHA = 6,
};
enum spng_format {
  SPNG_FMT_RGBA8 = 1,
  SPNG_FMT_RGBA16 = 2,
  SPNG_FMT_RGB8 = 4,
  SPNG_FMT_GA8 = 16,
  SPNG_FMT_GA16 = 32,
  SPNG_FMT_G8 = 64,
  SPNG_FMT_PNG = 256,
  SPNG_FMT_RAW = 512,
};
enum spng_option {
  SPNG_KEEP_UNKNOWN_CHUNKS = 1,
  SPNG_IMG_COMPRESSION_LEVEL,
  SPNG_IMG_WINDOW_BITS,
  SPNG_IMG_MEM_LEVEL,
  SPNG_IMG_COMPRESSION_STRATEGY,
  SPNG_TEXT_COMPRESSION_LEVEL,
  SPNG_TEXT_WINDOW_BITS,
  SPNG_TEXT_MEM_LEVEL,
  SPNG_TEXT_COMPRESSION_STRATEGY,
  SPNG_FILTER_CHOICE,
  SPNG_CHUNK_COUNT_LIMIT,
  SPNG_ENCODE_TO_BUFFER,
};
typedef struct spng_ctx spng_ctx;
spng_ctx *spng_ctx_new(int);
void spng_ctx_free(spng_ctx *);
struct spng_ihdr {
  uint32_t width;
  uint32_t height;
  uint8_t bit_depth;
  uint8_t color_type;
  uint8_t compression_method;
  uint8_t filter_method;
  uint8_t interlace_method;
};
int spng_get_ihdr(spng_ctx *, struct spng_ihdr *);
int spng_set_ihdr(spng_ctx *, struct spng_ihdr *);
int spng_set_png_buffer(spng_ctx *, const void *, size_t);
int spng_set_png_file(spng_ctx *, FILE *);
int spng_set_option(spng_ctx *, enum spng_option, int);
int spng_decoded_image_size(spng_ctx *, int, size_t *);
int spng_decode_image(spng_ctx *, void *, size_t, int, int);
int spng_encode_image(spng_ctx *, const void *, size_t, int, int);
const char *spng_strerror(int);
]]
