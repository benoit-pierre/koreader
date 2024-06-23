project(
  'libzmq',
  'cpp',
  version: '4.0.0',
  meson_version: '>= 0.60.0',
)

cxx = meson.get_compiler('cpp')

threads_dep = dependency('threads')
rt_dep = cxx.find_library('rt', required: false)

compile_args = []
deps = [threads_dep, rt_dep]
internal_compile_args = ['-D_REENTRANT', '-D_THREAD_SAFE']

if get_option('default_library') == 'static'
  compile_args += '-DZMQ_STATIC'
endif

cdata = configuration_data()

poller = get_option('poller')
if poller == 'auto'
  if cxx.has_header('sys/event.h') and cxx.has_function('kqueue')
    poller = 'kqueue'
  elif cxx.has_header('sys/epoll.h') and cxx.has_function('epoll_create')
    poller = 'epoll'
  elif (
    cxx.has_header('sys/devpoll.h')
    and cxx.has_type('epoll_create', prefix: '#include<sys/devpoll.h>')
  )
    poller = 'devpoll'
  elif cxx.has_header('poll.h') and cxx.has_function('poll')
    poller = 'poll'
  elif (
    (host_machine.system() == 'windows' and cxx.has_header('winsock2.h'))
    or (cxx.has_header('sys/select.h')
    and cxx.has_function('select'))
  )
    poller = 'select'
  else
    error('no supported poller detected')
  endif
  cdata.set('ZMQ_USE_' + poller.to_upper(), 1)
endif

foreach _header : [
  'windows.h',
  'sys/uio.h',
]
  if cxx.has_header(_header)
    cdata.set(
      'ZMQ_HAVE_' + _header.replace('sys/', '').replace('.h', '').to_upper(),
      1,
    )
  endif
endforeach

if cxx.has_header('ifaddrs.h') and cxx.has_function('getifaddrs') and cxx.has_function('freeifaddrs')
  cdata.set('ZMQ_HAVE_IFADDRS', 1)
endif

if cxx.has_header('sys/eventfd.h', required: get_option('eventfd'))
  cdata.set('ZMQ_HAVE_EVENTFD', 1)
endif

if host_machine.system() == 'windows'
  ws2_dep = cxx.find_library('ws2_32', required: false)
  if not ws2_dep.found()
    ws2_dep = cxx.find_library('ws2', required: false)
  endif
  if not ws2_dep.found()
    error('C++ shared or static library \'ws2_32\' or \'ws2\' not found')
  endif
  rpcrt4_dep = cxx.find_library('rpcrt4')
  iphlpapi_dep = cxx.find_library('iphlpapi')
  deps += [ws2_dep, rpcrt4_dep, iphlpapi_dep]
  if get_option('default_library') != 'static'
    compile_args += '-DDLL_EXPORT'
  endif
endif

if host_machine.system() != 'windows' and cxx.has_header('sys/socket.h')
  foreach _def : [
    'LOCAL_PEERCRED',
    'SO_PEERCRED',
  ]
    if cxx.get_define(_def, prefix: '#include<sys/socket.h>') != ''
      cdata.set('ZMQ_HAVE_' + _def, 1)
    endif
  endforeach
endif

internal_compile_args += {
  'linux': '-D_GNU_SOURCE',
  'freebsd': '-D__BSD_VISIBLE',
  'netbsd': '-D_NETBSD_SOURCE',
  'openbsd': '-D_OPENBSD_SOURCE',
  'sunos': '-D_PTHREADS',
  'darwin': '-D_DARWIN_C_SOURCE',
}.get(host_machine.system(), [])

if cxx.has_function('clock_gettime', dependencies: rt_dep)
  cdata.set('HAVE_CLOCK_GETTIME', 1)
endif

foreach _function, _header : {
  'fork': 'unistd.h',
  'gethrtime': 'sys/time.h',
}
  if cxx.has_header(_header) and cxx.has_function(_function)
    cdata.set('HAVE_' + _function.underscorify().to_upper(), 1)
  endif
endforeach

if (
  host_machine.system() in ['netbsd', 'sunos']
  and not cxx.compiles(
    '''
    #include <atomic.h>
    int main()
    {
      uint32_t value;
      atomic_cas_32(&value, 0, 0);
      return 0;
    }
    ''',
  )
)
  cdata.set('ZMQ_FORCE_MUTEXES', 1)
endif

