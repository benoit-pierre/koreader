project('dropbear', 'c', version: '2022.83')

cc = meson.get_compiler('c')

bsd_dep = cc.find_library('bsd', required: false)
crypt_dep = cc.find_library('crypt', required: false)
util_dep = cc.find_library('util', required: false)
zlib_dep = dependency('zlib', required: get_option('zlib'))

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

  c_args += '-D_FORTIFY_SOURCE=2'

  foreach _arg: [
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

  foreach _arg: [
    '-Wl,-z,now',
    '-Wl,-z,relro',
    ]
    if cc.has_link_argument(_arg)
      link_args += _arg
    endif
  endforeach

endif

cdata = configuration_data()

cdata.set('BUNDLED_LIBTOM', 1)
cdata.set('DISABLE_LASTLOG', 1)
cdata.set('DISABLE_PAM', 1)

if not zlib_dep.found()
  cdata.set('DISABLE_ZLIB', 1)
endif

default_includes = []

foreach _header: [
  'crypt.h', 'inttypes.h', 'lastlog.h', 'libgen.h', 'libutil.h',
  'linux/pkt_sched.h', 'netdb.h', 'netinet/in.h', 'netinet/in_systm.h',
  'netinet/tcp.h', 'pam/pam_appl.h', 'paths.h', 'pty.h', 'security/pam_appl.h',
  'shadow.h', 'stddef.h', 'stdint.h', 'stdio.h', 'stdlib.h', 'string.h',
  'strings.h', 'stropts.h', 'sys/prctl.h', 'sys/random.h', 'sys/socket.h',
  'sys/stat.h', 'sys/types.h', 'sys/uio.h', 'unistd.h', 'util.h', 'utmp.h',
  'utmpx.h',
  ]
  if cc.has_header(_header)
    cdata.set('HAVE_' + _header.underscorify().to_upper(), 1)
    default_includes += f'#include<@_header@>'
  endif
endforeach

default_includes = '\n'.join(default_includes)

if cc.has_function('crypt', dependencies: crypt_dep)
  cdata.set('HAVE_CRYPT', 1)
endif

if cc.has_function('login', dependencies: [bsd_dep, util_dep])
  cdata.set('HAVE_LOGIN', 1)
endif

if cc.has_function('openpty', dependencies: [util_dep])
  cdata.set('HAVE_OPENPTY', 1)
endif

foreach _function: [
  '_getpty', 'basename', 'clearenv', 'clock_gettime', 'daemon', 'endutent',
  'endutxent', 'explicit_bzero', 'fexecve', 'fork', 'freeaddrinfo',
  'gai_strerror', 'getaddrinfo', 'getgrouplist', 'getnameinfo', 'getrandom',
  'getusershell', 'logout', 'logwtmp', 'memset_s', 'pututline',
  'pututxline', 'setutent', 'setutxent', 'strlcat', 'strlcpy', 'writev',
  ]
  if cc.has_function(_function)
    cdata.set('HAVE_' + _function.underscorify().to_upper(), 1)
  endif
endforeach

if cdata.get('HAVE_GAI_STRERROR', 1) == 1 and cc.compiles(
  '''
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>

  const char *gai_strerror(int);

  int main(void) {
    char *str;

    str = gai_strerror(0);
    return 0;
  }
  '''
  )
  cdata.set('HAVE_CONST_GAI_STRERROR_PROTO', 1)
endif

foreach _type: [
  'uint8_t', 'u_int8_t', 'uint16_t', 'u_int16_t', 'uint32_t', 'u_int32_t',
  ]
  if cc.has_type(_type, prefix: default_includes)
    cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
  endif
endforeach

foreach _type: [
  'struct addrinfo',
  'struct in6_addr',
  'struct sockaddr_in6',
  'struct sockaddr_storage',
  ]
  if cc.sizeof(_type, prefix: default_includes) > 0
    cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
  endif
endforeach

foreach _struct, _member_list: {
  'utmp': [
    'ut_host', 'ut_pid', 'ut_type', 'ut_tv', 'ut_id', 'ut_addr', 'ut_addr_v6',
    'ut_exit', 'ut_time',
    ],
  'utmpx': [
    'ut_host', 'ut_syslen', 'ut_type', 'ut_id', 'ut_addr', 'ut_addr_v6',
    'ut_time', 'ut_tv',
    ],
  'sockaddr_storage': ['ss_family'],
  }
  foreach _member: _member_list
    if cc.compiles('''
    @0@

    int main(void) {
      char t[sizeof (((struct @1@ *)0)->@2@) >= 0 ? 1 : -1];
      return 0;
      }
    '''.format(default_includes, _struct, _member),
    name: f'has `struct @_struct@.@_member@` member',
    )
      cdata.set('HAVE_STRUCT_' + (_struct + '_' + _member).underscorify().to_upper(), 1)
    endif
  endforeach
endforeach

config_h = configure_file(
  output: 'config.h',
  configuration: cdata,
  )

cdata = configuration_data()

dbscp_path = get_option('dbscp_path')
if dbscp_path != ''
  cdata.set_quoted('DBSCP_PATH', dbscp_path)
endif

cdata.set_quoted('DROPBEAR_DEFPORT', get_option('default_port').to_string())
cdata.set10('INETD_MODE', get_option('inetd_mode'))

sftpserver_path = get_option('sftpserver_path')
if sftpserver_path != ''
  cdata.set_quoted('SFTPSERVER_PATH', sftpserver_path)
endif

default_options_h = configure_file(
  configuration: cdata,
  input: 'localoptions.meson.h',
  output: 'localoptions.h',
  )

default_options_guard_h = custom_target(
  'default_options_guard_h',
  capture: true, feed: true,
  input: 'default_options.h',
  output: 'default_options_guard.h',
  command: [meson.project_source_root() / 'ifndef_wrapper.sh'],
  )

tomcrypt_src = []
foreach _file: [
  'ciphers/aes/aes.c', 'ciphers/anubis.c', 'ciphers/blowfish.c',
  'ciphers/camellia.c', 'ciphers/cast5.c', 'ciphers/des.c', 'ciphers/kasumi.c',
  'ciphers/khazad.c', 'ciphers/kseed.c', 'ciphers/multi2.c',
  'ciphers/noekeon.c', 'ciphers/rc2.c', 'ciphers/rc5.c', 'ciphers/rc6.c',
  'ciphers/safer/safer.c', 'ciphers/safer/saferp.c', 'ciphers/skipjack.c',
  'ciphers/twofish/twofish.c', 'ciphers/xtea.c', 'encauth/ccm/ccm_add_aad.c',
  'encauth/ccm/ccm_add_nonce.c', 'encauth/ccm/ccm_done.c',
  'encauth/ccm/ccm_init.c', 'encauth/ccm/ccm_memory.c',
  'encauth/ccm/ccm_process.c', 'encauth/ccm/ccm_reset.c',
  'encauth/ccm/ccm_test.c', 'encauth/chachapoly/chacha20poly1305_add_aad.c',
  'encauth/chachapoly/chacha20poly1305_decrypt.c',
  'encauth/chachapoly/chacha20poly1305_done.c',
  'encauth/chachapoly/chacha20poly1305_encrypt.c',
  'encauth/chachapoly/chacha20poly1305_init.c',
  'encauth/chachapoly/chacha20poly1305_memory.c',
  'encauth/chachapoly/chacha20poly1305_setiv.c',
  'encauth/chachapoly/chacha20poly1305_setiv_rfc7905.c',
  'encauth/chachapoly/chacha20poly1305_test.c', 'encauth/eax/eax_addheader.c',
  'encauth/eax/eax_decrypt.c', 'encauth/eax/eax_decrypt_verify_memory.c',
  'encauth/eax/eax_done.c', 'encauth/eax/eax_encrypt.c',
  'encauth/eax/eax_encrypt_authenticate_memory.c', 'encauth/eax/eax_init.c',
  'encauth/eax/eax_test.c', 'encauth/gcm/gcm_add_aad.c',
  'encauth/gcm/gcm_add_iv.c', 'encauth/gcm/gcm_done.c',
  'encauth/gcm/gcm_gf_mult.c', 'encauth/gcm/gcm_init.c',
  'encauth/gcm/gcm_memory.c', 'encauth/gcm/gcm_mult_h.c',
  'encauth/gcm/gcm_process.c', 'encauth/gcm/gcm_reset.c',
  'encauth/gcm/gcm_test.c', 'encauth/ocb/ocb_decrypt.c',
  'encauth/ocb/ocb_decrypt_verify_memory.c', 'encauth/ocb/ocb_done_decrypt.c',
  'encauth/ocb/ocb_done_encrypt.c', 'encauth/ocb/ocb_encrypt.c',
  'encauth/ocb/ocb_encrypt_authenticate_memory.c', 'encauth/ocb/ocb_init.c',
  'encauth/ocb/ocb_ntz.c', 'encauth/ocb/ocb_shift_xor.c',
  'encauth/ocb/ocb_test.c', 'encauth/ocb/s_ocb_done.c',
  'encauth/ocb3/ocb3_add_aad.c', 'encauth/ocb3/ocb3_decrypt.c',
  'encauth/ocb3/ocb3_decrypt_last.c',
  'encauth/ocb3/ocb3_decrypt_verify_memory.c', 'encauth/ocb3/ocb3_done.c',
  'encauth/ocb3/ocb3_encrypt.c',
  'encauth/ocb3/ocb3_encrypt_authenticate_memory.c',
  'encauth/ocb3/ocb3_encrypt_last.c', 'encauth/ocb3/ocb3_init.c',
  'encauth/ocb3/ocb3_int_ntz.c', 'encauth/ocb3/ocb3_int_xor_blocks.c',
  'encauth/ocb3/ocb3_test.c', 'hashes/blake2b.c', 'hashes/blake2s.c',
  'hashes/chc/chc.c', 'hashes/helper/hash_file.c',
  'hashes/helper/hash_filehandle.c', 'hashes/helper/hash_memory.c',
  'hashes/helper/hash_memory_multi.c', 'hashes/md2.c', 'hashes/md4.c',
  'hashes/md5.c', 'hashes/rmd128.c', 'hashes/rmd160.c', 'hashes/rmd256.c',
  'hashes/rmd320.c', 'hashes/sha1.c', 'hashes/sha2/sha224.c',
  'hashes/sha2/sha256.c', 'hashes/sha2/sha384.c', 'hashes/sha2/sha512.c',
  'hashes/sha2/sha512_224.c', 'hashes/sha2/sha512_256.c', 'hashes/sha3.c',
  'hashes/sha3_test.c', 'hashes/tiger.c', 'hashes/whirl/whirl.c',
  'mac/blake2/blake2bmac.c', 'mac/blake2/blake2bmac_file.c',
  'mac/blake2/blake2bmac_memory.c', 'mac/blake2/blake2bmac_memory_multi.c',
  'mac/blake2/blake2bmac_test.c', 'mac/blake2/blake2smac.c',
  'mac/blake2/blake2smac_file.c', 'mac/blake2/blake2smac_memory.c',
  'mac/blake2/blake2smac_memory_multi.c', 'mac/blake2/blake2smac_test.c',
  'mac/f9/f9_done.c', 'mac/f9/f9_file.c', 'mac/f9/f9_init.c',
  'mac/f9/f9_memory.c', 'mac/f9/f9_memory_multi.c', 'mac/f9/f9_process.c',
  'mac/f9/f9_test.c', 'mac/hmac/hmac_done.c', 'mac/hmac/hmac_file.c',
  'mac/hmac/hmac_init.c', 'mac/hmac/hmac_memory.c',
  'mac/hmac/hmac_memory_multi.c', 'mac/hmac/hmac_process.c',
  'mac/hmac/hmac_test.c', 'mac/omac/omac_done.c', 'mac/omac/omac_file.c',
  'mac/omac/omac_init.c', 'mac/omac/omac_memory.c',
  'mac/omac/omac_memory_multi.c', 'mac/omac/omac_process.c',
  'mac/omac/omac_test.c', 'mac/pelican/pelican.c',
  'mac/pelican/pelican_memory.c', 'mac/pelican/pelican_test.c',
  'mac/pmac/pmac_done.c', 'mac/pmac/pmac_file.c', 'mac/pmac/pmac_init.c',
  'mac/pmac/pmac_memory.c', 'mac/pmac/pmac_memory_multi.c',
  'mac/pmac/pmac_ntz.c', 'mac/pmac/pmac_process.c', 'mac/pmac/pmac_shift_xor.c',
  'mac/pmac/pmac_test.c', 'mac/poly1305/poly1305.c',
  'mac/poly1305/poly1305_file.c', 'mac/poly1305/poly1305_memory.c',
  'mac/poly1305/poly1305_memory_multi.c', 'mac/poly1305/poly1305_test.c',
  'mac/xcbc/xcbc_done.c', 'mac/xcbc/xcbc_file.c', 'mac/xcbc/xcbc_init.c',
  'mac/xcbc/xcbc_memory.c', 'mac/xcbc/xcbc_memory_multi.c',
  'mac/xcbc/xcbc_process.c', 'mac/xcbc/xcbc_test.c',
  'math/fp/ltc_ecc_fp_mulmod.c', 'math/gmp_desc.c', 'math/ltm_desc.c',
  'math/multi.c', 'math/radix_to_bin.c', 'math/rand_bn.c', 'math/rand_prime.c',
  'math/tfm_desc.c', 'misc/adler32.c', 'misc/base64/base64_decode.c',
  'misc/base64/base64_encode.c', 'misc/burn_stack.c',
  'misc/compare_testvector.c', 'misc/crc32.c', 'misc/crypt/crypt.c',
  'misc/crypt/crypt_argchk.c', 'misc/crypt/crypt_cipher_descriptor.c',
  'misc/crypt/crypt_cipher_is_valid.c', 'misc/crypt/crypt_constants.c',
  'misc/crypt/crypt_find_cipher.c', 'misc/crypt/crypt_find_cipher_any.c',
  'misc/crypt/crypt_find_cipher_id.c', 'misc/crypt/crypt_find_hash.c',
  'misc/crypt/crypt_find_hash_any.c', 'misc/crypt/crypt_find_hash_id.c',
  'misc/crypt/crypt_find_hash_oid.c', 'misc/crypt/crypt_find_prng.c',
  'misc/crypt/crypt_fsa.c', 'misc/crypt/crypt_hash_descriptor.c',
  'misc/crypt/crypt_hash_is_valid.c', 'misc/crypt/crypt_inits.c',
  'misc/crypt/crypt_ltc_mp_descriptor.c', 'misc/crypt/crypt_prng_descriptor.c',
  'misc/crypt/crypt_prng_is_valid.c', 'misc/crypt/crypt_prng_rng_descriptor.c',
  'misc/crypt/crypt_register_all_ciphers.c',
  'misc/crypt/crypt_register_all_hashes.c',
  'misc/crypt/crypt_register_all_prngs.c', 'misc/crypt/crypt_register_cipher.c',
  'misc/crypt/crypt_register_hash.c', 'misc/crypt/crypt_register_prng.c',
  'misc/crypt/crypt_sizes.c', 'misc/crypt/crypt_unregister_cipher.c',
  'misc/crypt/crypt_unregister_hash.c', 'misc/crypt/crypt_unregister_prng.c',
  'misc/error_to_string.c', 'misc/hkdf/hkdf.c', 'misc/hkdf/hkdf_test.c',
  'misc/mem_neq.c', 'misc/pk_get_oid.c', 'misc/pkcs5/pkcs_5_1.c',
  'misc/pkcs5/pkcs_5_2.c', 'misc/pkcs5/pkcs_5_test.c', 'misc/zeromem.c',
  'modes/cbc/cbc_decrypt.c', 'modes/cbc/cbc_done.c', 'modes/cbc/cbc_encrypt.c',
  'modes/cbc/cbc_getiv.c', 'modes/cbc/cbc_setiv.c', 'modes/cbc/cbc_start.c',
  'modes/cfb/cfb_decrypt.c', 'modes/cfb/cfb_done.c', 'modes/cfb/cfb_encrypt.c',
  'modes/cfb/cfb_getiv.c', 'modes/cfb/cfb_setiv.c', 'modes/cfb/cfb_start.c',
  'modes/ctr/ctr_decrypt.c', 'modes/ctr/ctr_done.c', 'modes/ctr/ctr_encrypt.c',
  'modes/ctr/ctr_getiv.c', 'modes/ctr/ctr_setiv.c', 'modes/ctr/ctr_start.c',
  'modes/ctr/ctr_test.c', 'modes/ecb/ecb_decrypt.c', 'modes/ecb/ecb_done.c',
  'modes/ecb/ecb_encrypt.c', 'modes/ecb/ecb_start.c', 'modes/f8/f8_decrypt.c',
  'modes/f8/f8_done.c', 'modes/f8/f8_encrypt.c', 'modes/f8/f8_getiv.c',
  'modes/f8/f8_setiv.c', 'modes/f8/f8_start.c', 'modes/f8/f8_test_mode.c',
  'modes/lrw/lrw_decrypt.c', 'modes/lrw/lrw_done.c', 'modes/lrw/lrw_encrypt.c',
  'modes/lrw/lrw_getiv.c', 'modes/lrw/lrw_process.c', 'modes/lrw/lrw_setiv.c',
  'modes/lrw/lrw_start.c', 'modes/lrw/lrw_test.c', 'modes/ofb/ofb_decrypt.c',
  'modes/ofb/ofb_done.c', 'modes/ofb/ofb_encrypt.c', 'modes/ofb/ofb_getiv.c',
  'modes/ofb/ofb_setiv.c', 'modes/ofb/ofb_start.c', 'modes/xts/xts_decrypt.c',
  'modes/xts/xts_done.c', 'modes/xts/xts_encrypt.c', 'modes/xts/xts_init.c',
  'modes/xts/xts_mult_x.c', 'modes/xts/xts_test.c',
  'pk/asn1/der/bit/der_decode_bit_string.c',
  'pk/asn1/der/bit/der_decode_raw_bit_string.c',
  'pk/asn1/der/bit/der_encode_bit_string.c',
  'pk/asn1/der/bit/der_encode_raw_bit_string.c',
  'pk/asn1/der/bit/der_length_bit_string.c',
  'pk/asn1/der/boolean/der_decode_boolean.c',
  'pk/asn1/der/boolean/der_encode_boolean.c',
  'pk/asn1/der/boolean/der_length_boolean.c',
  'pk/asn1/der/choice/der_decode_choice.c',
  'pk/asn1/der/generalizedtime/der_decode_generalizedtime.c',
  'pk/asn1/der/generalizedtime/der_encode_generalizedtime.c',
  'pk/asn1/der/generalizedtime/der_length_generalizedtime.c',
  'pk/asn1/der/ia5/der_decode_ia5_string.c',
  'pk/asn1/der/ia5/der_encode_ia5_string.c',
  'pk/asn1/der/ia5/der_length_ia5_string.c',
  'pk/asn1/der/integer/der_decode_integer.c',
  'pk/asn1/der/integer/der_encode_integer.c',
  'pk/asn1/der/integer/der_length_integer.c',
  'pk/asn1/der/object_identifier/der_decode_object_identifier.c',
  'pk/asn1/der/object_identifier/der_encode_object_identifier.c',
  'pk/asn1/der/object_identifier/der_length_object_identifier.c',
  'pk/asn1/der/octet/der_decode_octet_string.c',
  'pk/asn1/der/octet/der_encode_octet_string.c',
  'pk/asn1/der/octet/der_length_octet_string.c',
  'pk/asn1/der/printable_string/der_decode_printable_string.c',
  'pk/asn1/der/printable_string/der_encode_printable_string.c',
  'pk/asn1/der/printable_string/der_length_printable_string.c',
  'pk/asn1/der/sequence/der_decode_sequence_ex.c',
  'pk/asn1/der/sequence/der_decode_sequence_flexi.c',
  'pk/asn1/der/sequence/der_decode_sequence_multi.c',
  'pk/asn1/der/sequence/der_decode_subject_public_key_info.c',
  'pk/asn1/der/sequence/der_encode_sequence_ex.c',
  'pk/asn1/der/sequence/der_encode_sequence_multi.c',
  'pk/asn1/der/sequence/der_encode_subject_public_key_info.c',
  'pk/asn1/der/sequence/der_length_sequence.c',
  'pk/asn1/der/sequence/der_sequence_free.c',
  'pk/asn1/der/sequence/der_sequence_shrink.c',
  'pk/asn1/der/set/der_encode_set.c', 'pk/asn1/der/set/der_encode_setof.c',
  'pk/asn1/der/short_integer/der_decode_short_integer.c',
  'pk/asn1/der/short_integer/der_encode_short_integer.c',
  'pk/asn1/der/short_integer/der_length_short_integer.c',
  'pk/asn1/der/teletex_string/der_decode_teletex_string.c',
  'pk/asn1/der/teletex_string/der_length_teletex_string.c',
  'pk/asn1/der/utctime/der_decode_utctime.c',
  'pk/asn1/der/utctime/der_encode_utctime.c',
  'pk/asn1/der/utctime/der_length_utctime.c',
  'pk/asn1/der/utf8/der_decode_utf8_string.c',
  'pk/asn1/der/utf8/der_encode_utf8_string.c',
  'pk/asn1/der/utf8/der_length_utf8_string.c', 'pk/dh/dh.c',
  'pk/dh/dh_check_pubkey.c', 'pk/dh/dh_export.c', 'pk/dh/dh_export_key.c',
  'pk/dh/dh_free.c', 'pk/dh/dh_generate_key.c', 'pk/dh/dh_import.c',
  'pk/dh/dh_set.c', 'pk/dh/dh_set_pg_dhparam.c', 'pk/dh/dh_shared_secret.c',
  'pk/dsa/dsa_decrypt_key.c', 'pk/dsa/dsa_encrypt_key.c', 'pk/dsa/dsa_export.c',
  'pk/dsa/dsa_free.c', 'pk/dsa/dsa_generate_key.c', 'pk/dsa/dsa_generate_pqg.c',
  'pk/dsa/dsa_import.c', 'pk/dsa/dsa_make_key.c', 'pk/dsa/dsa_set.c',
  'pk/dsa/dsa_set_pqg_dsaparam.c', 'pk/dsa/dsa_shared_secret.c',
  'pk/dsa/dsa_sign_hash.c', 'pk/dsa/dsa_verify_hash.c',
  'pk/dsa/dsa_verify_key.c', 'pk/ecc/ecc.c', 'pk/ecc/ecc_ansi_x963_export.c',
  'pk/ecc/ecc_ansi_x963_import.c', 'pk/ecc/ecc_decrypt_key.c',
  'pk/ecc/ecc_encrypt_key.c', 'pk/ecc/ecc_export.c', 'pk/ecc/ecc_free.c',
  'pk/ecc/ecc_get_size.c', 'pk/ecc/ecc_import.c', 'pk/ecc/ecc_make_key.c',
  'pk/ecc/ecc_shared_secret.c', 'pk/ecc/ecc_sign_hash.c', 'pk/ecc/ecc_sizes.c',
  'pk/ecc/ecc_test.c', 'pk/ecc/ecc_verify_hash.c',
  'pk/ecc/ltc_ecc_is_valid_idx.c', 'pk/ecc/ltc_ecc_map.c',
  'pk/ecc/ltc_ecc_mul2add.c', 'pk/ecc/ltc_ecc_mulmod.c',
  'pk/ecc/ltc_ecc_mulmod_timing.c', 'pk/ecc/ltc_ecc_points.c',
  'pk/ecc/ltc_ecc_projective_add_point.c',
  'pk/ecc/ltc_ecc_projective_dbl_point.c', 'pk/katja/katja_decrypt_key.c',
  'pk/katja/katja_encrypt_key.c', 'pk/katja/katja_export.c',
  'pk/katja/katja_exptmod.c', 'pk/katja/katja_free.c', 'pk/katja/katja_import.c',
  'pk/katja/katja_make_key.c', 'pk/pkcs1/pkcs_1_i2osp.c',
  'pk/pkcs1/pkcs_1_mgf1.c', 'pk/pkcs1/pkcs_1_oaep_decode.c',
  'pk/pkcs1/pkcs_1_oaep_encode.c', 'pk/pkcs1/pkcs_1_os2ip.c',
  'pk/pkcs1/pkcs_1_pss_decode.c', 'pk/pkcs1/pkcs_1_pss_encode.c',
  'pk/pkcs1/pkcs_1_v1_5_decode.c', 'pk/pkcs1/pkcs_1_v1_5_encode.c',
  'pk/rsa/rsa_decrypt_key.c', 'pk/rsa/rsa_encrypt_key.c', 'pk/rsa/rsa_export.c',
  'pk/rsa/rsa_exptmod.c', 'pk/rsa/rsa_free.c', 'pk/rsa/rsa_get_size.c',
  'pk/rsa/rsa_import.c', 'pk/rsa/rsa_import_pkcs8.c', 'pk/rsa/rsa_import_x509.c',
  'pk/rsa/rsa_make_key.c', 'pk/rsa/rsa_set.c', 'pk/rsa/rsa_sign_hash.c',
  'pk/rsa/rsa_sign_saltlen_get.c', 'pk/rsa/rsa_verify_hash.c',
  'prngs/chacha20.c', 'prngs/fortuna.c', 'prngs/rc4.c', 'prngs/rng_get_bytes.c',
  'prngs/rng_make_prng.c', 'prngs/sober128.c', 'prngs/sprng.c', 'prngs/yarrow.c',
  'stream/chacha/chacha_crypt.c', 'stream/chacha/chacha_done.c',
  'stream/chacha/chacha_ivctr32.c', 'stream/chacha/chacha_ivctr64.c',
  'stream/chacha/chacha_keystream.c', 'stream/chacha/chacha_setup.c',
  'stream/chacha/chacha_test.c', 'stream/rc4/rc4_stream.c',
  'stream/rc4/rc4_test.c', 'stream/sober128/sober128_stream.c',
  'stream/sober128/sober128_test.c',
  ]
  tomcrypt_src += 'libtomcrypt/src' / _file
endforeach

tomcrypt_enc_lib = static_library('tomcrypt_enc',
  c_args: ['-DLTC_SOURCE', '-DENCRYPT_ONLY'],
  include_directories: include_directories(
    'libtommath',
    'libtomcrypt/src/headers',
    ),
  sources: [
    default_options_guard_h,
    'libtomcrypt/src/ciphers/aes/aes.c',
    ],
  )

tomcrypt_lib = static_library('tomcrypt',
  c_args: ['-DLTC_SOURCE'],
  include_directories: include_directories(
    'libtommath',
    'libtomcrypt/src/headers',
    ),
  sources: [
    default_options_guard_h,
    files(tomcrypt_src),
    ],
  )

tommath_src = []
foreach _file: [
  'bn_cutoffs.c', 'bn_deprecated.c', 'bn_mp_2expt.c', 'bn_mp_abs.c',
  'bn_mp_add.c', 'bn_mp_add_d.c', 'bn_mp_addmod.c', 'bn_mp_and.c',
  'bn_mp_clamp.c', 'bn_mp_clear.c', 'bn_mp_clear_multi.c', 'bn_mp_cmp.c',
  'bn_mp_cmp_d.c', 'bn_mp_cmp_mag.c', 'bn_mp_cnt_lsb.c', 'bn_mp_complement.c',
  'bn_mp_copy.c', 'bn_mp_count_bits.c', 'bn_mp_decr.c', 'bn_mp_div.c',
  'bn_mp_div_2.c', 'bn_mp_div_2d.c', 'bn_mp_div_3.c', 'bn_mp_div_d.c',
  'bn_mp_dr_is_modulus.c', 'bn_mp_dr_reduce.c', 'bn_mp_dr_setup.c',
  'bn_mp_error_to_string.c', 'bn_mp_exch.c', 'bn_mp_expt_u32.c',
  'bn_mp_exptmod.c', 'bn_mp_exteuclid.c', 'bn_mp_fread.c', 'bn_mp_from_sbin.c',
  'bn_mp_from_ubin.c', 'bn_mp_fwrite.c', 'bn_mp_gcd.c', 'bn_mp_get_i32.c',
  'bn_mp_get_i64.c', 'bn_mp_get_l.c', 'bn_mp_get_ll.c', 'bn_mp_get_mag_u32.c',
  'bn_mp_get_mag_u64.c', 'bn_mp_get_mag_ul.c', 'bn_mp_get_mag_ull.c',
  'bn_mp_grow.c', 'bn_mp_incr.c', 'bn_mp_init.c', 'bn_mp_init_copy.c',
  'bn_mp_init_i32.c', 'bn_mp_init_i64.c', 'bn_mp_init_l.c', 'bn_mp_init_ll.c',
  'bn_mp_init_multi.c', 'bn_mp_init_set.c', 'bn_mp_init_size.c',
  'bn_mp_init_u32.c', 'bn_mp_init_u64.c', 'bn_mp_init_ul.c',
  'bn_mp_init_ull.c', 'bn_mp_invmod.c', 'bn_mp_is_square.c', 'bn_mp_iseven.c',
  'bn_mp_isodd.c', 'bn_mp_kronecker.c', 'bn_mp_lcm.c', 'bn_mp_log_u32.c',
  'bn_mp_lshd.c', 'bn_mp_mod.c', 'bn_mp_mod_2d.c', 'bn_mp_mod_d.c',
  'bn_mp_montgomery_calc_normalization.c', 'bn_mp_montgomery_reduce.c',
  'bn_mp_montgomery_setup.c', 'bn_mp_mul.c', 'bn_mp_mul_2.c', 'bn_mp_mul_2d.c',
  'bn_mp_mul_d.c', 'bn_mp_mulmod.c', 'bn_mp_neg.c', 'bn_mp_or.c',
  'bn_mp_pack.c', 'bn_mp_pack_count.c', 'bn_mp_prime_fermat.c',
  'bn_mp_prime_frobenius_underwood.c', 'bn_mp_prime_is_prime.c',
  'bn_mp_prime_miller_rabin.c', 'bn_mp_prime_next_prime.c',
  'bn_mp_prime_rabin_miller_trials.c', 'bn_mp_prime_rand.c',
  'bn_mp_prime_strong_lucas_selfridge.c', 'bn_mp_radix_size.c',
  'bn_mp_radix_smap.c', 'bn_mp_rand.c', 'bn_mp_read_radix.c', 'bn_mp_reduce.c',
  'bn_mp_reduce_2k.c', 'bn_mp_reduce_2k_l.c', 'bn_mp_reduce_2k_setup.c',
  'bn_mp_reduce_2k_setup_l.c', 'bn_mp_reduce_is_2k.c',
  'bn_mp_reduce_is_2k_l.c', 'bn_mp_reduce_setup.c', 'bn_mp_root_u32.c',
  'bn_mp_rshd.c', 'bn_mp_sbin_size.c', 'bn_mp_set.c', 'bn_mp_set_i32.c',
  'bn_mp_set_i64.c', 'bn_mp_set_l.c', 'bn_mp_set_ll.c', 'bn_mp_set_u32.c',
  'bn_mp_set_u64.c', 'bn_mp_set_ul.c', 'bn_mp_set_ull.c', 'bn_mp_shrink.c',
  'bn_mp_signed_rsh.c', 'bn_mp_sqr.c', 'bn_mp_sqrmod.c', 'bn_mp_sqrt.c',
  'bn_mp_sqrtmod_prime.c', 'bn_mp_sub.c', 'bn_mp_sub_d.c', 'bn_mp_submod.c',
  'bn_mp_to_radix.c', 'bn_mp_to_sbin.c', 'bn_mp_to_ubin.c',
  'bn_mp_ubin_size.c', 'bn_mp_unpack.c', 'bn_mp_xor.c', 'bn_mp_zero.c',
  'bn_prime_tab.c', 'bn_s_mp_add.c', 'bn_s_mp_balance_mul.c',
  'bn_s_mp_exptmod.c', 'bn_s_mp_exptmod_fast.c', 'bn_s_mp_get_bit.c',
  'bn_s_mp_invmod_fast.c', 'bn_s_mp_invmod_slow.c', 'bn_s_mp_karatsuba_mul.c',
  'bn_s_mp_karatsuba_sqr.c', 'bn_s_mp_montgomery_reduce_fast.c',
  'bn_s_mp_mul_digs.c', 'bn_s_mp_mul_digs_fast.c', 'bn_s_mp_mul_high_digs.c',
  'bn_s_mp_mul_high_digs_fast.c', 'bn_s_mp_prime_is_divisible.c',
  'bn_s_mp_rand_jenkins.c', 'bn_s_mp_rand_platform.c', 'bn_s_mp_reverse.c',
  'bn_s_mp_sqr.c', 'bn_s_mp_sqr_fast.c', 'bn_s_mp_sub.c', 'bn_s_mp_toom_mul.c',
  'bn_s_mp_toom_sqr.c',
  ]
  tommath_src += 'libtommath' / _file
endforeach

tommath_lib = static_library('tommath',
  sources: [
    default_options_guard_h,
    files(tommath_src),
    ],
  )

executable('dropbear',
  c_args: c_args,
  include_directories: include_directories(
    'libtomcrypt/src/headers',
    ),
  dependencies: [bsd_dep, crypt_dep, util_dep, zlib_dep],
  install: true,
  link_args: link_args,
  link_with: [tomcrypt_enc_lib, tomcrypt_lib, tommath_lib],
  sources: [config_h, default_options_guard_h] + files(
    'dbutil.c', 'buffer.c', 'dbhelpers.c', 'dss.c', 'bignum.c', 'signkey.c',
    'rsa.c', 'dbrandom.c', 'queue.c', 'atomicio.c', 'compat.c',
    'fake-rfc2553.c', 'ltc_prng.c', 'ecc.c', 'ecdsa.c', 'sk-ecdsa.c',
    'crypto_desc.c', 'curve25519.c', 'ed25519.c', 'sk-ed25519.c', 'dbmalloc.c',
    'gensignkey.c', 'gendss.c', 'genrsa.c', 'gened25519.c', 'common-session.c',
    'packet.c', 'common-algo.c', 'common-kex.c', 'common-channel.c',
    'common-chansession.c', 'termcodes.c', 'loginrec.c', 'tcp-accept.c',
    'listener.c', 'process-packet.c', 'dh_groups.c', 'common-runopts.c',
    'circbuffer.c', 'list.c', 'netio.c', 'chachapoly.c', 'gcm.c', 'svr-kex.c',
    'svr-auth.c', 'sshpty.c', 'svr-authpasswd.c', 'svr-authpubkey.c',
    'svr-authpubkeyoptions.c', 'svr-session.c', 'svr-service.c',
    'svr-chansession.c', 'svr-runopts.c', 'svr-agentfwd.c', 'svr-main.c',
    'svr-x11fwd.c', 'svr-tcpfwd.c', 'svr-authpam.c',
    ),
  )

if get_option('scp')
  executable('scp',
    c_args: c_args + '-DPROGRESS_METER',
    dependencies: [zlib_dep],
    include_directories: include_directories(
      'libtomcrypt/src/headers',
      ),
    install: true,
    sources: [config_h, default_options_guard_h] + files(
      'scp.c', 'progressmeter.c', 'atomicio.c', 'scpmisc.c', 'compat.c', 
      ),
    )
endif
