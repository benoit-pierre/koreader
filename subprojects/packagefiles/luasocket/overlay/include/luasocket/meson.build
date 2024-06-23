fs = import('fs')
foreach _hdr : [
  'buffer.h',
  'compat.h',
  'io.h',
  'luasocket.h',
  'socket.h',
  'timeout.h',
  'usocket.h',
  'wsocket.h',
]
  luasocket_headers += fs.copyfile(meson.project_source_root() / 'src' / _hdr, _hdr)
endforeach
