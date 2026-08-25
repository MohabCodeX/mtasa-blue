project "GUI"
	language "C++"
	kind "SharedLib"
	targetname "cgui"
	targetdir(buildpath("mta"))
	clangtidy "On"

	filter "system:windows"
		includedirs { "../../vendor/sparsehash/src/windows" }

	filter {}
		includedirs {
			"../../Shared/sdk",
			"../sdk",
			"../../vendor/sparsehash/src/"
		}

	if _OPTIONS["with-cegui-next"] then
		defines {
			"MTA_USE_CEGUI_NEXT",
			"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
		}
		includedirs {
			"../../vendor/cegui-0.8.7/include",
			"../../vendor/cegui-0.8.7/include/CEGUI/RendererModules/Direct3D9",
			"../../vendor/freetype/include",
		}
		links {
			"CEGUI-0.8.7",
			"freetype",
			"d3d9.lib",
			"d3dx9.lib",
			"dxerr.lib"
		}
	else
		defines {
			"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
		}
		includedirs {
			"../../vendor/cegui-0.4.0-custom/include",
		}
		links {
			"CEGUI", "DirectX9GUIRenderer", "Falagard",
			"d3dx9.lib",
			"dxerr.lib"
		}
	end

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"*.h",
		"*.cpp"
	}

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
