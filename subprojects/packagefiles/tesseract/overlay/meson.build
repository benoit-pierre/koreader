project(
  'tesseract', 'cpp',
  version: files('VERSION'),
  default_options: 'cpp_std=c++17',
)

cxx = meson.get_compiler('cpp')

lept_dep = dependency('lept')

cdata = configuration_data()

_version = meson.project_version()
_version_parts = _version.split('.')
cdata.set('GENERIC_MAJOR_VERSION', _version_parts[0])
cdata.set('GENERIC_MINOR_VERSION', _version_parts[1])
cdata.set('GENERIC_MICRO_VERSION', _version_parts[2])
cdata.set('PACKAGE_VERSION', _version)

cdata.set('GRAPHICS_DISABLED', true)
cdata.set('DISABLED_LEGACY_ENGINE', not get_option('legacy-engine'))

foreach _fn : [
  'feenableexcept',
]
  if cxx.has_function(_fn)
    cdata.set('HAVE_' + _fn.underscorify().to_upper(), true)
  endif
endforeach

compile_args = []

# Fix crash when compiling with LTO enabled.
# compile_args += cxx.get_supported_arguments('-fno-strict-aliasing')

compile_args += [
  '-DHAVE_CONFIG_H',
  '-DTESS_EXPORTS',
  '-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1',
]

if host_machine.system() == 'android'
  compile_args += '-DANDROID'
elif host_machine.system() == 'windows'
  compile_args += '-DWINDLLNAME="libtesseract.dll"'
endif

tesseract_inc = [
  'include',
  'src/arch',
  'src/ccmain',
  'src/ccstruct',
  'src/ccutil',
  'src/classify',
  'src/cutil',
  'src/dict',
  'src/lstm',
  'src/opencl',
  'src/textord',
  'src/viewer',
  'src/wordrec',
]

