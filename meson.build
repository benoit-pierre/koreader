project(
  'koreader',
  ['c', 'cpp'],
  version: '2023.01',
  meson_version: '>= 0.64.0',
  default_options: ['c_std=gnu11', 'cpp_std=gnu++17'],
)

opt_amalgam = get_option('static_if_amalgam') == 'static'

cc = meson.get_compiler('c')
fs = import('fs')

toinstall = {}

global_comp_args = [
  '-fno-direct-access-external-data', '-mno-direct-extern-access', # Clang / GCC.
  '-fno-semantic-interposition',
  '-fvisibility-inlines-hidden',
]
global_link_args = [
  '-Wl,-Bsymbolic-functions',
]

if host_machine.system() == 'android'
  cc = meson.get_compiler('c')
  # Workaround https://github.com/mesonbuild/meson/issues/3519
  # NOTE: to be removed when upgrading requirements to `API >= 24`.
  # (https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md)
  if cc.sizeof('void *') == 4 and not meson.get_compiler('c').has_function('fseeko64')
    global_comp_args += ['-U_FILE_OFFSET_BITS', '-D_FILE_OFFSET_BITS=32']
  endif
  # Fix linker issues with dynamic relocations on read-only sections.
  if host_machine.cpu_family() == 'x86'
    global_link_args += '-Wl,-z,notext'
  endif
endif

if host_machine.system() == 'darwin'
  # FIXME: does not work; meson overrides the runtime
  # path at install, so we have to fix it again in
  # our `fix-libs.py` install script.
  global_link_args += '-Wl,-rpath,@executable_path/libs,-rpath,@executable_path/../koreader/libs'
else
  global_link_args += '-Wl,-z,origin,-rpath=$ORIGIN,-rpath=$ORIGIN/libs,--disable-new-dtags'
endif

if host_machine.cpu_family() in ['x86', 'x86_64']
  global_link_args += '-Wl,-z,pack-relative-relocs'
endif

foreach _language : ['c', 'cpp']
  _compiler = meson.get_compiler(_language)
  add_global_arguments(
    _compiler.get_supported_arguments(global_comp_args),
    language: _language,
  )
  add_global_link_arguments(
    _compiler.get_supported_link_arguments(global_link_args),
    language: _language,
  )
endforeach

# Dependencies. {{{

# Iconv replacement…
if host_machine.system() == 'android'
  subproject('miniconv')
endif

# Luajit.
luajit_dep = dependency('luajit')
lua_dep = dependency('lua')

# Libraries.
foreach _dep, _opt : {
  'crengine'        : 'crengine',
  'djvulibre'       : 'djvulibre',
  'freetype2'       : 'freetype',
  'fribidi'         : 'fribidi',
  'giflib'          : 'giflib',
  'harfbuzz'        : 'harfbuzz',
  'k2pdfopt'        : 'k2pdfopt',
  'koreader-lfs'    : '',
  'lept'            : 'leptonica',
  'libczmq'         : 'zeromq',
  'libmupdf'        : 'mupdf',
  'libpopen_noshell': 'koreader_input',
  'libturbojpeg'    : 'turbojpeg',
  'libunibreak'     : 'unibreak',
  'libutf8proc'     : 'utf8proc',
  'libwebpdemux'    : 'webp',
  'libzmq'          : 'zeromq',
  'libzstd'         : '',
  'lodepng'         : 'lodepng',
  'lua-htmlparser'  : 'lua-htmlparser',
  'lua-ljsqlite3'   : 'sqlite3',
  'lua-rapidjson'   : 'lua-rapidjson',
  'lua-spore'       : 'lua-spore',
  'lua-turbo'       : 'lua-turbo',
  'luajson'         : 'luajson',
  'luasec'          : 'luasec',
  'luasocket'       : 'luasocket',
  'nanosvg'         : 'nanosvg',
  'sdl2'            : 'sdl',
  'sqlite3'         : 'sqlite3',
  'threads'         : '',
  'zlib'            : '',
}
  set_variable(_dep.underscorify() + '_dep', dependency(_dep, required: _opt == '' or get_option(_opt) or disabler()))
endforeach

# }}}

# Helpers. {{{

