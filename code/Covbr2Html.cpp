#include <Covbr2Html.h>
#include <SOM/fio.h>
#include <filesystem>
#include <fstream>
#include <iostream>

// see:
// https://stackoverflow.com/questions/42909618/a-simple-c11-regex-that-throws-regex-error-while-searching
#ifdef _MSC_VER
#define _REGEX_MAX_STACK_COUNT 200000
#endif
#include <SOM/pyregex.h>

#define TRACE_ME
#include <SOM/TraceMacros.h>

using py::repl;
using std::regex, std::regex_search;
using std::string;
using fpath = std::filesystem::path;

bool Covbr2Html::convert(
    const string& covbrTxt,
    const std::string& odir,
    const bool wb, const bool hc, const bool fc)
{
    TRACE_FUNC_TIME()
    #define C_BEGIN "(?:^|(\\n))"
    #define C_FILE  "(?:\\w+:/?)?\\w+(?:/\\w+)*\\.(?:cpp|h):"

    //  txt file cleanup
    static const regex reFiles(
        C_BEGIN
        "((?:" C_FILE "\\r?\\n)*)"
        "("   C_FILE ")"
    );
    static const regex reFile(C_BEGIN "("  C_FILE ")");
    static const regex reTail(C_FILE "\\s+$");

    // html conversion
    static const regex reAmp("&");
    static const regex reLt("<");
    static const regex reGt(">");

    #define C_BEGIN_B C_BEGIN "( *)"
    // OK cases & replacements
    static const regex re_tf(C_BEGIN_B "(TF|tf)\\b(.*)");
    static const regex re_X (C_BEGIN_B "X\\b(.*)");

    const CONST_C_STRING rep_tf     = hc ? "$1<i>$2<u>$3</u>$4</i>" : "$1$2<u>$3</u>$4";
    const CONST_C_STRING rep_X      = hc ? "$1<i>$2 $3</i>" : "$1$2 $3";
    const CONST_C_STRING rep_Files  = hc ? "$1<i>$2</i><em>$3</em>" : "$1$2<em>$3</em>";

    // highlighting clean up
    static const regex re_ie ("<i></i>");
    static const regex re_ib ("(\\r)?\\n</i>");

    #define C_BEGIN_NOK C_BEGIN_B "--&gt;"
    #define C_CONT "(?: (.*))?"
    // NOK cases & replacements
    static const regex re_x(C_BEGIN_NOK C_CONT);
    static const regex re_t(C_BEGIN_NOK "t" C_CONT);
    static const regex re_f(C_BEGIN_NOK "f" C_CONT);
    static const regex re_T(C_BEGIN_NOK "T" C_CONT);
    static const regex re_F(C_BEGIN_NOK "F" C_CONT);

    #define C_SPAN "$1<b>$2"
    static const CONST_C_STRING rep_x  = C_SPAN "<s>X</s>   $3</b>";
    static const CONST_C_STRING rep_t  = C_SPAN "<u>t</u><s>f</s>   $3</b>";
    static const CONST_C_STRING rep_f  = C_SPAN "<s>t</s><u>f</u>   $3</b>";
    static const CONST_C_STRING rep_T  = C_SPAN "<u>T</u><s>F</s>   $3</b>";
    static const CONST_C_STRING rep_F  = C_SPAN "<s>T</s><u>F</u>   $3</b>";

    //  file extension
    static const regex reExt("\\.\\w+$");

    string buff;
    bool ok = read(buff, covbrTxt);
    // if (ok and regex_search(buff, reFile))
    if (ok)
    {
        try
        {
            TRACE_FLOW_TIME(processing file)
            const bool fWb = not odir.empty();
            const auto opath = fWb ? fpath(odir) : fpath(covbrTxt).parent_path();
            const auto fname = fpath(covbrTxt).filename().string();
            std::ofstream os;
            string rep;
            if (fc)
            {
                rep = buff;
            }
            else
            {
                TRACE_FLOW_TIME(clean txt)
                rep = repl(reTail, "", repl(reFiles, "$1$3", buff));
            }
            //  if anything left
            if (regex_search(rep, reFile))
            {
                //  write text file if changed or output directory specified
                if (wb and (fWb or rep != buff))
                {
                    TRACE_FLOW_TIME(re-write source)
                    if (open(os, opath / fname))
                    {
                        os << rep;
                        os.close();
                    }
                }
                {
                    TRACE_FLOW_TIME(convert to html)
                    //  html escape
                    rep = repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", rep)));

                    if (fc)
                    {
                        rep = repl(reFiles, rep_Files, rep);
                        if (hc)
                        {
                            rep = repl(re_ib, "</i>$1\n", repl(re_ie, "", rep));
                        }
                    }
                    else {
                        rep = repl(reFile, "$1<em>$2</em>", rep);
                    }
                    rep =   repl(re_tf, rep_tf,
                            repl(re_X,  rep_X,
                            repl(re_x,  rep_x,
                            repl(re_t,  rep_t,
                            repl(re_f,  rep_f,
                            repl(re_T,  rep_T,
                            repl(re_F,  rep_F, rep
                        )))))));
                }
                //  write html file
                {
                    TRACE_FLOW_TIME(write html)
                    const string ttl = repl(reExt, "", fname);
                    if (open(os, opath / repl(reExt, ".html", fname)))
                    {
                        os << cTtl << ttl << cHead << rep << cTail;
                        os.close();
                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << (e.what()) << '\n';
            ok = false;
        }
    }
    return ok;
}

const CONST_C_STRING Covbr2Html::cTtl =
    "<!DOCTYPE html>\n"
    "<html lang=en>\n"
    "<head>\n"
    "<title>";

const CONST_C_STRING Covbr2Html::cHead =
    "</title>\n"
    "<meta charset=\"UTF-8\">\n"
    "<style>\n"
    "* { font-style: normal; text-decoration: none; font-weight: normal;\n"
    "    font-family:Consolas,Consolas,Menlo,monospace;\n"
    "    margin: 0;\n"
    "    white-space:pre;\n"
    "}\n"
    "p { font-size:10pt; margin-left: 1em; margin-bottom: 2em; }\n"
    "u { color: blue; font-weight: bold; }\n"
    "s { color: red; font-weight: bold; }\n"
    "b { background-color: hsl(355,100%,91%); }\n"
    "i { background-color: hsl(120,100%,93%); }\n"
    "em { font-weight: bold; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>";

const CONST_C_STRING Covbr2Html::cTail = "</p></body></html>\n";
