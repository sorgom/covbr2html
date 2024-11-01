#include <Covbr2Html.h>
#include <glob.h>
#include <iostream>

#define TRACE_ME
#include <trace.h>

class CovbrGlobber : public I_GlobProcessor
{
public:
    void process(const CONST_C_STRING item) override
    {
        if (not cov.convert(item))
        {
            _ret = 1;
        }
    }
    inline INT32 ret() const { return _ret; }
private:
    Covbr2Html cov;
    INT32 _ret = 0;  
};

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC()
    CovbrGlobber globber;

    for (INT32 i = 1; (i < argc) and (globber.ret() == 0); ++i)
    {
        glob(argv[i], globber);
    }
    if (globber.ret() != 0)
    {
        std::cerr << "Error converting files" << '\n';
    }
    return globber.ret();
}
