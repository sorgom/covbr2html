"""
clean covbr reports from fully covered files and create html reports

usage: this script [options] files
options:
    -o  <directory> output directory
    -w  re-write cleaned covbr text files
    -c  highlight covered items
    -h  help
"""

import re
from os.path import dirname, basename, join, isdir
from os import makedirs
from html import escape
import sompy
#   due to usage of match case
import checkVersion
checkVersion.apply(3, 10, __file__)

class Covbr2html(object):
    """covbr cleaner and html converter"""
    def __init__(self, wb:bool=False, hc=False, odir=None) -> None:
        template = join(dirname(__file__), 'covbr_template.html')
        with open(template, 'r') as fh:
            self.template = fh.read()
            fh.close()
        self.err = False
        self.wb = wb
        self.odir = odir
        if odir:
            if not isdir(odir):
                try:
                    makedirs(odir)
                except:
                    print('cannot create output directory:', odir)
                    self.err = True

        if hc:
            self.okb = '<i>'
            self.oke = '</i>'
        else:
            self.okb = ''
            self.oke = ''
        self.rFile = r'(?:\w+:/?)?\w+(?:/\w+)*\.(?:cpp|h(?:pp)?):'
        self.rEclip = r'(?: +\.\.\.\n)?'
        self.rxFile = re.compile(rf'^{self.rFile}', re.M)
        self.rxDouble = re.compile(rf'^{self.rEclip}(?:{self.rFile}\n)*({self.rFile})', re.M)
        self.rxTail = re.compile(rf'{self.rEclip}(?:{self.rFile})?\s*$')
        self.rxSpc = re.compile(rf'\s+(\n{self.rFile})')
        self.rx_fp = re.compile(rf'^\n*({self.rFile})\n*', re.M)
        self.rx_ok = re.compile(r'^( *)(X|TF|tf)(?:$| (.*))', re.M)
        self.rx_nok = re.compile(r'^( *)--&gt;(\w+)?( .*)?', re.M)
        self.cnt = 0

    def ok(self):
        return not self.err

    def write(self, fp:str, newc:str):
        if self.odir:
            fp = join(self.odir, basename(fp))
        with open(fp, 'w') as fh:
            fh.write(newc)
            fh.close()
            self.cnt += 1

    #   indication: not covered
    def _replNok(self, mo):
        ind, what, line = mo.groups('')
        match what:
            case 'T': tag = f'<u>T</u><s>F</s>  '
            case 'F': tag = f'<s>T</s><u>F</u>  '
            case 't': tag = f'<u>t</u><s>f</s>  '
            case 'f': tag = f'<s>t</s><u>f</u>  '
            case _:
                tag = '<s>X</s>  '
        return f'<b>{ind}{tag}{line}</b>'

    #   indication: covered
    def _replOk(self, mo):
        ind, what, line = mo.groups('')
        match what:
            case 'X':
                tag = '  '
            case _:
                tag = f'<u>{what}</u> '
        return f'{self.okb}{ind}{tag}{line}{self.oke}'

    def process(self, fp:str):
        """clean txt file write html file"""
        if self.err: return
        with open(fp, 'r') as fh:
            oldc = fh.read()
            fh.close()
            if not self.rxFile.search(oldc): return
            # clean txt
            newc = re.sub(r'\s+$', '',
                self.rxTail.sub('',
                    self.rxSpc.sub(r'\n\1',
                        self.rxDouble.sub(r'\n\1', oldc.replace('\r', ''))))) + '\n'

            if not self.rxFile.search(newc): return

            if self.wb and (self.odir or newc != oldc):
                self.write(fp, newc)

            # create html
            newc = self.rx_ok.sub(self._replOk,
                    self.rx_nok.sub(self._replNok,
                        self.rx_fp.sub(r'\n<em>\1</em>\n', escape(newc)))).strip()
            fp = re.sub(r'\.\w+$', '', fp)
            ttl = basename(fp)
            newc = self.template.replace('##TITLE', ttl, 1).replace('##CONTENT', newc, 1)
            self.write(fp + '.html', newc)

if __name__ == '__main__':
    from docopts import docopts
    from fglob import fglob

    opts, args = docopts(__doc__, reqArgs=True)
    cb = Covbr2html(wb=opts.get('w', False), hc=opts.get('c', False), odir=opts.get('o'))
    if cb.ok():
        for arg in fglob(args):
            cb.process(arg)
        if cb.cnt: print('>', cb.cnt)
