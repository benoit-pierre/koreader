project(
  'mupdf',
  ['c'],
  version: '1.13.0',
  default_options: 'c_std=gnu99',
)

cc = meson.get_compiler('c')

aes_dep = dependency('libaes', required: false)
freetype_dep = dependency('freetype2')
harfbuzz_dep = dependency('harfbuzz')
jbig2dec_dep = dependency('jbig2dec')
jpeg_dep = dependency('libjpeg')
if host_machine.system() == 'android'
  log_dep = cc.find_library('log')
else
  log_dep = declare_dependency()
endif
m_dep = cc.find_library('m', required: false)
mujs_dep = dependency('mujs')
openjp2_dep = dependency('libopenjp2')
webp_dep = declare_dependency(
  dependencies: [
    dependency(['libwebpdecoder', 'libwebp'], disabler: true, required: false),
    dependency('libwebpdemux', disabler: true, required: false),
  ],
)
webp_dep = webp_dep.found() ? webp_dep : dependency('', required: false)
zlib_dep = dependency('zlib')

cdata = configuration_data()

# No builtin fonts.
cdata.set('NOBUILTINFONT', true)
# No ICC support (and lcms2 dependency).
cdata.set('NO_ICC', true)
# No EPUB support.
cdata.set10('FZ_ENABLE_EPUB', false)
# No SVG support.
cdata.set10('FZ_ENABLE_SVG', false)
# No XPS support.
cdata.set10('FZ_ENABLE_XPS', false)
# Encrypted ZIPs support.
cdata.set('HAVE_LIBAES', aes_dep.found())
# WebP support.
cdata.set('HAVE_WEBP', webp_dep.found())
# Do not use vendored JPEG library.
cdata.set10('SHARE_JPEG', true)

mupdf_src = []

