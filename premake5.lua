APP_NAME = "popcnt"

workspace "popcnt"
    location "build"
    configurations { "Debug", "Release" }

architecture "x86_64"

project "popcnt"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/"
    systemversion "latest"
    
    files { "src/**.h", "src/**.cpp" }
    includedirs { "src" }

    symbols "On"
    debugargs {"--break"}

    filter {"Debug"}
        targetname ("%{APP_NAME}_Debug")
        optimize "Off"
    filter {"Release"}
        targetname ("%{APP_NAME}")
        optimize "Full"
        
