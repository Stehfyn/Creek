project "WalnutApp"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../Walnut/src",

      --"%{IncludeDir.VulkanSDK}",
	  "%{IncludeDir.OpenGL}",
      "%{IncludeDir.glm}",
	  "%{IncludeDir.assimp}",
   }

    links
    {
        "Walnut",
		"opengl32.lib",
		"glew32.lib",
    }

	libdirs
	{
	    "../vendor/GL/Lib",
		"../vendor/assimp/lib",
	}

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
	  links
	  {
	      "assimp-vc143-mtd.lib",
	  }
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      links
      {
          "assimp-vc143-mt.lib",
      }
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
