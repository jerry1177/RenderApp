#pragma once

typedef struct VkSemaphore_T* VkSemaphore;

namespace VEE {
	class VDevice;
	class VSemaphore
	{
	public:
		VSemaphore(VDevice*);
		VkSemaphore GetHandle() { return m_Semaphore; }
		~VSemaphore();
	private:
		VDevice* m_Device = nullptr;
		VkSemaphore m_Semaphore = nullptr;
	};
}
