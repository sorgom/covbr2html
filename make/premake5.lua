--  ============================================================
--  premake5 build rules for covbr2html
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /O2 /Ot /wd4100 /wd4103'
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

workspace 'covbr2html'

    configurations { 'ci', 'verbose' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/obj/%{_PROJECT}}/%{_TARGET_OS}/%{cfg.name}'
    defines { 'NDEBUG' }
    optimize 'Speed'
    kind 'ConsoleApp'

    includedirs { '../code' }

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }

    filter { 'configurations:verbose' }
        defines { 'VERBOSE' }

    project 'covbr2html'
        files { '../code/*.cpp' }
  
    -- project 'lab'
    --     files { '../lab/*.cpp' }
