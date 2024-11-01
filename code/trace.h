#pragma once
#ifndef TRACE_H
#define TRACE_H


#if defined(TRACE_ALL) || (defined(TRACE_ON) && defined(TRACE_ME))

#include <iostream>
#include <iomanip>
#define TRACE(MSG) std::cout << MSG << '\n';
#define TRACE_ERR(MSG) std::cerr << MSG << '\n';
#define TRACE_VAR(VAR) TRACE(#VAR << ": " << VAR)

#include <chrono>

using _TracePoint = std::chrono::time_point<std::chrono::steady_clock>;

struct T_Trace 
{
    const char* const mSection;
    const _TracePoint mStart;
    inline T_Trace(const char* section):
        mSection(section),
        mStart(std::chrono::steady_clock::now())
    {}
    inline ~T_Trace()
    {
        const _TracePoint end = std::chrono::steady_clock::now();
        TRACE(std::setw(4) << std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count() << ": " << mSection);
    }    
};
//  compiler specifica
#if defined(COMPILER_CADUL)
    #define TRACE_FUNC()
#elif defined(_MSC_VER)
    #define TRACE_FUNC() T_Trace __T_TRACE__ ## __LINE__ (__FUNCTION__);
#else
    #define TRACE_FUNC() T_Trace __T_TRACE__ ## __LINE__ (__func__);
#endif

#define TRACE_FLOW(WHATEVER) T_Trace __T_TRACE__ ## __LINE__ (#WHATEVER);

#else
#define TRACE(MSG)
#define TRACE_ERR(MSG)
#define TRACE_VAR(VAR)
#define TRACE_FUNC()
#define TRACE_FLOW(WHATEVER)
#endif

#endif // _H