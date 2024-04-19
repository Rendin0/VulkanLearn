#pragma once
#include "lve_device.hpp"
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLM/glm.hpp>

namespace lve
{
	class LveModel
	{
	public:

		struct Vertex {
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		};

		LveModel(LveDevice& lve_device, const std::vector<Vertex>& vertices);
		~LveModel();

		LveModel(const LveModel&) = delete;
		LveModel& operator=(const LveModel&) = delete;

		void bind(VkCommandBuffer buffer);
		void draw(VkCommandBuffer buffer);

	private:
		void createVertexBuffer(const std::vector<Vertex>& vertices);

		LveDevice& lve_device;
		VkBuffer vertex_buffer;
		VkDeviceMemory vertex_buffer_memory;
		uint32_t vertex_count;
	};
}