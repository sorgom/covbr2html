#include <Covbr2HtmlGlob.h>
#include <SOM/docopts.h>
#define TRACE_ME
#include <SOM/TraceMacros.h>

#include <filesystem>
#include <iostream>
#include <regex>

const CONST_C_STRING cHelp =
    "\nconvert covbr text files to html\n\n"
    "usage: ARGV0 [options] covbr files\n"
    "options:\n"
    "-o  <directory> output to directory\n"
    "-c  highlight covered items\n"
    "-f  keep fully covered sources listed\n"
    "-h  this help\n"
;

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC_TIME()
    auto ret = 1;
    DocOpts opts(cHelp);
    if (opts.process(argc, argv))
    {
        if (opts.isSet('h'))
        {
            opts.help(argv[0]);
        }
        else
        {
            Covbr2HtmlGlob proc;
            proc.setHc(opts.isSet('c'));
            proc.setFc(opts.isSet('f'));
            CONST_C_STRING odir = nullptr;
            if ((not opts.getValue(odir, 'o')) or proc.setOdir(odir))
            {
                Glob glob(proc, true);
                for (auto i = 0; i < opts.argc(); ++i)
                {
                    glob.glob(opts.args()[i]);
                }
            }
            ret = proc.ret();
        }
    }
    return ret;
}
