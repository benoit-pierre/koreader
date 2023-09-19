project('koreader-testrunner', version: '0.1')

kodir = get_option('prefix')
testdir = 'spec'

fs = import('fs')

find = find_program('find')
luajit = find_program(kodir / 'luajit')

is_parallel = {
  'base/blitbuffer'           : true,
  'base/common'               : true,
  'base/font'                 : true,
  'base/framebuffer'          : true,
  'base/freetype'             : true,
  'base/koptcontext'          : true,
  'base/md5'                  : true,
  'base/mupdf'                : true,
  'base/nnsvg'                : true,
  'base/pic'                  : true,
  'base/png'                  : true,
  'base/qrencode'             : true,
  'base/spore'                : true,
  'base/util'                 : true,
  'front/background_runner'   : true,
  'front/dbg'                 : true,
  'front/docsettings'         : true,
  'front/document'            : true,
  'front/exporter_plugin_main': true,
  'front/filemanager'         : true,
  'front/frontlight'          : true,
  'front/gettext'             : true,
  'front/koptinterface'       : true,
  'front/luadata'             : true,
  'front/luasettings'         : true,
  'front/opds'                : true,
  'front/optmath'             : true,
  'front/persist'             : true,
  'front/random'              : true,
  'front/readerbookmark'      : true,
  'front/readerdictionary'    : true,
  'front/readerfooter'        : true,
  'front/readerhighlight'     : true,
  'front/readerlink'          : true,
  'front/readerpaging'        : true,
  'front/readerrolling'       : true,
  'front/readertoc'           : true,
  'front/readerview'          : true,
  'front/readhistory'         : true,
  'front/screenshoter'        : true,
  'front/size'                : true,
  'front/spore'               : true,
  'front/translator'          : true,
  'ftron/background_runner'   : true,
}

is_xfail = {}
# if host_machine.system() == 'darwin'
#   is_xfail += {
#     'front/koptinterface'  : true,
#     'front/readerhighlight': true,
#   }
# endif

_testroot = kodir / testdir
foreach _testpath : run_command(
  find, '-L', _testroot,
  '-type', 'f',
  '(',
  '-name', '*_bench.lua',
  '-o',
  '-name', '*_spec.lua',
  ')',
  '-print0',
  check: true,
).stdout().strip('\0').split('\0')
  assert(_testpath.startswith(_testroot))
  # Fugly, but no `str.length()`…
  _testpath = _testpath.replace(_testroot + '/', '')
  _parts = _testpath.split('/')
  _suite = _parts[0]
  assert(_suite != '')
  _is_bench = _parts[-1].endswith('_bench.lua')
  if _is_bench
    _test = _parts[-1].substring(0, -10)
    # message('benchmark:', _suite, _test)
  else
    _test = _parts[-1].substring(0, -9)
    _id = _suite / _test
    _is_parallel = is_parallel.get(_id, true)
    _is_xfail = is_xfail.get(_id, false)
    # message('test:', _suite, _test, _is_parallel, _is_xfail)
    # if not _is_parallel
    #   continue
    # endif
  endif
  _args = [
    '-e', 'require "busted.runner" {standalone = false}', '/dev/null',
    '--output=gtest', '-Xoutput=--color', '--exclude-tags=notest',
    '--run=' + _suite, testdir / _testpath,
  ]
  _env = environment()
  _env.set('KO_HOME', meson.current_build_dir() / '_'.join(_suite, _test))
  # Don't fail the testsuite on ASAN detected leaks.
  _env.prepend('LSAN_OPTIONS', 'exitcode=0', separator: ' ')
  if _is_bench
    benchmark(
      _test,
      luajit,
      args: _args,
      env: _env,
      protocol: 'exitcode',
      suite: _suite,
      timeout: 120,
      workdir: fs.parent(luajit.full_path()),
    )
  else
    test(
      _test,
      luajit,
      args: _args,
      env: _env,
      is_parallel: _is_parallel,
      protocol: 'exitcode',
      should_fail: _is_xfail,
      suite: _suite,
      timeout: 60,
      workdir: fs.parent(luajit.full_path()),
    )
  endif
endforeach
