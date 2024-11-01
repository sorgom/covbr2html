#pragma once
#ifndef TRACE_H
#define TRACE_H

#include <iostream>

#if defined(TRACE_ALL) || (defined(TRACE_ON) && defined(TRACE_ME))
#define TRACE(MSG) std::cout << MSG << '\n';
#define TRACE_ERR(MSG) std::cerr << MSG << '\n';
#define TRACE_VAR(VAR) TRACE(#VAR << ": " << VAR)
#else
#define TRACE(MSG)
#define TRACE_ERR(MSG)
#define TRACE_VAR(VAR)
#endif

#endif // _H