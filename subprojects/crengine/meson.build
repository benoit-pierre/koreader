project(
  'crengine',
  ['c', 'cpp'],
  version: '3.0.57-ko',
  default_options: 'cpp_std=c++17',
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
fs = import('fs')

enable_antiword = get_option('antiword').allowed()
enable_chmlib   = get_option('chmlib').allowed()

freetype_dep = dependency('freetype2', required: get_option('freetype'))
fribidi_dep  = dependency('fribidi', required: get_option('fribidi'))
harfbuzz_dep = dependency('harfbuzz', required: get_option('harfbuzz'))
jpeg_dep     = dependency('libjpeg', required: get_option('jpeg'))
png_dep      = dependency('libpng', required: get_option('png'))
srell_dep    = dependency('srell', required: get_option('srell_regex'))
utf8proc_dep = dependency('libutf8proc', required: get_option('utf8proc'))
zlib_dep     = dependency('zlib', required: get_option('zlib'))
zstd_dep     = dependency('libzstd', required: get_option('zstd'))

# WebP support: need both decode and demux libraries.
webp_dep = declare_dependency()
webp_opt = get_option('webp')
foreach _depname : [
  ['libwebpdecoder', 'libwebp'],
  'libwebpdemux',
]
  webp_dep = declare_dependency(
    dependencies: [
      webp_dep,
      dependency(_depname, disabler: true, required: webp_opt),
    ],
  )
  webp_opt = webp_opt.require(webp_dep.found())
endforeach
if webp_opt.disabled()
  webp_dep = dependency('', required: false)
endif

# Ensure libunibreak has the required `lb_get_char_class` support.
unibreak_opt = get_option('libunibreak')
unibreak_dep = dependency('libunibreak', disabler: true, required: unibreak_opt)
unibreak_opt = unibreak_opt.require(
  cc.has_header_symbol(
    'linebreakdef.h',
    'lb_get_char_class',
    dependencies: unibreak_dep.partial_dependency(
      compile_args: true,
      includes: true,
    ),
    required: unibreak_opt,
  ),
)
if unibreak_opt.disabled()
  unibreak_dep = dependency('', required: false)
endif

err_msg = 'the lunasvg and nanosvg features are mutually exclusive'
lunasvg_dep = dependency(
  'lunasvg',
  required: get_option(
    'lunasvg',
  ).require(
    not get_option('nanosvg').enabled(),
    error_message: err_msg,
  ),
)
nanosvg_dep = dependency(
  'nanosvg',
  required: get_option(
    'nanosvg',
  ).require(
    not lunasvg_dep.found(),
    error_message: err_msg,
  ),
)
if lunasvg_dep.found()
  svg_support = 'lunasvg'
elif nanosvg_dep.found()
  svg_support = 'nanosvg'
else
  svg_support = ''
endif

cdata = configuration_data()

cdata.set('_DEBUG', get_option('debug'))

cdata.set10('CHM_SUPPORT_ENABLED' , enable_chmlib)
cdata.set10('ENABLE_ANTIWORD'     , enable_antiword)
cdata.set10('LDOM_USE_OWN_MEM_MAN', not get_option('sysmalloc'))
cdata.set10('MATHML_SUPPORT'      , get_option('mathml').allowed())
cdata.set10('USE_GIF'             , get_option('gif').allowed())

foreach _flag, _dep : {
  'USE_FREETYPE'   : freetype_dep,
  'USE_FRIBIDI'    : fribidi_dep,
  'USE_HARFBUZZ'   : harfbuzz_dep,
  'USE_LIBJPEG'    : jpeg_dep,
  'USE_LIBPNG'     : png_dep,
  'USE_LIBUNIBREAK': unibreak_dep,
  'USE_LIBWEBP'    : webp_dep,
  'USE_LUNASVG'    : lunasvg_dep,
  'USE_NANOSVG'    : nanosvg_dep,
  'USE_SRELL_REGEX': srell_dep,
  'USE_UTF8PROC'   : utf8proc_dep,
  'USE_ZLIB'       : zlib_dep,
  'USE_ZSTD'       : zstd_dep,
}
  cdata.set10(_flag, _dep.found())
endforeach

if (
  8 == meson.get_compiler('cpp').sizeof(
    'off64_t',
    args: '-D_LARGEFILE64_SOURCE=1',
    prefix: '#include<<sys/types.h>',
  )
)
  cdata.set('_LARGEFILE64_SOURCE', 1)
  cdata.set('DHAVE_STAT64', 1)
endif

add_project_arguments(
  cpp.get_supported_arguments(
    '-Wno-reorder',
    '-Wno-template-id-cdtor',
    '-ftabstop=4',
  ),
  language: 'cpp',
)

subdir('crengine/include')

if enable_antiword
  add_project_arguments('-DCR3_ANTIWORD_PATCH', language: ['c', 'cpp'])
  if get_option('debug')
    # For `antiword.h` crap…
    add_project_arguments('-DDEBUG', language: ['c', 'cpp'])
  endif
  subdir('thirdparty/antiword')
else
  antiword_dep = declare_dependency()
endif

if enable_chmlib
  subdir('thirdparty/chmlib')
else
  chmlib_dep = declare_dependency()
endif

subdir('crengine/qimagescale')
subdir('crengine/src')

_toinstall = []

foreach _data : [
  # Directories.
  'hyph/',
  # Files.
  'NOTES.txt',
  'chm.css',
  'cr3.css',
  'dict.css',
  'doc.css',
  'epub.css',
  'fb2.css',
  'htm.css',
  'html5.css',
  'rtf.css',
  'txt.css',
]
  _toinstall += 'cr3gui/data' / _data
endforeach

foreach _data : _toinstall
  if get_option('install_symlinks')
    install_symlink(
      fs.name(_data),
      install_dir: 'data',
      install_tag: 'runtime',
      pointing_to: meson.project_source_root() / _data,
    )
  else
    if fs.is_file(_data)
      install_data(
        _data,
        install_dir: 'data',
        install_tag: 'runtime',
      )
    else
      install_subdir(
        _data,
        install_dir: 'data',
        install_tag: 'runtime',
      )
    endif
  endif
endforeach

summary(
  {
    'Antiword'   : get_option('antiword').allowed(),
    'CHMlib'     : get_option('chmlib').allowed(),
    'FreeType'   : freetype_dep.found(),
    'FriBidi'    : fribidi_dep.found(),
    'GIF'        : get_option('gif').allowed(),
    'HarfBuzz'   : harfbuzz_dep.found(),
    'JPEG'       : jpeg_dep.found(),
    'libunibreak': unibreak_dep.found(),
    'MathML'     : get_option('mathml').allowed(),
    'PNG'        : png_dep.found(),
    'SRELL regex': srell_dep.found(),
    'SVG'        : svg_support == '' ? false: svg_support,
    'utf8proc'   : utf8proc_dep.found(),
    'WebP'       : webp_dep.found(),
    'zlib'       : zlib_dep.found(),
    'ZSTD'       : zstd_dep.found(),
  },
  bool_yn: true,
  section: 'Features',
)
