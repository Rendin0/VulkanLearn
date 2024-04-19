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
		std::vector<VkVertexInputAttributeDescription> attribute_descriptions(1);
		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[0].offset = 0;
		return attribute_descriptions;
	}

	std::vector<LveModel::Vertex> LveModel::Vertex::makeSerpinskiStep(const std::vector<Vertex>& vertices)
	{

		if (vertices.size() == 3)
		{
			std::vector<LveModel::Vertex> new_vertices(3);
			Vertex new_points[3];

			for (int i = 0; i < 3; i++)
			{
				new_points[i] =
				{
					{(vertices[i].position.x + vertices[(i + 1) % 3].position.x) / 2,
					(vertices[i].position.y + vertices[(i + 1) % 3].position.y) / 2}
				};
			}

			new_vertices.resize(9);

			new_vertices[0] = vertices[0];
			new_vertices[1] = new_points[0];
			new_vertices[2] = new_points[2];

			new_vertices[3] = new_points[0];
			new_vertices[4] = vertices[1];
			new_vertices[5] = new_points[1];

			new_vertices[6] = new_points[2];
			new_vertices[7] = new_points[1];
			new_vertices[8] = vertices[2];
			return new_vertices;
		}
		else
		{
			std::vector<LveModel::Vertex> new_vertices;
			new_vertices.resize(pow(3, (log2(vertices.size()) / log2(3) + 1)));


			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				std::vector<Vertex>tmp_vertices = makeSerpinskiStep({ vertices[i], vertices[i + 1], vertices[i + 2] });

				for (size_t j = 0; j < tmp_vertices.size(); j++)
				{
					new_vertices[(i * 3) + j] = tmp_vertices[j];
				}
			}
			std::cout << "\n{Vector size =" << new_vertices.size() << "}\n";
			return new_vertices;
		}

	}


}