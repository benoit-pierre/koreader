project(
  'tesseract',
  'cpp',
  default_options: 'cpp_std=c++11',
  version: '3.05.00',
)

cxx = meson.get_compiler('cpp')

lept_dep = dependency('lept')

cdata = configuration_data()

cdata.set('GRAPHICS_DISABLED', true)
cdata.set('HAVE_LIBLEPT', 1)

foreach _header, _checks : {
  'sys/types.h': {
    'types': [
      'off_t',
    ],
  },
  'wchar.h': {
    'types': ['wchar_t'],
  },

}
  if _header != '' and not cxx.has_header(_header)
    continue
  endif
  cdata.set('HAVE_' + _header.underscorify().to_upper(), 1)
  _prefix = _header != '' ? f'#include<@_header@>' : ''
  foreach _type : _checks.get('types', [])
    if cxx.has_type(_type, prefix: _prefix)
      cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
    endif
  endforeach
  foreach _function : _checks.get('functions', [])
    if cxx.has_function(_function, prefix: _prefix)
      cdata.set('HAVE_' + _function.underscorify().to_upper(), 1)
    endif
  endforeach
endforeach

config_auto_h = configure_file(
  input: 'config_auto.h.meson',
  output: 'config_auto.h',
  configuration: cdata,
)

compile_args = []

# Fix crash when compiling with LTO enabled.
compile_args += cxx.get_supported_arguments('-fno-strict-aliasing')

compile_args += [
  '-DHAVE_CONFIG_H',
  '-DTESS_EXPORTS',
  '-DUSE_STD_NAMESPACE=1',
  '-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1',
]

if host_machine.system() == 'android'
  compile_args += '-DANDROID'
elif host_machine.system() == 'windows'
  compile_args += '-DWINDLLNAME="libtesseract.dll"'
endif

tesseract_src = (
  [config_auto_h] + files(
    'api/baseapi.cpp',
    'api/capi.cpp',
    'api/pdfrenderer.cpp',
    'api/renderer.cpp',
  )
)

