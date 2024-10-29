#include <Covbr2Html.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <streambuf>

using std::cout, std::endl;
using std::regex, std::regex_replace, std::regex_constants::extended;
using std::string;
using fpath = std::filesystem::path;

bool Covbr2Html::convert(const CONST_C_STRING covbrTxt)
{
    #define C_BEGIN "(?:^|(\\n))"
    #define C_ECLIP "(?: +\\.\\.\\.\\n)?"
    #define C_FILE  "(?:\\w+:/?)?\\w+(?:/\\w+)*\\.(?:cpp|h):"

    //  txt file cleanup
    static const regex reDouble(
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
    static const regex re_tf(C_BEGIN_B "(TF|tf)(?: (.*))?");
    static const regex re_X (C_BEGIN_B "X  (.*)");

    #define C_SPAN_OK  "$1<span>$2"
    static const CONST_C_STRING rep_tf = C_SPAN_OK "<u>$3</u> $4</span>";
    static const CONST_C_STRING rep_X  = C_SPAN_OK "<u>&gt;&gt;</u> $3</span>";
    
    #define C_BEGIN_NOK C_BEGIN_B "--&gt;"
    // NOK cases & replacements
    static const regex re_x(C_BEGIN_NOK " (.*)");
    static const regex re_t(C_BEGIN_NOK "t (.*)");
    static const regex re_f(C_BEGIN_NOK "f (.*)");
    static const regex re_T(C_BEGIN_NOK "T (.*)");
    static const regex re_F(C_BEGIN_NOK "F (.*)");

    #define C_SPAN_NOK "$1<span class=x>$2"
    static const CONST_C_STRING rep_x  = C_SPAN_NOK "<s>X</s>   $3</span>";
    static const CONST_C_STRING rep_t  = C_SPAN_NOK "<u>t</u><s>f</s>   $3</span>";
    static const CONST_C_STRING rep_f  = C_SPAN_NOK "<s>t</s><u>f</u>   $3</span>";
    static const CONST_C_STRING rep_T  = C_SPAN_NOK "<u>T</u><s>F</s>   $3</span>";
    static const CONST_C_STRING rep_F  = C_SPAN_NOK "<s>T</s><u>F</u>   $3</span>";

    //  file extension
    static const regex reExt("\\.\\w+$");

    string buff;
    const bool ok = read(buff, covbrTxt);
    if (ok)
    {
        string rep = repl(reTail, "", repl(reSpc, "\n$1", repl(reDouble, "\n$1$2", buff)));

        //  TODO:  write file if changed
        // if (rep != buff)
        // {
        //     std::ofstream os(covbrTxt);
        //     if (os.good())
        //     {
        //         cout << "-> " << covbrTxt << endl;
        //         os << rep;
        //     }
        //     os.close();
        // }

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

        const string ttl = repl(reExt, "", basename(covbrTxt));
        const string out = repl(reExt, ".html", covbrTxt);

        {
            std::ofstream os(out);
            if (os.good())
            {
                cout << "-> " << out << endl;
                os << cTtl << ttl << cHead << rep << cTail;
            }
            os.close();
        }
    }
    else
    {
        cout << "Error reading file " << covbrTxt << endl;
    }
    return ok;
}

bool Covbr2Html::read(string& trg, const CONST_C_STRING txtFile)
{
    //  read text file into string
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

const CONST_C_STRING Covbr2Html::basename(const CONST_C_STRING fp)
{
    static const auto isDirSign = [](const CHAR c)
    {
        static const CHAR cSlash = '/';
        static const CHAR cBack  = '\\';
        return ((c == cSlash) or (c == cBack));
    };

    CONST_C_STRING ps = fp;
    for (; *ps != 0; ++ps);
    for (; (ps != fp) and (not isDirSign(*ps)); --ps);
    if (isDirSign(*ps)) ++ps;
    return ps;
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
    // "span { background-color: hsl(120,100%,93%); }\n"
    "span > u { color: blue; font-weight: bold; }\n"
    "span.x { background-color: hsl(355,100%,91%); }\n"
    "span > s { color: red; font-weight: bold; }\n"
    "em { font-weight: bold; font-size:12pt; }\n"
    "span > s { color: red; font-weight: bold; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>";

const CONST_C_STRING Covbr2Html::cTail = "</p></body></html>";
