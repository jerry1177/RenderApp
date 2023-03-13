#pragma once
#include "vulkan/vulkan.h"

typedef struct VkBuffer_T* VkBuffer;
namespace VEE {
	struct Vertex;
	enum class VBufferAttributeType {
		Float,
		Vec2,
		Vec3,
		Vec4
	};
	class VBufferAttribute
	{
	public:
		VBufferAttribute() = default;
		VBufferAttribute(VBufferAttributeType attribute);
		VkVertexInputAttributeDescription GetAttributeDescription() const { return m_AttributeDescription; }
		uint32_t GetSize() { return m_Size; }
		VkFormat GetFormat() { return m_AttributeDescription.format; }
		void SetLocation(uint32_t location) { m_AttributeDescription.location = location; }
		void SetBindingIndex(uint32_t bindingIndex) { m_AttributeDescription.binding = bindingIndex; }
		void SetOffset(uint32_t offset) { m_AttributeDescription.offset = offset; }
		void SetFormat(VkFormat format) { m_AttributeDescription.format = format; }
		static uint32_t GetSizeOf(VBufferAttributeType attribute);
		static VkFormat GetFormat(VBufferAttributeType attribute);
		void operator=(VBufferAttributeType attributeType);
	public:
		
	private:
		VkVertexInputAttributeDescription  m_AttributeDescription{};
		uint32_t m_Size;
		uint32_t m_Offset;
	};

	class VDevice;
	class VBufferLayout {
	public:
		VBufferLayout(const std::vector<VBufferAttribute>& attributes, uint32_t bindingIndex);
		VBufferLayout(const std::vector<VBufferAttributeType>& attributes, uint32_t bindingIndex);
		const VkVertexInputBindingDescription& GetBindingDescription() const { return m_BindingDescription; }
		const std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions() const { return m_AttributeDescriptions; }
	private:
		std::vector<VBufferAttribute> m_Attributes;
		uint32_t m_Stride;
		std::vector<VkVertexInputAttributeDescription>  m_AttributeDescriptions;
		VkVertexInputBindingDescription m_BindingDescription{};
	};
	class VCommandBuffer;
	
	class VVertexBuffer {
	public:
		VVertexBuffer() = default;
		
		VVertexBuffer(const VDevice* device, uint32_t size, uint32_t count);
		void Bind(const std::shared_ptr<VCommandBuffer>& commandBuffer);
		uint32_t GetCount() { return m_Count; }

		template<typename T>
		void CopyVerticesToGPU(const std::vector<T>& verticies);

		template<typename T>
		static VVertexBuffer* Create(const VDevice* device, const std::vector<T>& vertices);

		~VVertexBuffer();
	private:
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		uint32_t m_Count = 0;
		uint32_t m_Size = 0;
		VkBuffer m_VertexBuffer = nullptr;
		VkDeviceMemory m_VertexBufferMemory = nullptr;
		const VDevice* m_Device = nullptr;
	};

	template<typename T>
	void VVertexBuffer::CopyVerticesToGPU(const std::vector<T>& verticies)
	{
		void* data;
		vkMapMemory(m_Device->GetLogicalDeviceHandle(), m_VertexBufferMemory, 0, m_Size, 0, &data);
		memcpy(data, verticies.data(), (size_t)m_Size);
		vkUnmapMemory(m_Device->GetLogicalDeviceHandle(), m_VertexBufferMemory);

	}

	template<typename T>
	VVertexBuffer* VVertexBuffer::Create(const VDevice* device, const std::vector<T>& vertices)
	{
		return new VVertexBuffer(device, sizeof(T) * vertices.size(), vertices.size());
	}

}