foreach _prefix, _files : {
  'source/fitz': [
    'archive.c',
    'bbox-device.c',
    'bidi.c',
    'bidi-std.c',
    'bitmap.c',
    'buffer.c',
    'color-icc-create.c',
    'color-lcms.c',
    'colorspace.c',
    'compress.c',
    'compressed-buffer.c',
    'context.c',
    'crypt-aes.c',
    'crypt-arc4.c',
    'crypt-md5.c',
    'crypt-sha2.c',
    'device.c',
    'directory.c',
    'document-all.c',
    'document.c',
    'draw-affine.c',
    'draw-blend.c',
    'draw-device.c',
    'draw-edgebuffer.c',
    'draw-edge.c',
    'draw-glyph.c',
    'draw-mesh.c',
    'draw-paint.c',
    'draw-path.c',
    'draw-rasterize.c',
    'draw-scale-simple.c',
    'draw-unpack.c',
    'encode-basic.c',
    'error.c',
    'filter-basic.c',
    'filter-dct.c',
    'filter-fax.c',
    'filter-flate.c',
    'filter-jbig2.c',
    'filter-leech.c',
    'filter-lzw.c',
    'filter-predict.c',
    'filter-sgi.c',
    'filter-thunder.c',
    'font.c',
    'ftoa.c',
    'geometry.c',
    'getopt.c',
    'glyph.c',
    'halftone.c',
    'harfbuzz.c',
    'hash.c',
    'image.c',
    'jmemcust.c',
    'link.c',
    'list-device.c',
    'load-bmp.c',
    'load-gif.c',
    'load-jpeg.c',
    'load-jpx.c',
    'load-jxr.c',
    'load-png.c',
    'load-pnm.c',
    'load-tiff.c',
    'load-webp.c',
    'memento.c',
    'memory.c',
    'noto.c',
    'outline.c',
    'output.c',
    'output-cbz.c',
    # 'output-pcl.c',
    # 'output-pclm.c',
    'output-png.c',
    'output-pnm.c',
    # 'output-ps.c',
    # 'output-psd.c',
    # 'output-pwg.c',
    'output-svg.c',
    'output-tga.c',
    'path.c',
    'pixmap.c',
    'pool.c',
    'printf.c',
    'random.c',
    'separation.c',
    'shade.c',
    'stext-device.c',
    'stext-output.c',
    'stext-search.c',
    'store.c',
    'stream-open.c',
    'stream-prog.c',
    'stream-read.c',
    'string.c',
    'strtof.c',
    'svg-device.c',
    'tempfile.c',
    'test-device.c',
    'text.c',
    'time.c',
    'trace-device.c',
    'track-usage.c',
    'transition.c',
    'tree.c',
    'ucdn.c',
    'untar.c',
    'unzip.c',
    'util.c',
    'writer.c',
    'xml.c',
    'zip.c',
  ],
  'source/pdf': [
    'pdf-annot.c',
    'pdf-annot-edit.c',
    'pdf-appearance.c',
    'pdf-clean.c',
    'pdf-clean-file.c',
    'pdf-cmap.c',
    'pdf-cmap-load.c',
    'pdf-cmap-parse.c',
    'pdf-cmap-table.c',
    'pdf-colorspace.c',
    'pdf-crypt.c',
    'pdf-device.c',
    'pdf-encoding.c',
    'pdf-event.c',
    'pdf-field.c',
    'pdf-font.c',
    'pdf-form.c',
    'pdf-function.c',
    'pdf-graft.c',
    'pdf-image.c',
    'pdf-interpret.c',
    'pdf-js.c',
    'pdf-layer.c',
    'pdf-lex.c',
    'pdf-metrics.c',
    'pdf-nametree.c',
    'pdf-object.c',
    'pdf-op-buffer.c',
    'pdf-op-filter.c',
    'pdf-op-run.c',
    'pdf-outline.c',
    'pdf-page.c',
    'pdf-parse.c',
    'pdf-pattern.c',
    'pdf-portfolio.c',
    'pdf-repair.c',
    'pdf-resources.c',
    'pdf-run.c',
    'pdf-shade.c',
    'pdf-signature.c',
    'pdf-store.c',
    'pdf-stream.c',
    'pdf-type3.c',
    'pdf-unicode.c',
    'pdf-write.c',
    'pdf-xobject.c',
    'pdf-xref.c',
  ],
  # 'source/xps': [
  #   'xps-common.c',
  #   'xps-doc.c',
  #   'xps-glyphs.c',
  #   'xps-gradient.c',
  #   'xps-image.c',
  #   'xps-link.c',
  #   'xps-outline.c',
  #   'xps-path.c',
  #   'xps-resource.c',
  #   'xps-tile.c',
  #   'xps-util.c',
  #   'xps-zip.c',
  #   ],
  # 'source/svg': [
  #   'svg-color.c',
  #   'svg-doc.c',
  #   'svg-parse.c',
  #   'svg-run.c',
  #   ],
  'source/html': [
    'css-apply.c',
    'css-parse.c',
    'epub-doc.c',
    'html-doc.c',
    'html-font.c',
    'html-layout.c',
  ],
  'source/cbz': [
    'mucbz.c',
    'muimg.c',
    'mutiff.c',
  ],
}
  foreach _entry : _files
    mupdf_src += _prefix / _entry
  endforeach
endforeach

namedump_exe = executable(
  'namedump',
  'scripts/namedump.c',
  native: true,
  override_options: 'b_lto=false',
)

resource_names = files('resources/pdf/names.txt')
generated_headers = []

subdir('include/mupdf/fitz')
subdir('include/mupdf/pdf')

hexdump_exe = executable(
  'hexdump',
  'scripts/hexdump.c',
  native: true,
  override_options: 'b_lto=false',
)

mupdf_src += custom_target(
  output: 'pdf-js-util.c',
  input: 'source/pdf/pdf-js-util.js',
  command: [hexdump_exe, '-s', '@OUTPUT@', 'source_pdf_pdf_js_util_js=@INPUT@'],
)