_helpers = {
  'blitbuffer': {
    '_loader': ['ffi', 'blitbuffer'],
    'dependencies': [get_option('blitbuffer') ? declare_dependency() : disabler()],
    'sources': ['base/blitbuffer.c'],
  },
  'koreader-cre': {
    '_loader': ['module', 'libs/libkoreader-cre', 'luaopen_cre'],
    'dependencies': [crengine_dep, lua_dep],
    'sources': ['base/cre.cpp'],
  },
  'koreader-djvu': {
    '_loader': ['module', 'libs/libkoreader-djvu', 'luaopen_djvu'],
    'dependencies': [djvulibre_dep, k2pdfopt_dep, lua_dep, threads_dep],
    'sources': ['base/djvu.c'],
  },
  'koreader-input': {
    '_loader': ['module', 'libs/libkoreader-input', 'luaopen_input'],
    'c_args': '-D' + get_option('target').to_upper(),
    'dependencies': [lua_dep, libpopen_noshell_dep],
    'include_directories': include_directories('base/input'),
    'sources': 'base/input/input.c',
  },
  'koreader-nnsvg': {
    '_loader': ['module', 'libs/libkoreader-nnsvg', 'luaopen_nnsvg'],
    'dependencies': [lua_dep, nanosvg_dep],
    'sources': ['base/nnsvg.c'],
  },
  'koreader-xtext': {
    '_loader': ['module', 'libs/libkoreader-xtext', 'luaopen_xtext'],
    'dependencies': [
      freetype2_dep,
      fribidi_dep,
      harfbuzz_dep,
      lua_dep,
      libunibreak_dep,
    ],
    'sources': ['base/xtext.cpp'],
  },
  'wrap-mupdf': {
    '_loader': ['ffi', 'wrap-mupdf'],
    'dependencies': [libmupdf_dep],
    'sources': ['base/wrap-mupdf.c'],
  },
}

foreach _name, _params : _helpers
  _subdeps = _params.get('dependencies', [])
  if not declare_dependency(dependencies: _subdeps).found()
    # Disabled, skip it.
    continue
  endif
  _lib = build_target(
    _name,
    _params['sources'],
    c_args: _params.get('c_args', []),
    gnu_symbol_visibility: 'inlineshidden',
    dependencies: _params.get('dependencies', []),
    include_directories: _params.get('include_directories', []),
    install: true,
    install_dir: get_option('libdir'),
    target_type: opt_amalgam ? 'static_library' : 'shared_module',
  )
  _helpers += {_name: {'_lib': _lib} + _params}
endforeach

_deps = []
_exports = []
_libs = []
_loaders = []

# LUA modules {{{.

_deps += dependency('koreader-lfs')
_exports += 'koreader-lfs'
_loaders += [['module', 'libs/libkoreader-lfs', 'luaopen_lfs']]

if lua_turbo_dep.found()
  _deps += lua_turbo_dep
  _exports += 'lua-turbo'
  _loaders += [['ffi', 'tffi_wrap']]
endif

foreach _spec : [
  # Dependency             Exports          LUA modules
  [dependency('lua-lpeg'), 'lpeg'         , ['lpeg']],
  [luasocket_dep         , 'luasocket'    , ['mime.mcore', 'socket.score']],
  [lua_rapidjson_dep     , 'lua-rapidjson', ['rapidjson']],
  [luasec_dep            , 'luasec'       , ['ssl.config', 'ssl.context', 'ssl.core', 'ssl.x509']],
]
  if not _spec[0].found()
    # Disabled, skip it.
    continue
  endif
  _deps += _spec[0]
  _exports += _spec[1]
  foreach _mod : _spec[2]
    _fn = 'luaopen_' + _mod.underscorify()
    _loaders += [['module', _mod, _fn]]
  endforeach
endforeach

# }}}

foreach _spec : [
  # FFI library       Exports        Dependency
  ['czmq.1'         , 'zeromq'     , libczmq_dep     ],
  ['freetype.6'     , 'freetype2'  , freetype2_dep   ],
  ['gif.7'          , 'giflib'     , giflib_dep      ],
  ['harfbuzz.0'     , 'harfbuzz'   , harfbuzz_dep    ],
  ['k2pdfopt.2'     , 'koptcontext', k2pdfopt_dep    ],
  ['lept.6'         , 'leptonica'  , lept_dep        ],
  ['lodepng'        , 'lodepng'    , lodepng_dep     ],
  ['mupdf'          , 'mupdf'      , libmupdf_dep    ],
  ['SDL2-2.0.0'     , 'SDL2_0'     , sdl2_dep        ],
  ['sqlite3.0'      , 'sqlite3'    , sqlite3_dep     ],
  ['turbojpeg.0.3.0', 'turbojpeg'  , libturbojpeg_dep],
  ['utf8proc.2'     , 'utf8proc'   , libutf8proc_dep ],
  ['webpdemux.2'    , 'libwebp'    , libwebpdemux_dep],
  ['z'              , 'zlib'       , zlib_dep        ],
  ['zmq.4'          , 'zeromq'     , libzmq_dep      ],
  ['zstd.1'         , 'zstd'       , libzstd_dep     ],
]
  if not _spec[2].found()
    # Disabled, skip it.
    continue
  endif
  _deps += _spec[2]
  _exports += _spec[1]
  _loaders += [['ffi', _spec[0]]]
