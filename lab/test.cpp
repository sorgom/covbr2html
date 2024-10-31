#include <RegexBuffer.h>

#include <iostream>
using std::cout, std::endl;


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
            cout << "replacing" << endl;
            if (
                rb.repl(re1, "$1???") and
                rb.repl(re2, "$1Wumpel$2")   
            )
                cout << rb.str() << '\n';
        }
    } 

    return 0;
}