if meson.can_run_host_binaries()
  foreach _name, _test : {
    'have SOCK_CLOEXEC':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    int main(int argc, char *argv [])
    {
        int s = socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
        return(s == -1);
    }
    ''',
    'have SO_KEEPALIVE':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    int main(int argc, char *argv [])
    {
        int s, rc, opt = 1;
        return(
           ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) ||
           ((rc = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*) &opt, sizeof(int))) == -1)
        );
    }
    ''',
    'have TCP_KEEPCNT':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    int main(int argc, char *argv [])
    {
        int s, rc, opt = 1;
        return(
           ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) ||
           ((rc = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*) &opt, sizeof(int))) == -1) ||
           ((rc = setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, (char*) &opt, sizeof(int))) == -1)
        );
    }
    ''',
    'have tcp keepidle':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    int main(int argc, char *argv [])
    {
        int s, rc, opt = 1;
        return(
           ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) ||
           ((rc = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*) &opt, sizeof(int))) == -1) ||
           ((rc = setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, (char*) &opt, sizeof(int))) == -1)
        );
    }
    ''',
    'have TCP_KEEPINTVL':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    int main(int argc, char *argv [])
    {
        int s, rc, opt = 1;
        return(
           ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) ||
           ((rc = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*) &opt, sizeof(int))) == -1) ||
           ((rc = setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, (char*) &opt, sizeof(int))) == -1)
        );
    }
    ''',
    'have TCP_KEEPALIVE':
    '''
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    int main(int argc, char *argv [])
    {
        int s, rc, opt = 1;
        return(
           ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) ||
           ((rc = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*) &opt, sizeof(int))) == -1) ||
           ((rc = setsockopt(s, IPPROTO_TCP, TCP_KEEPALIVE, (char*) &opt, sizeof(int))) == -1)
        );
    }
    ''',
  }
    _res = cxx.run(_test, name: _name)
    if _res.compiled() and _res.returncode() == 0
      cdata.set('ZMQ_' + _name.underscorify().to_upper(), 1)
    endif
  endforeach
endif

platform_hpp = configure_file(
  input: 'builds/cmake/platform.hpp.in',
  output: 'platform.hpp',
  format: 'cmake@',
  configuration: cdata,
)

libzmq_lib = library(
  'zmq',
  [
    platform_hpp,
    'src/address.cpp',
    'src/clock.cpp',
    'src/ctx.cpp',
    'src/curve_client.cpp',
    'src/curve_server.cpp',
    'src/dealer.cpp',
    'src/devpoll.cpp',
    'src/dist.cpp',
    'src/epoll.cpp',
    'src/err.cpp',
    'src/fq.cpp',
    'src/io_object.cpp',
    'src/io_thread.cpp',
    'src/ip.cpp',
    'src/ipc_address.cpp',
    'src/ipc_connecter.cpp',
    'src/ipc_listener.cpp',
    'src/kqueue.cpp',
    'src/lb.cpp',
    'src/mailbox.cpp',
    'src/mechanism.cpp',
    'src/metadata.cpp',
    'src/msg.cpp',
    'src/mtrie.cpp',
    'src/null_mechanism.cpp',
    'src/object.cpp',
    'src/options.cpp',
    'src/own.cpp',
    'src/pair.cpp',
    'src/pgm_receiver.cpp',
    'src/pgm_sender.cpp',
    'src/pgm_socket.cpp',
    'src/pipe.cpp',
    'src/plain_client.cpp',
    'src/plain_server.cpp',
    'src/poll.cpp',
    'src/poller_base.cpp',
    'src/precompiled.cpp',
    'src/proxy.cpp',
    'src/pub.cpp',
    'src/pull.cpp',
    'src/push.cpp',
    'src/random.cpp',
    'src/raw_decoder.cpp',
    'src/raw_encoder.cpp',
    'src/reaper.cpp',
    'src/rep.cpp',
    'src/req.cpp',
    'src/router.cpp',
    'src/select.cpp',
    'src/session_base.cpp',
    'src/signaler.cpp',
    'src/socket_base.cpp',
    'src/stream.cpp',
    'src/stream_engine.cpp',
    'src/sub.cpp',
    'src/tcp.cpp',
    'src/tcp_address.cpp',
    'src/tcp_connecter.cpp',
    'src/tcp_listener.cpp',
    'src/thread.cpp',
    'src/trie.cpp',
    'src/v1_decoder.cpp',
    'src/v1_encoder.cpp',
    'src/v2_decoder.cpp',
    'src/v2_encoder.cpp',
    'src/xpub.cpp',
    'src/xsub.cpp',
    'src/zmq.cpp',
    'src/zmq_utils.cpp',
  ],
  cpp_args: compile_args + internal_compile_args,
  dependencies: deps,
  gnu_symbol_visibility: 'inlineshidden',
  install: true,
  soversion: 4,
)

libzmq_dep = declare_dependency(
  compile_args: compile_args,
  include_directories: include_directories('include'),
  link_with: libzmq_lib,
)

meson.override_dependency('libzmq', libzmq_dep)

install_headers('include/zmq.h', 'include/zmq_utils.h')

pkg = import('pkgconfig')
pkg.generate(libzmq_lib, name: 'libzmq')
