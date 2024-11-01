#include <Covbr2Html.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <streambuf>

#define TRACE_ME
#include <trace.h>

using std::cout, std::cerr, std::endl;
using std::regex, std::regex_replace, std::regex_constants::extended;
using std::string;
using fpath = std::filesystem::path;

bool Covbr2Html::convert(const std::string& covbrTxt, const bool wb)
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

    string buff;
    bool ok = false;

    ok = read(buff, covbrTxt);
    string rep;
    if (ok)
    {
        {
            TRACE_FLOW(rep 1)
            rep = repl(reTail, "", repl(reSpc, "\n$1", repl(reDup, "\n$1$2", buff)));
        }
        //  write text file if changed
        if (wb and rep != buff)
        {
            TRACE_FLOW(write 1)
            std::ofstream os(covbrTxt);
            if (os.good())
            {
                TRACE( "-> " << covbrTxt)
                os << rep;
            }
            os.close();
        }
        {
            TRACE_FLOW(rep 2)
            rep =   repl(re_tf, rep_tf, 
                    repl(re_X,  rep_X,
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
            TRACE_FLOW(write 2)
            const string ttl = repl(reExt, "", fpath(covbrTxt).filename().string());
            const string covbrHtml = repl(reExt, ".html", covbrTxt);
            std::ofstream os(covbrHtml);
            if (os.good())
            {
                TRACE("-> " << covbrHtml)
                os << cTtl << ttl << cHead << rep << cTail;
            }
            os.close();
        }
    }
    else
    {
        TRACE("Error reading file " << covbrTxt)
    }
    return ok;
}

bool Covbr2Html::read(string& trg, const string& txtFile)
{
    TRACE_FUNC()
    TRACE("<- " << txtFile)
    std::ifstream is(txtFile);
    const bool ok = is.good();
    if (ok)
    {
        trg.assign((std::istreambuf_iterator<char>(is)),
            std::istreambuf_iterator<char>());
    }
    is.close();
    return ok;
}

// const CONST_C_STRING Covbr2Html::basename(const CONST_C_STRING fp)
// {
//     static const auto isDirSign = [](const CHAR c)
//     {
//         return ((c == '/') or (c == '\\'));
//     };

//     CONST_C_STRING ps = fp;
//     for (; *ps != 0; ++ps);
//     for (; (ps != fp) and (not isDirSign(*ps)); --ps);
//     if (isDirSign(*ps)) ++ps;
//     return ps;
// }

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
