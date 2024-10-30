#pragma once
#ifndef GLOB_H
#define GLOB_H

#include <BaseTypes.h>
#include <regex>

using globFunc = void(*)(const CONST_C_STRING fpath);

//!  glob files
//!  @param item file, or directory with file glob pattern
//!  @param func file processing function
void glob(const CONST_C_STRING item, globFunc func);

#endif // _H