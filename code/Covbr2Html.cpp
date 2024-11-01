#include <Covbr2Html.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>

#define TRACE_ME
#include <trace.h>

using std::cout, std::cerr, std::endl;
using std::regex, std::regex_replace, std::regex_constants::extended;
using std::string;
using fpath = std::filesystem::path;


bool Covbr2Html::convert(const CONST_C_STRING covbrTxt)
{
    TRACE_FUNC()
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
    static const regex re_tf(C_BEGIN_B "(TF|tf)");
    static const regex re_X (C_BEGIN_B "X\\b");

    static const CONST_C_STRING rep_tf = "$1$2<u>$3</u>";
    static const CONST_C_STRING rep_X  = "$1$2 ";
    
    #define C_BEGIN_NOK C_BEGIN_B "--&gt;"
    #define C_CONT "(?:$| (.*))"
    // NOK cases & replacements
    static const regex re_x(C_BEGIN_NOK C_CONT);
    static const regex re_t(C_BEGIN_NOK "t" C_CONT);
    static const regex re_f(C_BEGIN_NOK "f" C_CONT);
    static const regex re_T(C_BEGIN_NOK "T" C_CONT);
    static const regex re_F(C_BEGIN_NOK "F" C_CONT);

    #define C_SPAN "$1<span>$2"
    static const CONST_C_STRING rep_x  = C_SPAN "    $3</span>";
    static const CONST_C_STRING rep_t  = C_SPAN "<u>t</u><s>f</s>   $3</span>";
    static const CONST_C_STRING rep_f  = C_SPAN "<s>t</s><u>f</u>   $3</span>";
    static const CONST_C_STRING rep_T  = C_SPAN "<u>T</u><s>F</s>   $3</span>";
    static const CONST_C_STRING rep_F  = C_SPAN "<s>T</s><u>F</u>   $3</span>";

    //  file extension
    static const regex reExt("\\.\\w+$");

    bool ok = 
        mRb.read(covbrTxt) and
        mRb.repl(reDup, "\n$1$2") and
        mRb.repl(reSpc, "\n$1") and
        mRb.repl(reTail, "") and
        mRb.repl(reAmp, "&amp;") and 
        mRb.repl(reLt,  "&lt;") and
        mRb.repl(reGt,  "&gt;") and
        mRb.repl(reFile, "$1<em>$2</em>") and
        mRb.repl(re_x,  rep_x) and
        mRb.repl(re_t,  rep_t) and
        mRb.repl(re_f,  rep_f) and
        mRb.repl(re_T,  rep_T) and
        mRb.repl(re_F,  rep_F) and
        mRb.repl(re_tf, rep_tf) and
        mRb.repl(re_X,  rep_X);

    if (ok)
    {
        //  write html file
        const string ttl = repl(reExt, "", fpath(covbrTxt).filename().string());
        const string covbrHtml = repl(reExt, ".html", covbrTxt);
        std::ofstream os(covbrHtml);
        ok = os.good();
        if (ok)
        {
            TRACE("-> " << covbrHtml)
            os << cTtl << ttl << cHead << mRb.str() << cTail;
        }
        os.close();
    }

    if (not ok)
    {
        TRACE_ERR("Error converting " << covbrTxt)
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
    "* { font-style: normal; text-decoration: none;\n"
    "    font-family:Consolas,Consolas,Menlo,monospace;\n"
    "    margin: 0;\n"
    "    white-space:pre;\n"
    "}\n"
    "p { font-size:10pt; margin-left: 1em; margin-bottom: 2em; }\n"
    "u { color: blue; font-weight: bold; }\n"
    "s { color: red; font-weight: bold; }\n"
    "span { background-color: hsl(355,100%,91%); }\n"
    "em { font-weight: bold; font-size:12pt; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>";

const CONST_C_STRING Covbr2Html::cTail = "</p></body></html>";
