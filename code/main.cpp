#include <Covbr2Html.h>


int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    INT32 ret = 0;
    for (INT32 i = 1; i < argc; ++i)
    {
        if (not Covbr2Html::convert(argv[i]))
        {
            ret = 1;
            break;
        }
    }
    return ret;
}
