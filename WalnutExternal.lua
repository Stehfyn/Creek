-- WalnutExternal.lua

--VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
--IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["OpenGL"] = "../vendor/GL"
IncludeDir["glm"] = "../vendor/glm"
IncludeDir["assimp"] = "../vendor/assimp/include"

LibraryDir = {}
--LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["OpenGL"] = "../vendor/GL/Lib"
--LibraryDir["assimp"] = "../vendor/assimp/lib/" --gotta do per config yucky

Library = {}
--Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["OpenGL"] = "%{LibraryDir.OpenGL}/glew32.lib"
--Library["assimp"] = "%{LibraryDir.assimp}/assimp-vc143-mtd.lib"

group "Dependencies"
   include "vendor/imgui"
   include "vendor/glfw"
   externalproject "assimp"
       location "vendor/assimp/code"
	   uuid "35F5D00A-3DBD-35A7-8EFF-93D0C8DC6A08"
	   kind "StaticLib"
	   language "C++"
   --include "vendor/Creek"

group ""

group "Core"
include "Walnut"
group ""
