--  ============================================================
--  premake5 build rules for covbr2html
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /O2 /Ot /wd4100 /wd4103'
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

workspace 'covbr2html'

    configurations { 'ci' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/obj/%{_TARGET_OS}/%{cfg.name}'
    
    includedirs { '../code' }
    files { '../code/*.cpp' }

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }

    filter { 'configurations:ci' }
        defines { 'NDEBUG' }
        optimize 'Speed'
        
    -- filter { 'configurations:debug' }
    --     defines { 'DEBUG' }
    --     symbols 'On'

    project 'covbr2html'
        kind 'ConsoleApp'
   
