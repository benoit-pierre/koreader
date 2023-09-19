#!/usr/bin/python3

from collections import defaultdict
from pathlib import Path
from shlex import quote
from subprocess import check_call, check_output
import os
import re
import shutil
import struct
import sys


DRY_RUN = 'MESON_INSTALL_DRY_RUN' in os.environ
VERBOSE = 'MESON_INSTALL_QUIET' not in os.environ

LIBRX = {
    'android': re.compile('^lib(?P<name>[^.]+).so(?:.(?P<version>\d+[.0-9]*))?$'),
    'darwin': re.compile('^lib(?P<name>[^.]+)(?:.(?P<version>\d+[.0-9]*))?.dylib$'),
    'linux': re.compile('^lib(?P<name>[^.]+).so(?:.(?P<version>\d+[.0-9]*))?$'),
}


if sys.platform == 'linux':
    MACOS_INSTNT = 'x86_64-apple-darwin-install_name_tool'
    MACOS_OTOOL = 'x86_64-apple-darwin-otool'
else:
    MACOS_INSTNT = 'install_name_tool'
    MACOS_OTOOL = 'otool'

def macho_type(path):
    with Path(path).open('rb') as fp:
        hdr = fp.read(16)
    if len(hdr) != 16:
        raise ValueError(path)
    magic = struct.unpack('4s', hdr[:4])[0]
    if magic in {b'\xfe\xed\xfa\xce', b'\xfe\xed\xfa\xcf'}:
        byte_order = '>'
    elif magic in {b'\xce\xfa\xed\xfe', b'\xcf\xfa\xed\xfe'}:
        byte_order = '<'
    else:
        raise ValueError(path)
    ftype = struct.unpack(byte_order + 'I', hdr[-4:])[0]
    if ftype in {2, 5}:
        return 'exe'
    if ftype in {6, 8}:
        return 'lib'
    return ftype


def relpath(path, anchor):
    commonpath = os.path.commonpath((path, anchor))
    return Path(*(['..'] * len(anchor.relative_to(commonpath).parts) + [path.relative_to(commonpath)]))

def ln(link, target):
    if DRY_RUN or VERBOSE:
        print('ln -s', quote(str(target)), quote(str(link)))
    if DRY_RUN:
        return
    Path(link).symlink_to(target)

def rm(path):
    if DRY_RUN or VERBOSE:
        print('rm', quote(str(path)))
    if DRY_RUN:
        return
    Path(path).unlink()

def rmtree(path):
    if DRY_RUN or VERBOSE:
        print('rm -rf', quote(str(path)))
    if DRY_RUN:
        return
    shutil.rmtree(path)


def mv(src, dst):
    if DRY_RUN or VERBOSE:
        print('mv', quote(str(src)), quote(str(dst)))
    if DRY_RUN:
        return
    Path(src).rename(dst)


def collect_libraries(libsdir, system):
    librx = LIBRX.get(system)
    if librx is None:
        return {}
    libraries = defaultdict(list)
    for dirpath, dirnames, filenames in os.walk(libsdir):
        for entry in filenames:
            m = librx.match(entry)
            if m is None:
                continue
            name, version = m['name'], m['version']
            libraries[name].append((Path(dirpath) / entry, version))
    return libraries

def fix_devel_libraries(prefix, staging, system):
    if not staging.exists():
        return
    dev_libraries = collect_libraries(prefix / staging, system)
    runtime_libsdir = prefix.parent / 'koreader/libs'
    runtime_libraries = collect_libraries(runtime_libsdir, system)
    for libname, candidates in dev_libraries.items():
        assert len(candidates) == 1
        dev_path = candidates[0][0]
        assert dev_path.is_symlink()
        rm(dev_path)
        runtime_candidates = runtime_libraries.get(libname)
        assert len(runtime_candidates) == 1
        runtime_path = runtime_candidates[0][0]
        runtime_path = runtime_path.relative_to(prefix.parent)
        runtime_path = Path('../..') / runtime_path
        ln(dev_path, runtime_path)

def fix_runtime_libraries(prefix, staging, system):
    assert staging.name.endswith('.staging')
    libsdir = prefix / staging.name[:-8]
    if libsdir.exists():
        rmtree(libsdir)
    if not staging.exists():
        return
    libraries = collect_libraries(staging, system)
    for libname, candidates in libraries.items():
        if len(candidates) == 1:
            continue
        match = None
        sound = None
        for path, version in candidates:
            if path.is_symlink():
                match = path
            elif path.is_file():
                sound = path
        assert None not in (match, sound)
        rm(match)
        mv(sound, match)
        for path, version in candidates:
            if path not in (match, sound):
                rm(path)
    mv(staging, libsdir)

def collect_macos_objects(prefix):
    librx = LIBRX.get('darwin')
    for dirpath, dirnames, filenames in os.walk(prefix):
        for entry in filenames:
            path = Path(dirpath, entry)
            if not path.stat().st_mode & 0o111:
                continue
            try:
                obj_type = macho_type(path)
            except ValueError:
                continue
            yield obj_type, path

def fix_macho_libs(path, staging_lib_rx):
    lines = check_output((MACOS_OTOOL, '-L', path)).decode().strip().split('\n')[1:]
    if not lines:
        return
    for l in lines:
        m = staging_lib_rx.match(l)
        if m is None:
            continue
        old, new = m.group(1, 2)
        new = '@rpath/' + new
        cmd = (MACOS_INSTNT, '-change', old, new, str(path))
        if DRY_RUN or VERBOSE:
           print(' '.join(map(quote, cmd)))
        if DRY_RUN:
           continue
        check_call(cmd)

def fix_macho_rpath(path, libdir):
    rpath = str(Path('@executable_path', relpath(libdir, path.parent)))
    cmd = (MACOS_INSTNT, '-add_rpath', rpath, str(path))
    if DRY_RUN or VERBOSE:
       print(' '.join(map(quote, cmd)))
    if DRY_RUN:
       return
    check_call(cmd)

def fix_macos_runtime_paths(prefix, staging):
    assert staging.endswith('.staging')
    libsdir = prefix / staging[:-8]
    staging_lib_rx = re.compile(r'^\t(/' + re.escape(staging) + r'/([^\s]+\.dylib)) ')
    objects = collect_macos_objects(prefix)
    for obj_type, obj in objects:
        if obj_type == 'exe':
            fix_macho_libs(obj, staging_lib_rx)
            fix_macho_rpath(obj, libsdir)
        elif obj_type == 'lib':
            fix_macho_libs(obj, staging_lib_rx)
        else:
            print('ignoring Mach-o file: “%s” [type: %s]' % (obj, obj_type), file=sys.stderr)


def main():
    system, staging, tag = sys.argv[1:]
    assert tag in ['devel', 'runtime']
    prefix = Path(os.environ['MESON_INSTALL_DESTDIR_PREFIX'])
    if tag == 'devel':
        fix_devel_libraries(prefix, prefix / staging, system)
    elif tag == 'runtime':
        fix_runtime_libraries(prefix, prefix / staging, system)
        if system == 'darwin':
            fix_macos_runtime_paths(prefix, staging)


if __name__ == '__main__':
    main()
