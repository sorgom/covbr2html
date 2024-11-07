#include <Covbr2Html.h>
#include <SOM/pyregex.h>
#include <SOM/fio.h>
#include <filesystem>
#include <fstream>

#define TRACE_ME
#include <SOM/TraceMacros.h>

using py::repl;
using std::regex, std::regex_search;
using std::string;
using fpath = std::filesystem::path;

bool Covbr2Html::convert(const string& covbrTxt, const std::string& odir, const bool wb, const bool hc)
{
    #define C_BEGIN "(?:^|(\\n))"
    #define C_ECLIP "(?: +\\.\\.\\.\\n)?"
    #define C_FILE  "(?:\\w+:/?)?\\w+(?:/\\w+)*\\.(?:cpp|h):"

    //  txt file cleanup
    static const regex reDup(
        C_BEGIN
        C_ECLIP
        "(?:" C_FILE "\\n)*"
        "("   C_FILE ")"
    );
    static const regex reSpc("\\s+(\\n" C_FILE ")");
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

    static const CONST_C_STRING rep_tf = "$1$2<u>$3</u>$4";
    static const CONST_C_STRING rep_X  = "$1$2 $3";
    static const CONST_C_STRING rep_tf_hc = "$1<i>$2<u>$3</u>$4</i>";
    static const CONST_C_STRING rep_X_hc  = "$1<i>$2 $3</i>";

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
    const bool ok = read(buff, covbrTxt);
    if (ok and regex_search(buff, reFile))
    {
        const bool fWb = not odir.empty();
        const auto opath = fWb ? fpath(odir) : fpath(covbrTxt).parent_path();
        const auto fname = fpath(covbrTxt).filename().string();

        string rep;
        {
            TRACE_FLOW_TIME(clean txt)
            rep = repl(reTail, "", repl(reSpc, "\n$1", repl(reDup, "\n$1$2", buff)));
        }
        //  if anything left
        if (regex_search(rep, reFile))
        {
            //  write text file if changed or output directory specified
            if (wb and (fWb or rep != buff))
            {
                TRACE_FLOW_TIME(re-write source)
                auto ofile = opath / fname;
                std::ofstream os(ofile);
                if (checkos(os, ofile.string()))
                {
                    os << rep;
                }
                os.close();
            }
            {
                TRACE_FLOW_TIME(convert to html)
                rep =   repl(re_tf, hc ? rep_tf_hc : rep_tf,
                        repl(re_X,  hc ? rep_X_hc  : rep_X,
                        repl(re_x,  rep_x,
                        repl(re_t,  rep_t,
                        repl(re_f,  rep_f,
                        repl(re_T,  rep_T,
                        repl(re_F,  rep_F,
                        repl(reFile, "$1<em>$2</em>",
                        repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", rep)
                    ))))))))));
            }
            //  write html file
            {
                TRACE_FLOW_TIME(write html)
                const string ttl = repl(reExt, "", fname);
                const auto ofile = opath / repl(reExt, ".html", fname);
                std::ofstream os(ofile);
                if (checkos(os, ofile.string()))
                {
                    os << cTtl << ttl << cHead << rep << cTail;
                }
                os.close();
            }
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
    "em { font-weight: bold; font-size:12pt; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>";

const CONST_C_STRING Covbr2Html::cTail = "</p></body></html>\n";