endforeach

foreach _name, _params : _helpers
  if not _params.has_key('_lib')
    # Disabled, skip it.
    continue
  endif
  _loaders += [_params['_loader']]
  _libs += _params['_lib']
  _exports += _name
endforeach

if opt_amalgam

  _link_scripts_inputs = []
  foreach _e : _exports
    _cdecl = f'base/ffi-cdecl/@_e@_cdecl.c'
    if not fs.exists(_cdecl)
      _cdecl = f'base/ffi-cdecl/@_e@_decl.c'
    endif
    _link_scripts_inputs += _cdecl
  endforeach

  _link_args = [
    '-Wl', '@' + (meson.current_build_dir() / 'libkoreader.link_args'),
  ]

  _link_scripts = custom_target(
    'koreader_link_scripts',
    input: _link_scripts_inputs,
    output: [
      'libkoreader.link_args',
      'libkoreader.version_script',
    ],
    command: [
      find_program('./base/libkoreader.gen_link_files.sh'),
      cc.get_linker_id(),
      '@OUTPUT@',
      '@INPUT@',
    ],
  )

  # TODO: check for missing exports: e.g. local symbols.
  # comm -23 \
  #   <(sed -ne 's/^--require-defined=//p' libkoreader.link_args) \
  #   <(nm --format=posix --dynamic libkoreader.so | sed -ne 's/^\(\w\+\)\(@@\w\+\)\? [CDGTR] .*$/\1/p')
  _libkoreader = shared_library(
    'koreader',
    dependencies: _deps,
    gnu_symbol_visibility: 'inlineshidden',
    install: true,
    install_dir: get_option('libdir'),
    link_args: ','.join(_link_args),
    link_depends: _link_scripts[0],
    link_language: 'cpp',
    link_whole: _libs,
    override_options: 'b_lundef=false',
  )

  _lua_modules = []
  _ffi_libraries = []

  foreach _l : _loaders
    if _l[0] == 'module'
      _lua_modules += '["' + _l[1] + '"] = "' + _l[2] + '",'
    elif _l[0] == 'ffi'
      _ffi_libraries += '["' + _l[1] + '"] = true,'
    else
      error(_l[0])
    endif
  endforeach

  _libdir = get_option('libdir')
  if _libdir.endswith('.staging')
    _libdir = _libdir.substring(0, -8)
  endif
  configure_file(
    input: 'base/libkoreader.meson.lua',
    output: 'libkoreader.lua',
    install_dir: '.',
    install_tag: 'runtime',
    configuration: {
      'library': _libdir / fs.name(_libkoreader.full_path()),
      'lua_modules': '\n    '.join(_lua_modules),
      'ffi_libraries': '\n    '.join(_ffi_libraries),
    },
  )

endif

# }}}

# Launcher. {{{

if get_option('launcher') == 'default'
  toinstall += {
    'koreader': 'base/launcher.sh',
  }
elif get_option('launcher') == 'luajit-launcher'
  subproject('luajit-launcher')
elif get_option('launcher') == 'macos'
  executable(
    'koreader',
    'base/osx_loader.c',
    dependencies: [luajit_dep],
    # FIXME: does not seem to work; when statically linking
    # to the luajit library loading a LUA module fail with
    # missing symbols…
    export_dynamic: true,
    install: true,
  )
elif get_option('launcher') == 'remarkable'
  executable('button-listen', 'base/button-listen.c', install: true)
endif

# }}}

# Subprojects.

if get_option(
  'ffi-cdecl',
).require(
  not meson.is_cross_build(),
  error_message: 'only supported for native builds',
).require(
  meson.get_compiler('c').get_id() == 'gcc',
  error_message: 'only support the GCC compiler',
).allowed()
  subproject('gcc-lua-cdecl')
endif

# Extra configurable subprojects.
foreach _prj : get_option('subprojects')
  subproject(_prj)
endforeach

# }}}.

# Install. {{{

# Update once marker.
configure_file(
  input: 'meson/blank.in',
  output: 'update_once.marker',
  install: true,
  install_dir: '.',
  install_tag: 'runtime',
  configuration: {
    'CONTENTS': '# This file indicates that update once patches have not been applied yet.',
  },
)

