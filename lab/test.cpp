#include <RegexBuffer.h>

#include <iostream>
using std::cout, std::endl;
#include <string>

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    RegexBuffer& rb = RegexBuffer::instance();
    if (argc > 1)
    {
        const std::regex re1("(\\n *)-->");
        const std::regex re2("(#include <)\\w+(\\.h>)");
        cout << "Reading " << argv[1] << endl;
        if (rb.read(argv[1]))
        {
            std::string s = rb.str();
            cout << "replacing" << endl;
            if (
                rb.repl(re1, "$1??? replaced") and
                rb.repl(re2, "$1Wumpel$2")   
            ) 
            {
                if (s != rb.str())
                {
                    cout << "Writing tmp.txt" << endl;
                    rb.write("tmp.txt");
                }
                rb.write("tmp.txt");
                // cout << rb.str() << endl;
            }
            
        }
    } 

    return 0;
}