tesseract_src = [
  'src/api/altorenderer.cpp',
  'src/api/baseapi.cpp',
  'src/api/capi.cpp',
  'src/api/hocrrenderer.cpp',
  'src/api/lstmboxrenderer.cpp',
  'src/api/pdfrenderer.cpp',
  'src/api/renderer.cpp',
  'src/api/wordstrboxrenderer.cpp',
  'src/ccmain/applybox.cpp',
  'src/ccmain/control.cpp',
  'src/ccmain/linerec.cpp',
  'src/ccmain/ltrresultiterator.cpp',
  'src/ccmain/mutableiterator.cpp',
  'src/ccmain/output.cpp',
  'src/ccmain/pageiterator.cpp',
  'src/ccmain/pagesegmain.cpp',
  'src/ccmain/pagewalk.cpp',
  'src/ccmain/paragraphs.cpp',
  'src/ccmain/paramsd.cpp',
  'src/ccmain/pgedit.cpp',
  'src/ccmain/reject.cpp',
  'src/ccmain/resultiterator.cpp',
  'src/ccmain/tessedit.cpp',
  'src/ccmain/tesseractclass.cpp',
  'src/ccmain/tessvars.cpp',
  'src/ccmain/thresholder.cpp',
  'src/ccmain/werdit.cpp',
  'src/ccstruct/blamer.cpp',
  'src/ccstruct/blobbox.cpp',
  'src/ccstruct/blobs.cpp',
  'src/ccstruct/blread.cpp',
  'src/ccstruct/boxread.cpp',
  'src/ccstruct/boxword.cpp',
  'src/ccstruct/ccstruct.cpp',
  'src/ccstruct/coutln.cpp',
  'src/ccstruct/detlinefit.cpp',
  'src/ccstruct/dppoint.cpp',
  'src/ccstruct/fontinfo.cpp',
  'src/ccstruct/image.cpp',
  'src/ccstruct/imagedata.cpp',
  'src/ccstruct/linlsq.cpp',
  'src/ccstruct/matrix.cpp',
  'src/ccstruct/mod128.cpp',
  'src/ccstruct/normalis.cpp',
  'src/ccstruct/ocrblock.cpp',
  'src/ccstruct/ocrpara.cpp',
  'src/ccstruct/ocrrow.cpp',
  'src/ccstruct/otsuthr.cpp',
  'src/ccstruct/pageres.cpp',
  'src/ccstruct/params_training_featdef.cpp',
  'src/ccstruct/pdblock.cpp',
  'src/ccstruct/points.cpp',
  'src/ccstruct/polyaprx.cpp',
  'src/ccstruct/polyblk.cpp',
  'src/ccstruct/quadlsq.cpp',
  'src/ccstruct/quspline.cpp',
  'src/ccstruct/ratngs.cpp',
  'src/ccstruct/rect.cpp',
  'src/ccstruct/rejctmap.cpp',
  'src/ccstruct/seam.cpp',
  'src/ccstruct/split.cpp',
  'src/ccstruct/statistc.cpp',
  'src/ccstruct/stepblob.cpp',
  'src/ccstruct/werd.cpp',
  'src/ccutil/ccutil.cpp',
  'src/ccutil/clst.cpp',
  'src/ccutil/elst.cpp',
  'src/ccutil/elst2.cpp',
  'src/ccutil/errcode.cpp',
  'src/ccutil/params.cpp',
  'src/ccutil/scanutils.cpp',
  'src/ccutil/serialis.cpp',
  'src/ccutil/tessdatamanager.cpp',
  'src/ccutil/tprintf.cpp',
  'src/ccutil/unichar.cpp',
  'src/ccutil/unicharcompress.cpp',
  'src/ccutil/unicharmap.cpp',
  'src/ccutil/unicharset.cpp',
  'src/classify/classify.cpp',
  'src/dict/context.cpp',
  'src/dict/dawg.cpp',
  'src/dict/dawg_cache.cpp',
  'src/dict/dict.cpp',
  'src/dict/stopper.cpp',
  'src/dict/trie.cpp',
  'src/lstm/convolve.cpp',
  'src/lstm/fullyconnected.cpp',
  'src/lstm/functions.cpp',
  'src/lstm/input.cpp',
  'src/lstm/lstm.cpp',
  'src/lstm/lstmrecognizer.cpp',
  'src/lstm/maxpool.cpp',
  'src/lstm/network.cpp',
  'src/lstm/networkio.cpp',
  'src/lstm/parallel.cpp',
  'src/lstm/plumbing.cpp',
  'src/lstm/recodebeam.cpp',
  'src/lstm/reconfig.cpp',
  'src/lstm/reversed.cpp',
  'src/lstm/series.cpp',
  'src/lstm/stridemap.cpp',
  'src/lstm/tfnetwork.cpp',
  'src/lstm/weightmatrix.cpp',
  'src/opencl/openclwrapper.cpp',
  'src/svpaint.cpp',
  'src/textord/alignedblob.cpp',
  'src/textord/baselinedetect.cpp',
  'src/textord/bbgrid.cpp',
  'src/textord/blkocc.cpp',
  'src/textord/blobgrid.cpp',
  'src/textord/ccnontextdetect.cpp',
  'src/textord/cjkpitch.cpp',
  'src/textord/colfind.cpp',
  'src/textord/colpartition.cpp',
  'src/textord/colpartitiongrid.cpp',
  'src/textord/colpartitionset.cpp',
  'src/textord/devanagari_processing.cpp',
  'src/textord/drawtord.cpp',
  'src/textord/edgblob.cpp',
  'src/textord/edgloop.cpp',
  'src/textord/fpchop.cpp',
  'src/textord/gap_map.cpp',
  'src/textord/imagefind.cpp',
  'src/textord/linefind.cpp',
  'src/textord/makerow.cpp',
  'src/textord/oldbasel.cpp',
  'src/textord/pithsync.cpp',
  'src/textord/pitsync1.cpp',
  'src/textord/scanedg.cpp',
  'src/textord/sortflts.cpp',
  'src/textord/strokewidth.cpp',
  'src/textord/tabfind.cpp',
  'src/textord/tablefind.cpp',
  'src/textord/tablerecog.cpp',
  'src/textord/tabvector.cpp',
  'src/textord/textlineprojection.cpp',
  'src/textord/textord.cpp',
  'src/textord/topitch.cpp',
  'src/textord/tordmain.cpp',
  'src/textord/tospace.cpp',
  'src/textord/tovars.cpp',
  'src/textord/underlin.cpp',
  'src/textord/wordseg.cpp',
  'src/textord/workingpartset.cpp',
  'src/viewer/scrollview.cpp',
  'src/viewer/svmnode.cpp',
  'src/viewer/svutil.cpp',
  'src/wordrec/tface.cpp',
  'src/wordrec/wordrec.cpp',
]

