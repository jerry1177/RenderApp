#pragma once

typedef struct VkFence_T* VkFence;
typedef uint32_t VkFlags;
typedef VkFlags VkFenceCreateFlags;
namespace VEE {
	class VDevice;
	class VFence
	{
	public:
		VFence(VDevice*, VkFenceCreateFlags = 0);
		const VkFence& GetHandle() { return m_Fence; }
		~VFence();
		
	private:
		VDevice* m_Device = nullptr;
		VkFence  m_Fence = nullptr;
	};
}

