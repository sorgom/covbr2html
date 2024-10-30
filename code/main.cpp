#include <Covbr2Html.h>
#include <glob.h>
#include <iostream>

INT32 ret = 0;

void covFunc(const CONST_C_STRING item)
{
    if (not Covbr2Html::convert(item))
    {
        ret = 1;
    }
}

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    for (INT32 i = 1; (i < argc) and (ret == 0); ++i)
    {
        glob(argv[i], &covFunc);
    }
    if (ret != 0)
    {
        std::cerr << "Error converting files" << '\n';
    }
    return ret;
}
