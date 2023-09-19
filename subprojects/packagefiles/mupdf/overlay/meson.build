project(
  'mupdf',
  ['c', 'cpp'],
  version: '1.22.1',
  default_options: 'c_std=gnu11',
)

cc = meson.get_compiler('c')

aes_dep = dependency('libaes', required: false)
freetype_dep = dependency('freetype2')
gumbo_parser_dep = dependency('gumbo')
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
# No Office Open XML or OpenDocument or OCR output support.
cdata.set10('FZ_ENABLE_DOCX_OUTPUT', false)
cdata.set10('FZ_ENABLE_ODT_OUTPUT', false)
cdata.set10('FZ_ENABLE_OCR_OUTPUT', false)
# No EPUB support.
cdata.set10('FZ_ENABLE_EPUB', false)
# No ICC support (and lcms2 dependency).
cdata.set10('FZ_ENABLE_ICC', false)
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

subdir('include/mupdf/fitz')

mupdf_src = [fitz_config_h]

foreach _prefix, _files : {
  'source/fitz': [
    'archive.c',
    'bbox-device.c',
    'bidi.c',
    'bidi-std.c',
    'bitmap.c',
    'buffer.c',
    'color-fast.c',
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
    'encode-fax.c',
    'encodings.c',
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
    'glyphbox.c',
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
    'load-jbig2.c',
    'load-jpeg.c',
    'load-jpx.c',
    'load-jxr.c',
    'load-png.c',
    'load-pnm.c',
    'load-tiff.c',
    'load-webp.c',
    'log.c',
    'memento.c',
    'memory.c',
    'noto.c',
    'ocr-device.c',
    'outline.c',
    'output.c',
    'output-cbz.c',
    # 'output-docx.c',
    'output-jpeg.c',
    # 'output-pcl.c',
    # 'output-pclm.c',
    # 'output-pdfocr.c',
    'output-png.c',
    'output-pnm.c',
    # 'output-ps.c',
    # 'output-psd.c',
    # 'output-pwg.c',
    'output-svg.c',
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
    'stream-read.c',
    'string.c',
    'strtof.c',
    'svg-device.c',
    'tessocr.cpp',
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
    'warp.c',
    'writer.c',
    'xml.c',
    'xmltext-device.c',
    'zip.c',
  ],
  'source/pdf': [
    'pdf-annot.c',
    'pdf-appearance.c',
    'pdf-clean.c',
    'pdf-clean-file.c',
    'pdf-cmap.c',
    'pdf-cmap-load.c',
    'pdf-cmap-parse.c',
    'pdf-colorspace.c',
    'pdf-crypt.c',
    'pdf-device.c',
    'pdf-event.c',
    'pdf-font-add.c',
    'pdf-font.c',
    'pdf-form.c',
    'pdf-function.c',
    'pdf-graft.c',
    'pdf-image.c',
    'pdf-interpret.c',
    'pdf-js.c',
    'pdf-layer.c',
    'pdf-layout.c',
    'pdf-lex.c',
    'pdf-link.c',
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
    'pdf-repair.c',
    'pdf-resources.c',
    'pdf-run.c',
    'pdf-shade.c',
    'pdf-signature.c',
    'pdf-store.c',
    'pdf-stream.c',
    'pdf-type3.c',
    'pdf-unicode.c',
    'pdf-util.c',
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
    'html-outline.c',
    'html-parse.c',
    'mobi.c',
    'story-writer.c',
    'xml-dom.c',
  ],
  'source/reflow': [
    'reflow-doc.c',
  ],
  'source/cbz': [
    'mucbz.c',
    'muimg.c',
  ],
}
  foreach _entry : _files
    mupdf_src += _prefix / _entry
  endforeach
endforeach

mupdf_lib = library(
  'mupdf',
  mupdf_src,
  dependencies: [
    aes_dep,
    freetype_dep,
    gumbo_parser_dep,
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
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: include_directories('include', 'include/mupdf'),
  install: true,
)

mupdf_dep = declare_dependency(
  include_directories: include_directories('include'),
  link_with: mupdf_lib,
)

meson.override_dependency('libmupdf', mupdf_dep)

install_subdir(
  'include/mupdf',
  exclude_files: [
    'fitz/config.meson.h',
    'fitz/meson.build',
  ],
  install_dir: get_option('includedir'),
  install_tag: 'devel',
  strip_directory: false,
)

pkg = import('pkgconfig')
pkg.generate(mupdf_lib)
