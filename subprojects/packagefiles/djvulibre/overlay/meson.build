project(
  'djvulibre',
  'cpp',
  default_options: [
    'cpp_std=c++11',
  ],
  version: '3.5.28',
)

cxx = meson.get_compiler('cpp')

cpp_args = []

m_dep = cxx.find_library('m', required: false)
libjpeg_dep = dependency('libjpeg', required: get_option('libjpeg'))
libtiff_dep = dependency('libtiff-4', required: get_option('libtiff-4'))
threads_dep = dependency('threads', required: false)

deps = [libjpeg_dep, libtiff_dep, m_dep, threads_dep]

cdata = configuration_data()

if host_machine.system() == 'darwin'
  cdata.set('HAVE_OS_APPLE', 1)
  deps += dependency('appleframeworks', modules: 'CoreFoundation')
endif

if host_machine.system() == 'windows'
  cdata.set('HAVE_OS_WIN32', 1)
  cpp_args += [
    '-DDJVUAPI_EXPORT',
    '-DDDJVUAPI_EXPORT',
    '-DMINILISPAPI_EXPORT',
  ]
endif

if host_machine.system() != 'window' and cxx.has_function_attribute('visibility:default')
  _vis_attr = '__attribute__ ((visibility("default")))'
  cpp_args += [
    '-DDJVUAPI=' + _vis_attr,
    '-DDDJVUAPI=' + _vis_attr,
    '-DMINILISPAPI=' + _vis_attr,
  ]
endif

cdata.set_quoted('DJVULIBRE_VERSION', meson.project_version())
cdata.set_quoted('DIR_DATADIR', get_option('datadir'))

if libjpeg_dep.found()
  cdata.set('HAVE_JPEG', 1)
endif

if libtiff_dep.found()
  cdata.set('HAVE_TIFF', 1)
endif

if cxx.has_header('pthread.h') and threads_dep.found()
  cdata.set('HAVE_PTHREAD', 1)
endif

if m_dep.found()
  cdata.set('HAVE_LIBM', 1)
endif

if cxx.get_id() == 'gcc'
  cdata.set('HAVE_EXCEPTIONS', 1)
endif

