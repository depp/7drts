#!/usr/bin/env python3
import os
import errno
import subprocess
import shutil
import pipes
import sys
import json

devnull = open(os.devnull, 'w+')

def find_images(path):
    paths = []
    cwd = os.getcwd()
    for dirpath, dirnames, filenames in os.walk('.'):
        dirnames[:] = [dirname for dirname in dirnames
                       if not dirname.startswith('.')]
        for filename in filenames:
            if filename.endswith('.png') and not filename.startswith('.'):
                paths.append(os.path.relpath(
                    os.path.join(dirpath, filename),
                    path))
    return paths

UNIT_SMALL = '-resize 20%'
UNIT_BIG = '-resize 25%'
UNIT_TALL = '-resize 25%'
CONVERSION = {
    'accessories': {
        'arrow': UNIT_SMALL,
        'bowandarrow': UNIT_SMALL,
        'axe': UNIT_BIG,
        'collar': UNIT_BIG,
        'halo': UNIT_TALL,
        'shield': UNIT_BIG,
        'splat': '-resize 15%',
        'staff': UNIT_TALL,
    },
    'unit_big': UNIT_BIG,
    'unit_small': UNIT_SMALL,
    'unit_tall': UNIT_TALL,
}

def get_opts(imgname):
    obj = CONVERSION
    for part in os.path.splitext(imgname)[0].split('/'):
        if isinstance(obj, str):
            return obj
        while True:
            try:
                obj = obj[part]
            except KeyError:
                i = part.rfind('_')
                if i < 0:
                    return None
                part = part[:i]
            else:
                break
    if not isinstance(obj, str):
        return None
    return obj

def convert(imgname, srcdir, destdir):
    opts = get_opts(imgname)
    if opts is None:
        print('cannot find options for {}'.format(imgname))
        sys.exit(1)

    srcpath = os.path.join(srcdir, imgname)
    destpath = os.path.join(destdir, imgname)
    os.makedirs(os.path.dirname(destpath), exist_ok=True)
    cmd = ['gm', 'convert', srcpath, '-trim'] + opts.split() + [destpath]
    print(' '.join(pipes.quote(x) for x in cmd))
    subprocess.check_call(cmd, stdout=devnull)

def run(srcpath, destpath):
    try:
        shutil.rmtree(destpath)
    except FileNotFoundError:
        pass
    images = find_images('.')
    for image in images:
        convert(image, srcpath, destpath)

    cmd = [os.path.expanduser('~/Projects/fontsimple/makesprites.py')]
    cmd += '--margin 16 --full-path --no-pngcrush -o sprites'.split()
    cmd.extend(images)
    print(' '.join(pipes.quote(x) for x in cmd))
    subprocess.check_call(cmd, cwd=destpath)

    desc = json.load(open(os.path.join(destpath, 'sprites.json')))
    with open(os.path.join(destpath, 'sprites.txt'), 'w') as fp:
        for k, v in sorted(desc.items()):
            fp.write('{} {} {} {} {}\n'.format(k, *v))

    cmd = ['pngcrush', 'sprites.png', 'sprites-crush.png']
    subprocess.check_call(cmd, cwd=destpath)

run('.', 'output')

