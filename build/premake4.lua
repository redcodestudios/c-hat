#!lua

solution "c-hat"
    configurations { "linux", "debug", "release"}

project "c-hat"
    kind "ConsoleApp"
    language "C"
    targetdir "bin"

    files { "../src/inc/**.h,", "../src/**.c" }

    configuration "debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "release"
        defines { "NODEBUG" }
        flags { "Optimize" }
    
    configuration "linux"
        buildoptions {"-pthread", "-lGL", "-lm", "-lrt", "-Wall", "-Wextra"}
        linkoptions { "-pthread", "-lrt" }
