workspace "RenderApp"
    configurations {"Debug", "Release"}
    architecture "x86_64"
    startproject "RenderApp"
    VULKAN_SDK = os.getenv("VULKAN_SDK")
    
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    IncludeDir = {}
    IncludeDir["GLFW"] = "%{wks.location}/Dependencies/GLFW/include"
    
    IncludeDir["GLAD"] = "%{wks.location}/Dependencies/glad/include"
    IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
    IncludeDir["glm"] = "Dependencies/glm"
    LibraryDir = {}

    LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

    Library = {}
    Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
    Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

    Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
    Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
    Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

    Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
    Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
    Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

    group "Dependencies"
        include "Dependencies/GLFW"
    group ""
    

project "RenderApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    pchheader "Renderpch.h"
	pchsource "%{prj.name}/Renderpch.cpp"

    includedirs
	{
		"%{IncludeDir.GLFW}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.glm}"
		
	}
    links
	{
		
		"GLFW",
        "%{Library.Vulkan}",
        "%{Library.VulkanUtils}"
	}

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/**.h", "%{prj.name}/**.cpp", 
        "Dependencies/glm/glm/**.hpp",
        "Dependencies/glm/glm/**.inl",
    }
    defines {"_GLFW_"}
    

    filter { "configurations:Debug" }
      defines { "DEBUG" }
      runtime "Debug"
      symbols "On"
      links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

   filter { "configurations:Release" }
      defines { "NDEBUG" }
      runtime "Release"
      optimize "On"
      links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
