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
		"STATIC_BUILD",
		"CEGUI_BUILD_STATIC_FACTORY_MODULE"
	}

	vpaths {
		["Headers/*"] = { "include/**.h", "src/**.inl" },
		["Sources/*"] = "src/**.cpp",
		["Sources/*"] = "src/**.c",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"src/**.cpp",
		"src/**.c",
		"src/**.inl",
		"include/**.h",
	}

	excludes {
		"src/minibidi.cpp",
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
		includedirs {
			path.join(dxdir, "Include")
		}
		libdirs {
			path.join(dxdir, "Lib/x86")
		}
		links { "d3d9", "d3dx9" }
		linkoptions { "/ignore:4221", "/ignore:4006" }
		disablewarnings { "4221", "4006" }
