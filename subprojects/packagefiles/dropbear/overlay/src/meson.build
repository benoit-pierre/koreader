c_args = [
  '-D_GNU_SOURCE',
  '-DDROPBEAR_SERVER',
  '-DLOCALOPTIONS_H_EXISTS',
]
link_args = []

foreach _arg: [
  '-fno-strict-overflow',
  ]
  if cc.has_argument(_arg)
    c_args += _arg
  endif
endforeach

if get_option('harden')

  if get_option('optimization') not in ['plain', '0']
    c_args += '-D_FORTIFY_SOURCE=2'
  endif

  foreach _arg : [
    '-fstack-protector-strong',
    '-fstack-protector',
    '--param=ssp-buffer-size=4',
    '-mfunction-return=thunk',
    '-mindirect-branch=thunk',
  ]
    if cc.has_argument(_arg)
      c_args += _arg
    endif
  endforeach

  foreach _arg : [
    '-Wl,-z,now',
    '-Wl,-z,relro',
  ]
    if cc.has_link_argument(_arg)
      link_args += _arg
    endif
  endforeach

endif

executable(
  'dropbear',
  c_args: c_args,
  dependencies: [
    bsd_dep,
    crypt_dep,
    tomcrypt_dep,
    util_dep,
    zlib_dep,
  ],
  install: true,
  link_args: link_args,
  sources: [
    config_h,
    default_options_guard_h,
    'atomicio.c',
    'bignum.c',
    'buffer.c',
    'chachapoly.c',
    'circbuffer.c',
    'common-algo.c',
    'common-channel.c',
    'common-chansession.c',
    'common-kex.c',
    'common-runopts.c',
    'common-session.c',
    'compat.c',
    'crypto_desc.c',
    'curve25519.c',
    'dbhelpers.c',
    'dbmalloc.c',
    'dbrandom.c',
    'dbutil.c',
    'dh_groups.c',
    'dss.c',
    'ecc.c',
    'ecdsa.c',
    'ed25519.c',
    'fake-rfc2553.c',
    'gcm.c',
    'gendss.c',
    'gened25519.c',
    'genrsa.c',
    'gensignkey.c',
    'list.c',
    'listener.c',
    'loginrec.c',
    'ltc_prng.c',
    'netio.c',
    'packet.c',
    'process-packet.c',
    'queue.c',
    'rsa.c',
    'signkey.c',
    'sk-ecdsa.c',
    'sk-ed25519.c',
    'sshpty.c',
    'svr-agentfwd.c',
    'svr-auth.c',
    'svr-authpam.c',
    'svr-authpasswd.c',
    'svr-authpubkey.c',
    'svr-authpubkeyoptions.c',
    'svr-chansession.c',
    'svr-kex.c',
    'svr-main.c',
    'svr-runopts.c',
    'svr-service.c',
    'svr-session.c',
    'svr-tcpfwd.c',
    'svr-x11fwd.c',
    'tcp-accept.c',
    'termcodes.c',
  ],
)

if get_option('scp')
  executable(
    'scp',
    c_args: c_args + '-DPROGRESS_METER',
    dependencies: [
      tomcrypt_dep.partial_dependency(compile_args: true, includes: true),
      zlib_dep,
    ],
    # include_directories: include_directories(
    #   'libtomcrypt/src/headers',
    # ),
    install: true,
    sources: [
      config_h,
      default_options_guard_h,
      'atomicio.c',
      'compat.c',
      'progressmeter.c',
      'scp.c',
      'scpmisc.c',
    ],
  )
endif
