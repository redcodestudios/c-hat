#!lua

solution "dummy-user"
    configurations { "linux" }

project "dummy-user"
    kind "ConsoleApp"
    language "C"
    targetdir "bin"

    files { "../src/inc/**.h", "**.c", "../src/**.c" }
    excludes { "../src/main.c" }
    includedirs { "../src/inc/"}
    
    configuration "linux"
        buildoptions {"-pthread", "-lGL", "-lm", "-lrt", "-Wall", "-Wextra"}
        linkoptions {"-pthread", "-lrt" }