# Platorm pre-built binaries.
foreach _exe : get_option('platform_executables')
  install_data(_exe, install_dir: get_option('bindir'))
endforeach

# Development stuff.
install_headers(
  'base/blitbuffer.h',
  'base/cre.h',
  'base/djvu.h',
  'base/drawcontext.h',
  'base/koptcontext.h',
  'base/nnsvg.h',
  'base/wrap-mupdf.h',
  'base/xtext.h',
  subdir: 'koreader-base',
)
pkg = import('pkgconfig')
pkg.generate(
  description: 'Base framework for creating document readers like KOReader',
  name: 'koreader-base',
  extra_cflags: '-I${basedir}',
  uninstalled_variables: {
    'basedir': '${srcdir}/base',
  },
  variables: {
    'basedir': '${includedir}/koreader-base',
  },
)

# Toolchain libraries.
if meson.is_cross_build()
  toolchain_libs = []
  toolchain_arg = ''
  if host_machine.system() == 'android'
    toolchain_libs += ['libc++_shared.so']
    if get_option('b_sanitize') == 'address'
      toolchain_libs += [
        'libclang_rt.asan-' + host_machine.cpu_family() + '-android.so',
      ]
    endif
    toolchain_arg = '--print-file-name'
  elif host_machine.system() == 'linux'
    toolchain_libs += ['libstdc++.so.6']
    toolchain_arg = '--print-file-name'
  elif host_machine.system() == 'windows' and cc.get_id() == 'gcc'
    toolchain_libs += [
      'libgcc_s_seh-1.dll',
      'libstdc++-6.dll',
      'libwinpthread-1.dll',
    ]
    toolchain_arg = '--print-prog-name'
  endif
  foreach _lib : toolchain_libs
    _path = run_command([cc, toolchain_arg, _lib], check: true).stdout().strip()
    if get_option('strip')
      custom_target(
        _lib,
        command: [
          find_program('strip'),
          '-o', '@OUTPUT@',
          '@INPUT@',
        ],
        input: _path,
        output: _lib,
        install: true,
        install_dir: get_option('libdir'),
      )
    else
      fs.copyfile(
        _path,
        install: true,
        install_dir: get_option('libdir'),
      )
    endif
  endforeach
endif

# Empty directories.
toinstall += {
  'cache'      : '',
  'clipboard'  : '',
  'history'    : '',
  'ota'        : '',
  'screenshots': '',
}

# Directories.
toinstall += {
  'ffi'                 : 'base/ffi',
  'fonts'               : 'resources/fonts',
  'frontend'            : 'frontend',
  'l10n'                : 'l10n',
  'plugins'             : 'plugins',
  'resources/icons'     : 'resources/icons',
  'resources/quickstart': 'resources/quickstart',
  'tools'               : 'tools',
}

# Files.
toinstall += {
  'COPYING'               : 'COPYING',
  'README.md'             : 'README.md',
  'datastorage.lua'       : 'datastorage.lua',
  'defaults.lua'          : 'defaults.lua',
  'reader.lua'            : 'reader.lua',
  'resources/koreader.png': 'resources/koreader.png',
  'resources/koreader.svg': 'resources/koreader.svg',
  'setupkoenv.lua'        : 'setupkoenv.lua',
}

# Tests data.
if get_option('tests')
  subproject('koreader-runtests')
  subproject('tessdata')
  toinstall += {
    '.luacov'             : '.luacov',
    'spec/base'           : '',
    'spec/base/unit'      : 'base/spec/unit',
    'spec/front'          : '',
    'spec/front/unit'     : 'spec/unit',
  }
endif

foreach _dst, _src : toinstall
  if _src == ''
    install_emptydir(_dst, install_tag: 'runtime')
  elif get_option('install_symlinks')
    install_symlink(
      fs.name(_dst),
      install_dir: fs.parent(_dst),
      install_tag: 'runtime',
      pointing_to: meson.project_source_root() / _src,
    )
  elif fs.is_dir(_src)
    install_subdir(
      _src,
      install_dir: _dst,
      install_tag: 'runtime',
      strip_directory: true,
    )
  else
    install_data(
      _src,
      install_dir: fs.parent(_dst),
      install_tag: 'runtime',
      rename: fs.name(_dst),
    )
  endif
endforeach

foreach _tag : ['devel', 'runtime']
  meson.add_install_script(
    [
      find_program('./base/fix_libs.py'),
      host_machine.system(),
      get_option('libdir'),
      _tag,
    ],
    install_tag: _tag,
  )
endforeach

# }}}

# vim: foldmethod=marker
