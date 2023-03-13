#include "Renderpch.h"
#include "Buffer.h"
#include "VDevice.h"
#include "CommandBuffer.h"
#include "Renderer/VRenderer.h"
#include "glm/glm.hpp"

namespace VEE {
	VBufferAttribute::VBufferAttribute(VBufferAttributeType attribute)
	{
		m_AttributeDescription.format = GetFormat(attribute);
		m_Size = GetSizeOf(attribute);
	}

		
	uint32_t VBufferAttribute::GetSizeOf(VBufferAttributeType attribute)
	{
		switch (attribute)
		{
		case VBufferAttributeType::Float:
			return sizeof(float);
			
		case VBufferAttributeType::Vec2:
			return sizeof(glm::vec2);
			
		case VBufferAttributeType::Vec3:
			return sizeof(glm::vec3);
			
		case VBufferAttributeType::Vec4:
			return sizeof(glm::vec4);
			
		default:
			ASSERT(false, "Invalid VBufferAttribute");
			break;
		}
	}

	
	VkFormat VBufferAttribute::GetFormat(VBufferAttributeType attribute)
	{
		switch (attribute)
		{
		case VBufferAttributeType::Float:
			return VK_FORMAT_R32_SFLOAT;

		case VBufferAttributeType::Vec2:
			return VK_FORMAT_R32G32_SFLOAT;

		case VBufferAttributeType::Vec3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case VBufferAttributeType::Vec4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		default:
			ASSERT(false, "Invalid VBufferAttribute");
			break;
		}
	}

	void VBufferAttribute::operator=(VBufferAttributeType attributeType)
	{
		m_Size = GetSizeOf(attributeType);
		m_AttributeDescription.format = GetFormat(attributeType);

	}

	VBufferLayout::VBufferLayout(const std::vector<VBufferAttribute>& attributes, uint32_t bindingIndex)
	{
		m_Stride = 0;
		m_Attributes.resize(attributes.size());
		m_AttributeDescriptions.resize(attributes.size());
		uint32_t offSet = 0;
		for (uint32_t i = 0; i < attributes.size(); i++) {
			m_Attributes[i].SetBindingIndex(bindingIndex);
			m_Attributes[i].SetLocation(i);
			m_AttributeDescriptions[i].offset = offSet;
			offSet += m_Attributes[i].GetSize();
			m_Stride += m_Attributes[i].GetSize();
			m_AttributeDescriptions[i] = m_Attributes[i].GetAttributeDescription();
		}

		m_BindingDescription.binding = bindingIndex;
		m_BindingDescription.stride = m_Stride;
		m_BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}

	VBufferLayout::VBufferLayout(const std::vector<VBufferAttributeType>& attributesTypes, uint32_t bindingIndex)
	{
		m_Stride = 0;
		m_Attributes.resize(attributesTypes.size());
		m_AttributeDescriptions.resize(attributesTypes.size());
		uint32_t offSet = 0;
		for (uint32_t i = 0; i < attributesTypes.size(); i++) {
			m_Attributes[i] = attributesTypes[i];
			m_Attributes[i].SetBindingIndex(bindingIndex);
			m_Attributes[i].SetLocation(i);
			m_Attributes[i].SetOffset(offSet);
			offSet += m_Attributes[i].GetSize();
			m_Stride += m_Attributes[i].GetSize();
			m_AttributeDescriptions[i] = m_Attributes[i].GetAttributeDescription();
		}

		m_BindingDescription.binding = bindingIndex;
		m_BindingDescription.stride = m_Stride;
		m_BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}
		
	VVertexBuffer::VVertexBuffer(const VDevice* device, uint32_t size, uint32_t count) : m_Device(device), m_Size(size), m_Count(count)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_Size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(device->GetLogicalDeviceHandle(), &bufferInfo, nullptr, &m_VertexBuffer);
		ASSERT(result == VK_SUCCESS, "failed to Allocate VertexBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device->GetLogicalDeviceHandle(), m_VertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		vkAllocateMemory(m_Device->GetLogicalDeviceHandle(), &allocInfo, nullptr, &m_VertexBufferMemory);
		ASSERT(m_VertexBufferMemory != nullptr, "failed to allocate Vertex Buffer Memory!");

		vkBindBufferMemory(m_Device->GetLogicalDeviceHandle(), m_VertexBuffer, m_VertexBufferMemory, 0);
	}
	
	void VVertexBuffer::Bind(const std::shared_ptr<VCommandBuffer>& commandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer->GetHandle(), 0, 1, vertexBuffers, offsets);

	}

	VVertexBuffer::~VVertexBuffer()
	{
		vkDestroyBuffer(m_Device->GetLogicalDeviceHandle(), m_VertexBuffer, nullptr);
		vkFreeMemory(m_Device->GetLogicalDeviceHandle(), m_VertexBufferMemory, nullptr);
	}
	
	uint32_t VVertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_Device->GetPhysicalDeviceHandle(), &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & properties) == properties)) {
				return i;
			}
		}
		ASSERT(false, "failed to find Suitable memory type!");


	}

}