foreach _prefix, _files : {
  'ccmain': [
    'adaptions.cpp',
    'applybox.cpp',
    'control.cpp',
    'cubeclassifier.cpp',
    'cube_control.cpp',
    'cube_reco_context.cpp',
    'docqual.cpp',
    'equationdetect.cpp',
    'fixspace.cpp',
    'fixxht.cpp',
    'ltrresultiterator.cpp',
    'osdetect.cpp',
    'output.cpp',
    'pageiterator.cpp',
    'pagesegmain.cpp',
    'pagewalk.cpp',
    'paragraphs.cpp',
    'paramsd.cpp',
    'par_control.cpp',
    'pgedit.cpp',
    'recogtraining.cpp',
    'reject.cpp',
    'resultiterator.cpp',
    'superscript.cpp',
    'tessbox.cpp',
    'tessedit.cpp',
    'tesseractclass.cpp',
    'tesseract_cube_combiner.cpp',
    'tessvars.cpp',
    'tfacepp.cpp',
    'thresholder.cpp',
    'werdit.cpp',
  ],
  'ccstruct': [
    'blamer.cpp',
    'blobbox.cpp',
    'blobs.cpp',
    'blread.cpp',
    'boxread.cpp',
    'boxword.cpp',
    'ccstruct.cpp',
    'coutln.cpp',
    'detlinefit.cpp',
    'dppoint.cpp',
    'fontinfo.cpp',
    'genblob.cpp',
    'imagedata.cpp',
    'linlsq.cpp',
    'matrix.cpp',
    'mod128.cpp',
    'normalis.cpp',
    'ocrblock.cpp',
    'ocrpara.cpp',
    'ocrrow.cpp',
    'otsuthr.cpp',
    'pageres.cpp',
    'params_training_featdef.cpp',
    'pdblock.cpp',
    'points.cpp',
    'polyaprx.cpp',
    'polyblk.cpp',
    'publictypes.cpp',
    'quadlsq.cpp',
    'quspline.cpp',
    'ratngs.cpp',
    'rect.cpp',
    'rejctmap.cpp',
    'seam.cpp',
    'split.cpp',
    'statistc.cpp',
    'stepblob.cpp',
    'vecfuncs.cpp',
    'werd.cpp',
  ],
  'ccutil': [
    'ambigs.cpp',
    'basedir.cpp',
    'bits16.cpp',
    'bitvector.cpp',
    'ccutil.cpp',
    'clst.cpp',
    'elst2.cpp',
    'elst.cpp',
    'errcode.cpp',
    'globaloc.cpp',
    'indexmapbidi.cpp',
    'mainblk.cpp',
    'memry.cpp',
    'params.cpp',
    'scanutils.cpp',
    'serialis.cpp',
    'strngs.cpp',
    'tessdatamanager.cpp',
    'tprintf.cpp',
    'unichar.cpp',
    'unicharmap.cpp',
    'unicharset.cpp',
    'unicodes.cpp',
    'universalambigs.cpp',
  ],
  'classify': [
    'adaptive.cpp',
    'adaptmatch.cpp',
    'blobclass.cpp',
    'classify.cpp',
    'cluster.cpp',
    'clusttool.cpp',
    'cutoffs.cpp',
    'errorcounter.cpp',
    'featdefs.cpp',
    'float2int.cpp',
    'fpoint.cpp',
    'intfeaturedist.cpp',
    'intfeaturemap.cpp',
    'intfeaturespace.cpp',
    'intfx.cpp',
    'intmatcher.cpp',
    'intproto.cpp',
    'kdtree.cpp',
    'mastertrainer.cpp',
    'mf.cpp',
    'mfdefs.cpp',
    'mfoutline.cpp',
    'mfx.cpp',
    'normfeat.cpp',
    'normmatch.cpp',
    'ocrfeatures.cpp',
    'outfeat.cpp',
    'picofeat.cpp',
    'protos.cpp',
    'sampleiterator.cpp',
    'shapeclassifier.cpp',
    'shapetable.cpp',
    'tessclassifier.cpp',
    'trainingsample.cpp',
    'trainingsampleset.cpp',
  ],
  'cube': [
    'altlist.cpp',
    'beam_search.cpp',
    'bmp_8.cpp',
    'cached_file.cpp',
    'char_altlist.cpp',
    'char_bigrams.cpp',
    'char_samp.cpp',
    'char_samp_enum.cpp',
    'char_samp_set.cpp',
    'char_set.cpp',
    'classifier_factory.cpp',
    'con_comp.cpp',
    'conv_net_classifier.cpp',
    'cube_line_object.cpp',
    'cube_line_segmenter.cpp',
    'cube_object.cpp',
    'cube_search_object.cpp',
    'cube_tuning_params.cpp',
    'cube_utils.cpp',
    'feature_bmp.cpp',
    'feature_chebyshev.cpp',
    'feature_hybrid.cpp',
    'hybrid_neural_net_classifier.cpp',
    'search_column.cpp',
    'search_node.cpp',
    'tess_lang_mod_edge.cpp',
    'tess_lang_model.cpp',
    'word_altlist.cpp',
    'word_list_lang_model.cpp',
    'word_size_model.cpp',
    'word_unigrams.cpp',
  ],
  'cutil': [
    'bitvec.cpp',
    'callcpp.cpp',
    'cutil_class.cpp',
    'cutil.cpp',
    'danerror.cpp',
    'efio.cpp',
    'emalloc.cpp',
    'freelist.cpp',
    'listio.cpp',
    'oldlist.cpp',
    'structures.cpp',
  ],
  'dict': [
    'context.cpp',
    'dawg_cache.cpp',
    'dawg.cpp',
    'dict.cpp',
    'hyphen.cpp',
    'permdawg.cpp',
    'stopper.cpp',
    'trie.cpp',
  ],
  'neural_networks/runtime': [
    'input_file_buffer.cpp',
    'neural_net.cpp',
    'neuron.cpp',
    'sigmoid_table.cpp',
  ],
  'opencl': [
    'openclwrapper.cpp',
  ],
  'textord': [
    'alignedblob.cpp',
    'baselinedetect.cpp',
    'bbgrid.cpp',
    'blkocc.cpp',
    'blobgrid.cpp',
    'ccnontextdetect.cpp',
    'cjkpitch.cpp',
    'colfind.cpp',
    'colpartition.cpp',
    'colpartitiongrid.cpp',
    'colpartitionset.cpp',
    'devanagari_processing.cpp',
    'drawedg.cpp',
    'drawtord.cpp',
    'edgblob.cpp',
    'edgloop.cpp',
    'equationdetectbase.cpp',
    'fpchop.cpp',
    'gap_map.cpp',
    'imagefind.cpp',
    'linefind.cpp',
    'makerow.cpp',
    'oldbasel.cpp',
    'pithsync.cpp',
    'pitsync1.cpp',
    'scanedg.cpp',
    'sortflts.cpp',
    'strokewidth.cpp',
    'tabfind.cpp',
    'tablefind.cpp',
    'tablerecog.cpp',
    'tabvector.cpp',
    'textlineprojection.cpp',
    'textord.cpp',
    'topitch.cpp',
    'tordmain.cpp',
    'tospace.cpp',
    'tovars.cpp',
    'underlin.cpp',
    'wordseg.cpp',
    'workingpartset.cpp',
  ],
  'viewer': [
    'scrollview.cpp',
    'svmnode.cpp',
    'svpaint.cpp',
    'svutil.cpp',
  ],
  'wordrec': [
    'associate.cpp',
    'chop.cpp',
    'chopper.cpp',
    'drawfx.cpp',
    'findseam.cpp',
    'gradechop.cpp',
    'language_model.cpp',
    'lm_consistency.cpp',
    'lm_pain_points.cpp',
    'lm_state.cpp',
    'outlines.cpp',
    'params_model.cpp',
    'pieces.cpp',
    'plotedges.cpp',
    'render.cpp',
    'segsearch.cpp',
    'tface.cpp',
    'wordclass.cpp',
    'wordrec.cpp',
  ],
}
  foreach _entry : _files
    tesseract_src += files(_prefix / _entry)
  endforeach
endforeach

tesseract_lib = library(
  'tesseract',
  dependencies: [lept_dep],
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: include_directories(
    'api',
    'ccmain',
    'ccstruct',
    'ccutil',
    'classify',
    'cube',
    'cutil',
    'dict',
    'neural_networks/runtime',
    'opencl',
    'textord',
    'vs2010/port',
    'viewer',
    'wordrec',
  ),
  cpp_args: compile_args,
  install: true,
  soversion: 3,
  sources: tesseract_src,
)

tesseract_dep = declare_dependency(
  include_directories: include_directories(
    '.', # fugly: for `config_auto.h`
    'api',
    'ccmain',
    'ccstruct',
    'ccutil',
  ),
  link_with: tesseract_lib,
)

meson.override_dependency('tesseract', tesseract_dep)
