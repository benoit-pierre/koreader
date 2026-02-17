#!/usr/bin/env python

from collections import defaultdict
from pathlib import Path
from subprocess import check_output
import os
import re
import sys


ARCHIVE_RX = re.compile(r'.+ \(ex (?P<archive>.+\.a)\)')


def codesize(pathlist, strip=0):
    out = check_output(['size', '--format=berkeley', '--radix=10', '--totals'] + pathlist)
    total_fsize = 0
    headers = None
    results = {}
    for line in out.decode().strip().split('\n'):
        if headers is None:
            headers = line.split()
            continue
        line = line.split(None, len(headers) - 1)
        assert len(headers) == len(line), (headers, line)
        fields = {}
        for k, v in zip(headers, line):
            if k == 'filename':
                if v == '(TOTALS)':
                    fields['file'] = total_fsize
                    key = v
                else:
                    # For an object in a static archive, use the
                    # archive path so we can aggregate stats later.
                    m = ARCHIVE_RX.fullmatch(v)
                    if m is None:
                        archive = None
                    else:
                        archive = v = m.group('archive')
                    fields['archive'] = archive
                    fields['file'] = Path(v).stat().st_size
                    v = Path(*Path(v).parts[strip:])
                    key = v.name
                    m = re.fullmatch(r'(.*)\.so(.\d+)*$', key)
                    if m is not None:
                        key = m.group(1)
                    if key == 'libleptonica':
                        key = 'liblept'
                fields['key'] = key
                v = str(v)
            elif k == 'dec':
                k = 'code'
                v = int(v)
            elif k == 'hex':
                continue
            else:
                v = int(v)
            fields[k] = v
        if 'archive' in fields and fields['key'] in results:
            # Aggregate static archive stats.
            r = results[fields['key']]
            for k, v in fields.items():
                if k != 'file' and isinstance(v, int):
                    r[k] += v
                else:
                    assert v == r[k], (k, v, r[k])
        else:
            assert fields['key'] not in results, fields['key']
            results[fields['key']] = fields
            total_fsize += fields['file']
    return results

def naturalsize(size, delta=False):
    for chunk, suffix, precision in (
        (10**9, 'GB', 1),
        (10**6, 'MB', 1),
        (10**3, 'KB', 1),
    ):
        if abs(size) >= chunk:
            break
    else:
        chunk, suffix, precision = 1, '  B ', 0
    fmt = '%'
    if delta:
        fmt += '+'
    fmt += '.*f %s'
    return fmt % (precision, size / chunk, suffix)


def main():
    args = sys.argv[1:]
    short = False
    percent = False
    while args[0].startswith('-'):
        a = args.pop(0)
        if a == '-s':
            short = True
        elif a == '-p':
            percent = True
        else:
            raise ValueError(a)
    pathlist = list(map(Path, args))
    groups = defaultdict(set)
    if len(pathlist) == 1:
        prefix = -1
    else:
        prefix = len(Path(os.path.commonpath(pathlist)).parts)
    for p in pathlist:
        root = Path(*p.parts[:prefix+1])
        assert p not in groups[root], (str(p), groups[root])
        groups[root].add(p)
    groups = {
        k: codesize(list(v), strip=prefix+1)
        for k, v in groups.items()
    }
    headers = ['filename', 'bss', 'data', 'text', 'code', 'file']
    headers_max = [max(len(e['filename']) for g in groups.values() for e in g.values()) + 1]
    size_max = len('1012.8 KB')
    delta_max = len('-1021.1 KB')
    for n in range(1, len(headers)):
        headers_max.append(size_max + 3 + delta_max)
    hline_size = sum(headers_max) + 2 * (len(headers_max) - 1)
    hline = '⎼' * hline_size
    header_line = []
    for n, (h, m) in enumerate(zip(headers, headers_max)):
        h = h.upper()
        if n != 0:
            h = ' ' * ((size_max - len(h)) // 2) + h
        header_line.append('%-*s' % (m, h))
    header_line = '  '.join(header_line)
    old_group = None
    for n, (root, new_group) in enumerate(groups.items()):
        if old_group is not None:
            print()
        if not short or old_group is not None:
            print(root)
            print(hline)
            print(header_line)
            print(hline)
            keys = sorted(set(new_group.keys()) | set((old_group or {}).keys()),
                          key=lambda p: (1 if p == '(TOTALS)' else 0, p))
            for k in keys:
                if k == keys[-1]:
                    print(hline)
                new = new_group.get(k, {})
                old = (old_group or {}).get(k, {})
                for h, m in zip(headers, headers_max):
                    if h == 'filename':
                        s = new.get(h) or old[h]
                        if not new:
                            s = '-' + s
                        elif old_group and not old:
                            s = '+' + s
                        else:
                            s = ' ' + s
                    else:
                        val = new.get(h, 0)
                        s = '%*s' % (size_max, naturalsize(val))
                        if old_group:
                            o = old.get(h, 0)
                            if percent:
                                p = (val / o - 1) * 100 if o else 0
                                if abs(p) >= 0.1:
                                    s += ' (%*s)' % (delta_max, '%+3.1f%%' % p)
                            else:
                                d = val - o
                                if d:
                                    s += ' (%*s)' % (delta_max, naturalsize(d, delta=True))
                    print('%-*s' % (m, s), end='  ')
                print()
        old_group = new_group


if __name__ == '__main__':
    main()
