-- Automatically generated with ffi-cdecl.

local ffi = require("ffi")

ffi.cdef[[
typedef struct _IO_FILE FILE;
struct zsync_state *zsync_begin(FILE *, int, const char *);
struct zsync_receiver *zsync_begin_receive(struct zsync_state *, int);
int zsync_complete(struct zsync_state *);
char *zsync_end(struct zsync_state *);
void zsync_end_receive(struct zsync_receiver *);
long int zsync_filelen(struct zsync_state *);
char *zsync_filename(const struct zsync_state *);
int zsync_hint_decompress(const struct zsync_state *);
long int *zsync_needed_byte_ranges(struct zsync_state *, int *, int);
void zsync_progress(const struct zsync_state *, long long int *, long long int *);
int zsync_receive_data(struct zsync_receiver *, const unsigned char *, long int, size_t);
int zsync_status(const struct zsync_state *);
long long int zsync_submit_source_file(struct zsync_state *, FILE *, int (*)(long long int, long long int));
]]
