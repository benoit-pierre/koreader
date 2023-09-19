project(
  'libk2pdfopt',
  ['c', 'cpp'],
  version: '2.42',
  meson_version: '>= 0.64.0',
)

fs = import('fs')

cc = meson.get_compiler('c')

add_project_arguments(
  # Fix crash when compiling with LTO enabled.
  cc.get_supported_arguments('-fno-strict-aliasing'),
  '-DNOMEMDEBUG',
  language: 'c',
)

m_dep = cc.find_library('m', required: false)
threads_dep = dependency('threads')

dependencies = [m_dep, threads_dep]

if host_machine.system() == 'windows'
  dependencies += cc.find_library('shlwapi')
endif

cdata = configuration_data()

foreach _depspec : [
  ['djvu'     , 'djvulibre', get_option('djvulibre')],
  ['jpeg'     , 'libjpeg'  , get_option('libjpeg')  ],
  ['leptonica', 'lept'     , true                   ],
  ['mupdf'    , 'libmupdf' , get_option('mupdf')    ],
  ['png'      , 'libpng'   , get_option('libpng')   ],
  ['tesseract', 'tesseract', true                   ],
  ['z'        , 'zlib'     , get_option('zlib')     ],
]
  _dep = dependency(_depspec[1], required: _depspec[2])
  cdata.set('HAVE_@0@_LIB'.format(_depspec[0].to_upper()), _dep.found())
  set_variable('@0@_dep'.format(_depspec[0]), _dep)
  dependencies += _dep
endforeach

install_kwargs = {
  'install_dir': get_option('includedir') / 'k2pdfopt',
  'install_tag': 'devel',
}

tesscapi_cpp = fs.copyfile(
  'tesseract_mod/tesscapi.cpp',
  'tesscapi.cpp',
)

tesseract_h = fs.copyfile(
  'include_mod/tesseract.h',
  'tesseract.h',
  install: true,
  kwargs: install_kwargs,
)

willus_config_h = configure_file(
  configuration: cdata,
  input: 'willus_config.meson.h',
  output: 'willus_config.h',
  kwargs: install_kwargs,
)

k2pdfopt_lib = library(
  'k2pdfopt',
  [
    tesscapi_cpp,
    tesseract_h,
    willus_config_h,
    'k2pdfoptlib/bmpregion.c',
    'k2pdfoptlib/devprofile.c',
    'k2pdfoptlib/k2bmp.c',
    'k2pdfoptlib/k2file.c',
    'k2pdfoptlib/k2files.c',
    'k2pdfoptlib/k2gui.c',
    'k2pdfoptlib/k2gui_cbox.c',
    'k2pdfoptlib/k2gui_osdep.c',
    'k2pdfoptlib/k2gui_overlay.c',
    'k2pdfoptlib/k2mark.c',
    'k2pdfoptlib/k2master.c',
    'k2pdfoptlib/k2mem.c',
    'k2pdfoptlib/k2menu.c',
    'k2pdfoptlib/k2ocr.c',
    'k2pdfoptlib/k2parsecmd.c',
    'k2pdfoptlib/k2proc.c',
    'k2pdfoptlib/k2publish.c',
    'k2pdfoptlib/k2settings.c',
    'k2pdfoptlib/k2settings2cmd.c',
    'k2pdfoptlib/k2sys.c',
    'k2pdfoptlib/k2usage.c',
    'k2pdfoptlib/k2version.c',
    'k2pdfoptlib/pagelist.c',
    'k2pdfoptlib/pageregions.c',
    'k2pdfoptlib/textrows.c',
    'k2pdfoptlib/textwords.c',
    'k2pdfoptlib/userinput.c',
    'k2pdfoptlib/wrapbmp.c',
    'willuslib/ansi.c',
    'willuslib/array.c',
    'willuslib/bmp.c',
    'willuslib/bmpdjvu.c',
    'willuslib/bmpmupdf.c',
    'willuslib/dtcompress.c',
    'willuslib/filelist.c',
    'willuslib/fontdata.c',
    'willuslib/fontrender.c',
    'willuslib/gslpolyfit.c',
    'willuslib/linux.c',
    'willuslib/math.c',
    'willuslib/mem.c',
    'willuslib/ocr.c',
    'willuslib/ocrgocr.c',
    'willuslib/ocrtess.c',
    'willuslib/pdfwrite.c',
    'willuslib/point2d.c',
    'willuslib/render.c',
    'willuslib/strbuf.c',
    'willuslib/string.c',
    'willuslib/token.c',
    'willuslib/wfile.c',
    'willuslib/wgs.c',
    'willuslib/wgui.c',
    'willuslib/willusversion.c',
    'willuslib/win.c',
    'willuslib/winbmp.c',
    'willuslib/wincomdlg.c',
    'willuslib/winmbox.c',
    'willuslib/winshell.c',
    'willuslib/winshellwapi.c',
    'willuslib/wleptonica.c',
    'willuslib/wmupdf.c',
    'willuslib/wmupdfinfo.c',
    'willuslib/wpdf.c',
    'willuslib/wsys.c',
    'willuslib/wzfile.c',
    'koptcrop.c',
    'koptimize.c',
    'koptocr.c',
    'koptreflow.c',
    'setting.c',
  ],
  c_args: '-DK2PDFOPT_EXPORT=__attribute__ ((visibility("default")))',
  dependencies: dependencies,
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: include_directories(
    'k2pdfoptlib',
    'willuslib',
  ),
  install: true,
  soversion: 2,
)

k2pdfopt_dep = declare_dependency(
  dependencies: [leptonica_dep.partial_dependency(includes: true)],
  include_directories: include_directories(
    '.',
    'k2pdfoptlib',
    'willuslib',
  ),
  link_with: k2pdfopt_lib,
)

meson.override_dependency('k2pdfopt', k2pdfopt_dep)

install_headers(
  'context.h',
  'k2pdfoptlib/k2pdfopt.h',
  'koptcrop.h',
  'koptimize.h',
  'koptocr.c',
  'koptocr.h',
  'koptreflow.h',
  'setting.h',
  'willuslib/willus.h',
  subdir: 'k2pdfopt',
)

pkg = import('pkgconfig')
pkg.generate(k2pdfopt_lib, subdirs: 'k2pdfopt')