cmapdump_exe = executable(
  'cmapdump',
  ['scripts/cmapdump.c', generated_headers],
  include_directories: include_directories('include'),
  native: true,
  override_options: 'b_lto=false',
)

foreach _prefix, _files : {
  'cjk': [
    '83pv-RKSJ-H',
    '90msp-RKSJ-H',
    '90msp-RKSJ-V',
    '90ms-RKSJ-H',
    '90ms-RKSJ-V',
    '90pv-RKSJ-H',
    'Add-RKSJ-H',
    'Add-RKSJ-V',
    'Adobe-CNS1-UCS2',
    'Adobe-GB1-UCS2',
    'Adobe-Japan1-UCS2',
    'Adobe-Korea1-UCS2',
    'B5pc-H',
    'B5pc-V',
    'CNS-EUC-H',
    'CNS-EUC-V',
    'ETen-B5-H',
    'ETen-B5-V',
    'ETenms-B5-H',
    'ETenms-B5-V',
    'EUC-H',
    'EUC-V',
    'Ext-RKSJ-H',
    'Ext-RKSJ-V',
    'GB-EUC-H',
    'GB-EUC-V',
    'GBK2K-H',
    'GBK2K-V',
    'GBK-EUC-H',
    'GBK-EUC-V',
    'GBKp-EUC-H',
    'GBKp-EUC-V',
    'GBpc-EUC-H',
    'GBpc-EUC-V',
    'H',
    'HKscs-B5-H',
    'HKscs-B5-V',
    'KSC-EUC-H',
    'KSC-EUC-V',
    'KSCms-UHC-H',
    'KSCms-UHC-HW-H',
    'KSCms-UHC-HW-V',
    'KSCms-UHC-V',
    'KSCpc-EUC-H',
    'UniCNS-UCS2-H',
    'UniCNS-UCS2-V',
    'UniCNS-UTF16-H',
    'UniCNS-UTF16-V',
    'UniCNS-X',
    'UniGB-UCS2-H',
    'UniGB-UCS2-V',
    'UniGB-UTF16-H',
    'UniGB-UTF16-V',
    'UniGB-X',
    'UniJIS-UCS2-H',
    'UniJIS-UCS2-HW-H',
    'UniJIS-UCS2-HW-V',
    'UniJIS-UCS2-V',
    'UniJIS-UTF16-H',
    'UniJIS-UTF16-V',
    'UniJIS-X',
    'UniJIS-X16',
    'UniKS-UCS2-H',
    'UniKS-UCS2-V',
    'UniKS-UTF16-H',
    'UniKS-UTF16-V',
    'UniKS-X',
    'V',
  ],
  'extra': [
    '78-EUC-H',
    '78-EUC-V',
    '78-H',
    '78ms-RKSJ-H',
    '78ms-RKSJ-V',
    '78-RKSJ-H',
    '78-RKSJ-V',
    '78-V',
    '90pv-RKSJ-V',
    'Add-H',
    'Add-V',
    'Adobe-CNS1-0',
    'Adobe-CNS1-1',
    'Adobe-CNS1-2',
    'Adobe-CNS1-3',
    'Adobe-CNS1-4',
    'Adobe-CNS1-5',
    'Adobe-CNS1-6',
    'Adobe-GB1-0',
    'Adobe-GB1-1',
    'Adobe-GB1-2',
    'Adobe-GB1-3',
    'Adobe-GB1-4',
    'Adobe-GB1-5',
    'Adobe-Japan1-0',
    'Adobe-Japan1-1',
    'Adobe-Japan1-2',
    'Adobe-Japan1-3',
    'Adobe-Japan1-4',
    'Adobe-Japan1-5',
    'Adobe-Japan1-6',
    'Adobe-Korea1-0',
    'Adobe-Korea1-1',
    'Adobe-Korea1-2',
    'B5-H',
    'B5-V',
    'CNS1-H',
    'CNS1-V',
    'CNS2-H',
    'CNS2-V',
    'ETHK-B5-H',
    'ETHK-B5-V',
    'Ext-H',
    'Ext-V',
    'GB-H',
    'GBT-EUC-H',
    'GBT-EUC-V',
    'GBT-H',
    'GBTpc-EUC-H',
    'GBTpc-EUC-V',
    'GBT-V',
    'GB-V',
    'Hankaku',
    'Hiragana',
    'HKdla-B5-H',
    'HKdla-B5-V',
    'HKdlb-B5-H',
    'HKdlb-B5-V',
    'HKgccs-B5-H',
    'HKgccs-B5-V',
    'HKm314-B5-H',
    'HKm314-B5-V',
    'HKm471-B5-H',
    'HKm471-B5-V',
    'Katakana',
    'KSC-H',
    'KSC-Johab-H',
    'KSC-Johab-V',
    'KSCpc-EUC-V',
    'KSC-V',
    'NWP-H',
    'NWP-V',
    'RKSJ-H',
    'RKSJ-V',
    'Roman',
    'UniJIS2004-UTF16-H',
    'UniJIS2004-UTF16-V',
    'UniJISPro-UCS2-HW-V',
    'UniJISPro-UCS2-V',
    'WP-Symbol',
  ],
  'utf8': [
    'UniCNS-UTF8-H',
    'UniCNS-UTF8-V',
    'UniGB-UTF8-H',
    'UniGB-UTF8-V',
    'UniJIS2004-UTF8-H',
    'UniJIS2004-UTF8-V',
    'UniJISPro-UTF8-V',
    'UniJIS-UTF8-H',
    'UniJIS-UTF8-V',
    'UniJIS-X8',
    'UniKS-UTF8-H',
    'UniKS-UTF8-V',
  ],
  'utf32': [
    'UniCNS-UTF32-H',
    'UniCNS-UTF32-V',
    'UniGB-UTF32-H',
    'UniGB-UTF32-V',
    'UniJIS2004-UTF32-H',
    'UniJIS2004-UTF32-V',
    'UniJIS-UTF32-H',
    'UniJIS-UTF32-V',
    'UniJISX02132004-UTF32-H',
    'UniJISX02132004-UTF32-V',
    'UniJISX0213-UTF32-H',
    'UniJISX0213-UTF32-V',
    'UniJIS-X32',
    'UniKS-UTF32-H',
    'UniKS-UTF32-V',
  ],
}
  _inputs = []
  foreach _entry : _files
    _inputs += 'resources/cmaps' / _prefix / _entry
  endforeach
  _target = f'pdf-cmap-@_prefix@.c'
  mupdf_src += custom_target(
    _target,
    output: _target,
    input: _inputs,
    command: [cmapdump_exe, '@OUTPUT@', '@INPUT@'],
  )
endforeach

mupdf_lib = library(
  'mupdf',
  [
    generated_headers,
    mupdf_src,
  ],
  dependencies: [
    aes_dep,
    freetype_dep,
    harfbuzz_dep,
    jbig2dec_dep,
    jpeg_dep,
    log_dep,
    m_dep,
    mujs_dep,
    openjp2_dep,
    webp_dep,
    zlib_dep,
  ],
  include_directories: include_directories('include'),
  install: true,
)

mupdf_dep = declare_dependency(
  include_directories: include_directories('include'),
  link_with: mupdf_lib,
  sources: generated_headers,
)

meson.override_dependency('libmupdf', mupdf_dep)

install_subdir(
  'include/mupdf',
  exclude_files: [
    'fitz/config.meson.h',
    'fitz/meson.build',
    'pdf/meson.build',
  ],
  install_dir: get_option('includedir'),
  install_tag: 'devel',
  strip_directory: false,
)

pkg = import('pkgconfig')
pkg.generate(mupdf_lib)
