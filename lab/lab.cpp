#include "Covbr2Html.h"
#include <SOM/BaseTypes.h>

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        Covbr2Html cov;
        cov.setHc();
        // cov.setFc();
        for (auto i = 1; i < argc; ++i)
        {
            cov.convert(argv[i]);
        }
    }
    return 0;
}