if get_option('legacy-engine')
  tesseract_src += [
    'src/ccmain/adaptions.cpp',
    'src/ccmain/docqual.cpp',
    'src/ccmain/equationdetect.cpp',
    'src/ccmain/fixspace.cpp',
    'src/ccmain/fixxht.cpp',
    'src/ccmain/osdetect.cpp',
    'src/ccmain/par_control.cpp',
    'src/ccmain/recogtraining.cpp',
    'src/ccmain/superscript.cpp',
    'src/ccmain/tessbox.cpp',
    'src/ccmain/tfacepp.cpp',
    'src/ccutil/ambigs.cpp',
    'src/ccutil/bitvector.cpp',
    'src/ccutil/indexmapbidi.cpp',
    'src/classify/adaptive.cpp',
    'src/classify/adaptmatch.cpp',
    'src/classify/blobclass.cpp',
    'src/classify/cluster.cpp',
    'src/classify/clusttool.cpp',
    'src/classify/cutoffs.cpp',
    'src/classify/featdefs.cpp',
    'src/classify/float2int.cpp',
    'src/classify/fpoint.cpp',
    'src/classify/intfeaturespace.cpp',
    'src/classify/intfx.cpp',
    'src/classify/intmatcher.cpp',
    'src/classify/intproto.cpp',
    'src/classify/kdtree.cpp',
    'src/classify/mf.cpp',
    'src/classify/mfoutline.cpp',
    'src/classify/mfx.cpp',
    'src/classify/normfeat.cpp',
    'src/classify/normmatch.cpp',
    'src/classify/ocrfeatures.cpp',
    'src/classify/outfeat.cpp',
    'src/classify/picofeat.cpp',
    'src/classify/protos.cpp',
    'src/classify/shapeclassifier.cpp',
    'src/classify/shapetable.cpp',
    'src/classify/tessclassifier.cpp',
    'src/classify/trainingsample.cpp',
    'src/cutil/oldlist.cpp',
    'src/dict/hyphen.cpp',
    'src/dict/permdawg.cpp',
    'src/textord/equationdetectbase.cpp',
    'src/wordrec/associate.cpp',
    'src/wordrec/chop.cpp',
    'src/wordrec/chopper.cpp',
    'src/wordrec/drawfx.cpp',
    'src/wordrec/findseam.cpp',
    'src/wordrec/gradechop.cpp',
    'src/wordrec/language_model.cpp',
    'src/wordrec/lm_consistency.cpp',
    'src/wordrec/lm_pain_points.cpp',
    'src/wordrec/lm_state.cpp',
    'src/wordrec/outlines.cpp',
    'src/wordrec/params_model.cpp',
    'src/wordrec/pieces.cpp',
    'src/wordrec/render.cpp',
    'src/wordrec/segsearch.cpp',
    'src/wordrec/wordclass.cpp',
  ]
endif

# SIMD support.
simd_flags = cxx.get_supported_arguments('-ffast-math')
simd_src = [
  'src/arch/dotproduct.cpp',
  'src/arch/intsimdmatrix.cpp',
  'src/arch/simddetect.cpp',
]
if get_option('simd')
  if host_machine.cpu_family() == 'aarch64' or cxx.has_argument('-mfpu=neon')
    _usable_neon = false
    foreach _fn : [
      'getauxval',
      'elf_aux_info',
      'android_getCpuFamily',
    ]
      if cxx.has_function(_fn)
        cdata.set('HAVE_' + _fn.underscorify().to_upper(), true)
        _usable_neon = true
        break
      endif
    endforeach
    if not _usable_neon
      warning(
        'NEON is available, but we don\'t know how to check for it, so we wont be able to use it.',
      )
    endif
    if host_machine.cpu_family() != 'aarch64'
      simd_flags += ['-mfpu=neon']
    endif
    simd_flags += ['-DHAVE_NEON']
    simd_src += [
      'src/arch/intsimdmatrixneon.cpp',
      'src/arch/dotproductneon.cpp',
    ]
  endif
  foreach _flag, _src : {
    '-fopenmp-simd': [],
    '-mavx'        : 'src/arch/dotproductavx.cpp',
    '-mavx2'       : 'src/arch/intsimdmatrixavx2.cpp',
    '-mavx512f'    : 'src/arch/dotproductavx512.cpp',
    '-mfma'        : 'src/arch/dotproductfma.cpp',
    '-msse4.1'     : ['src/arch/dotproductsse.cpp', 'src/arch/intsimdmatrixsse.cpp'],
  }
    if cxx.has_argument(_flag)
      simd_flags += [
        _flag,
        '-DHAVE_' + _flag.substring(2).underscorify().to_upper(),
      ]
      simd_src += _src
    endif
  endforeach
endif

subdir('include')
subdir('include/tesseract')

simd_lib = static_library(
  'tesseract-simd',
  [config_auto_h, tess_version_h, simd_src],
  cpp_args: simd_flags + compile_args,
  gnu_symbol_visibility: 'inlineshidden',
  implicit_include_directories: false,
  include_directories: include_directories(tesseract_inc),
  override_options: ['b_lto=false'],
)

tesseract_lib = library(
  'tesseract',
  [config_auto_h, tess_version_h, tesseract_src],
  dependencies: [lept_dep],
  gnu_symbol_visibility: 'inlineshidden',
  implicit_include_directories: false,
  include_directories: include_directories(tesseract_inc),
  cpp_args: compile_args,
  install: true,
  link_with: simd_lib,
  soversion: 3,
)

tesseract_dep = declare_dependency(
  include_directories: include_directories('include'),
  link_with: tesseract_lib,
)

meson.override_dependency('tesseract', tesseract_dep)