foreach _header, _checks : {
  '': {
    'types': [
      'bool',
      'long long int',
    ],
  },
  'cpuid.h': {},
  'dirent.h': {},
  'dlfcn.h': {},
  'getopt.h': {},
  'iconv.h': {
    'functions': ['iconv'],
  },
  'inttypes.h': {},
  'langinfo.h': {
    'functions': ['nl_langinfo'],
  },
  'ndir.h': {},
  'new.h': {},
  'pwd.h': {
    'functions': ['getpwuid'],
  },
  'sched.h': {
    'functions': ['sched_yield'],
  },
  'signal.h': {
    'functions': ['sigaction'],
  },
  'stdint.h': {},
  'stdio.h': {
    'functions': [
      'fseeko',
      'putc_unlocked',
      'vsnprintf',
    ],
  },
  'stdlib.h': {
    'functions': [
      'mkstemp',
      'setenv',
    ],
  },
  'strings.h': {},
  'string.h': {
    'functions': ['strerror'],
  },
  'sys/dir.h': {},
  'sys/ipc.h': {},
  'sys/mman.h': {
    'functions': ['mmap'],
  },
  'sys/ndir.h': {},
  'sys/param.h': {},
  'sys/shm.h': {},
  'sys/stat.h': {},
  'sys/time.h': {},
  'sys/types.h': {},
  'sys/wait.h': {},
  'time.h': {
    'functions': ['strftime'],
  },
  'vfork.h': {},
  'unistd.h': {
    'types': [],
    'functions': [
      'fork',
      'gethostname',
      'getpagesize',
      'vfork',
    ],
  },
  'wchar.h': {
    'types': ['mbstate_t', 'wchar_t'],
    'functions': ['wcrtomb'],
  },
  'wctype.h': {'functions': ['iswspace']},
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

if cdata.get('HAVE_SYS_TIME_H', 0) == 1
  cdata.set('TIME_WITH_SYS_TIME', 1)
endif

foreach _name, _source : {
  'exceptions': '''
  int fn(void) { try { throw  1; } catch (int i) { return i; } }
  ''',
  'gcctls': '''
  __thread int i;
  int fn(void) { return i; }
  ''',
  'intel_atomic_builtins': '''
  static int volatile l;
  int fn(void) {
    __sync_lock_test_and_set(&l,1);
    __sync_lock_release(&l);
    __sync_add_and_fetch(&l,1);
    __sync_bool_compare_and_swap(&l,&l,1);
    __sync_synchronize();
    return 0;
  }
  ''',
  'member templates': '''
  template<class T, int N> class A
  { public:
    template<int N2> A<T,N> operator=(const A<T,N2>& z) { return A<T,N>(); }
  };
  int fn(void) {
    A<double,4> x; A<double,7> y;
    x = y;
    return 0;
  }
  ''',
  'namespaces': '''
  namespace Outer { namespace Inner { int i = 0; }}
  int fn(void) {
    using namespace Outer::Inner;
    return i;
  }
  ''',
  'stdincludes': '''
  #include <new>
  struct X { int a; X(int a):a(a){} };
  X* foo(void *x) { return new(x) X(2); }
  int fn(void) { return 0; }
  ''',
  'typename': '''
  template<typename T>class X {public:X(){}};
  int fn(void) { X<float> z; return 0; }
  ''',
}
  if cxx.compiles(_source, name: _name)
    cdata.set('HAVE_' + _name.underscorify().to_upper(), 1)
  endif
endforeach

config_h = configure_file(
  input: 'config.h.meson',
  output: 'config.h',
  configuration: cdata,
)

djvulibre_lib = library(
  'djvulibre',
  [config_h] + [
    'libdjvu/Arrays.cpp',
    'libdjvu/BSByteStream.cpp',
    'libdjvu/BSEncodeByteStream.cpp',
    'libdjvu/ByteStream.cpp',
    'libdjvu/DataPool.cpp',
    'libdjvu/DjVmDir.cpp',
    'libdjvu/DjVmDir0.cpp',
    'libdjvu/DjVmDoc.cpp',
    'libdjvu/DjVmNav.cpp',
    'libdjvu/DjVuAnno.cpp',
    'libdjvu/DjVuDocEditor.cpp',
    'libdjvu/DjVuDocument.cpp',
    'libdjvu/DjVuDumpHelper.cpp',
    'libdjvu/DjVuErrorList.cpp',
    'libdjvu/DjVuFile.cpp',
    'libdjvu/DjVuFileCache.cpp',
    'libdjvu/DjVuGlobal.cpp',
    'libdjvu/DjVuGlobalMemory.cpp',
    'libdjvu/DjVuImage.cpp',
    'libdjvu/DjVuInfo.cpp',
    'libdjvu/DjVuMessage.cpp',
    'libdjvu/DjVuMessageLite.cpp',
    'libdjvu/DjVuNavDir.cpp',
    'libdjvu/DjVuPalette.cpp',
    'libdjvu/DjVuPort.cpp',
    'libdjvu/DjVuText.cpp',
    'libdjvu/DjVuToPS.cpp',
    'libdjvu/GBitmap.cpp',
    'libdjvu/GContainer.cpp',
    'libdjvu/GException.cpp',
    'libdjvu/GIFFManager.cpp',
    'libdjvu/GMapAreas.cpp',
    'libdjvu/GOS.cpp',
    'libdjvu/GPixmap.cpp',
    'libdjvu/GRect.cpp',
    'libdjvu/GScaler.cpp',
    'libdjvu/GSmartPointer.cpp',
    'libdjvu/GString.cpp',
    'libdjvu/GThreads.cpp',
    'libdjvu/GURL.cpp',
    'libdjvu/GUnicode.cpp',
    'libdjvu/IFFByteStream.cpp',
    'libdjvu/IW44EncodeCodec.cpp',
    'libdjvu/IW44Image.cpp',
    'libdjvu/JB2EncodeCodec.cpp',
    'libdjvu/JB2Image.cpp',
    'libdjvu/JPEGDecoder.cpp',
    'libdjvu/MMRDecoder.cpp',
    'libdjvu/MMX.cpp',
    'libdjvu/UnicodeByteStream.cpp',
    'libdjvu/XMLParser.cpp',
    'libdjvu/XMLTags.cpp',
    'libdjvu/ZPCodec.cpp',
    'libdjvu/atomic.cpp',
    'libdjvu/ddjvuapi.cpp',
    'libdjvu/debug.cpp',
    'libdjvu/miniexp.cpp',
  ],
  cpp_args: cpp_args + ['-include', 'config.h'],
  dependencies: deps,
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: include_directories('libdjvu'),
  install: true,
)

djvulibre_dep = declare_dependency(
  compile_args: cpp_args,
  include_directories: include_directories('.'),
  link_with: djvulibre_lib,
)

meson.override_dependency('djvulibre', djvulibre_dep)
