project "CEGUI-0.8.7"
	language "C++"
	kind "StaticLib"
	targetname "CEGUI-0.8.7"
	warnings "Off"

	includedirs {
		"include",
		"src/tinyxml",
		"../freetype/include"
	}

	links { "freetype" }

	defines {
		"CEGUIBASE_EXPORTS",
		"CEGUI_STATIC",
		"STATIC_BUILD"
	}

	vpaths {
		["Headers/*"] = "include/**.h",
		["Sources/*"] = "src/**.cpp",
		["Sources/*"] = "src/**.c",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"src/**.cpp",
		"src/**.c",
		"include/**.h",
	}

	excludes {
		"src/minibidi.cpp",
		"src/RendererModules/**",
		"src/ScriptModules/**",
		"src/ImageCodecModules/**",
		"src/PCRERegexMatcher.cpp",
		"src/IconvStringTranscoder.cpp",
		"src/FribidiVisualMapping.cpp",
		"src/MinizipResourceProvider.cpp",
		"src/implementations/mac/**"
	}

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }

	filter {"system:windows"}
		linkoptions { "/ignore:4221" }
		disablewarnings { "4221" }
