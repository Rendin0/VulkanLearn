#include "lve_model.hpp"

#include <cassert>
#include <iostream>
#include <cmath>

namespace lve
{
	LveModel::LveModel(LveDevice& lve_device, const std::vector<Vertex>& vertices)
		: lve_device(lve_device)
	{
		createVertexBuffer(vertices);
	}


	LveModel::~LveModel()
	{
		vkDestroyBuffer(lve_device.device(), vertex_buffer, nullptr);
		vkFreeMemory(lve_device.device(), vertex_buffer_memory, nullptr);
	}

	void LveModel::createVertexBuffer(const std::vector<Vertex>& vertices)
	{
		vertex_count = static_cast<uint32_t>(vertices.size());
		assert(vertex_count >= 3 && "Vertex count must be at least 3");

		VkDeviceSize buffer_size = sizeof(vertices[0]) * vertex_count;

		lve_device.createBuffer(buffer_size,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertex_buffer,
			vertex_buffer_memory);

		void* data;


		vkMapMemory(lve_device.device(), vertex_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(buffer_size));
		vkUnmapMemory(lve_device.device(), vertex_buffer_memory);
	}

	void LveModel::draw(VkCommandBuffer buffer)
	{
		vkCmdDraw(buffer, vertex_count, 1, 0, 0);
	}

	void LveModel::bind(VkCommandBuffer buffer)
	{
		VkBuffer buffers[] = { vertex_buffer };

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(buffer, 0, 1, buffers, offsets);
	}

	std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bingin_descriptions(1);
		bingin_descriptions[0].binding = 0;
		bingin_descriptions[0].stride = sizeof(Vertex);
		bingin_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bingin_descriptions;
	}

	std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attribute_descriptions(2);
		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[0].offset = offsetof(Vertex, position);

		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[1].offset = offsetof(Vertex, color);
		return attribute_descriptions;
	}

}