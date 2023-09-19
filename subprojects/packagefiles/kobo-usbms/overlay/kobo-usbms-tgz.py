#!/usr/bin/env python

from pathlib import Path
import sys
import tarfile


def main(epoch, outfile, srcdir, *inputs):
    epoch = int(epoch)
    outdir = Path(outfile).resolve().parent
    srcdir = Path(srcdir).resolve()
    def fix_tarinfo(tarinfo):
        tarinfo.mtime = epoch
        tarinfo.uid = tarinfo.gid = 0
        tarinfo.uname = tarinfo.gname = "root"
        return tarinfo
    with tarfile.open(outfile, 'w:gz') as tf:
        for i in map(Path, inputs):
            i = i.resolve()
            if i.is_relative_to(outdir):
                arcname = str(i.relative_to(outdir))
            elif i.is_relative_to(srcdir):
                arcname = str(i.relative_to(srcdir))
            else:
                arcname = str(i)
            tf.add(i, arcname='Kobo/' + arcname, filter=fix_tarinfo)


if __name__ == '__main__':
    main(*sys.argv[1:])
