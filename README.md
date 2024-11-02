# covbr2html

Bullseye code coverage provides a great tool: **covbr**

```shell
$>covbr -h
BullseyeCoverage Detail Report 9.11.3 Windows License 123456
Copyright (c) Bullseye Testing Technology
Usage: covbr [options] [region ...]
Show low-level coverage measurements with annotated source code
-a     --all          Show all source lines
-cN    --context N    Show N source lines before or after a probe [N=3]
-dDir  --srcdir Dir   Source file directory is Dir
-eN    --tab N        Expand tabs to every N column [N=8]
-fPath --file Path    Coverage file is Path
-h     --help         Display this usage summary
-i     --no-time      Ignore source file date/time
-oPath --output Path  Write output to Path instead of stdout
-q     --quiet        Quiet
-u     --uncover      Suppress fully covered probes (with -c)
-v     --verbose      Verbose
-wN    --width N      Report width is N columns
--csv                 Output comma separated values
--html                Output in HTML format
--no-banner           Suppress startup banner
```

especially the **-u** option is what one needs:

report only snippets containing uncovered parts

but the tool needs improvements:
-   it reports all files - regardless if containing uncovered bits
-   the **--html** option is a joke: just plain text wrapped in a pre

**covbr2html** improves on this:
-   removes fully covered files from listing
-   generates real highlighted stand-alone html 

## C++ / Python
-   use covbr2html.py in the [py folder](py)
-   build stand-alone executable from C++ code - see [make folder](make)



