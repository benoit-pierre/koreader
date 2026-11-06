#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include <zsync.h>

#include "ffi-cdecl.h"

cdecl_type(FILE);

cdecl_func(zsync_begin);
cdecl_func(zsync_begin_receive);
cdecl_func(zsync_complete);
cdecl_func(zsync_end);
cdecl_func(zsync_end_receive);
cdecl_func(zsync_filelen);
cdecl_func(zsync_filename);
cdecl_func(zsync_hint_decompress);
cdecl_func(zsync_needed_byte_ranges);
cdecl_func(zsync_progress);
cdecl_func(zsync_receive_data);
cdecl_func(zsync_status);
cdecl_func(zsync_submit_source_file);
