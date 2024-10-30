#include <Covbr2Html.h>
#include <glob.h>

INT32 ret = 0;

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    globFunc func = [](const CONST_C_STRING item)
    {
        if (not Covbr2Html::convert(item))
        {
            ret = 1;
        }
    };
    for (INT32 i = 1; (i < argc) and (ret == 0); ++i)
    {
        glob(argv[i], func);
    }
    return ret;
}
