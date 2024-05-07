#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{
	class LveWindow
	{
	public:
		LveWindow(int width, int height, std::string window_name);
		~LveWindow();

		LveWindow(const LveWindow&) = delete;
		LveWindow& operator=(const LveWindow&) = delete;

		bool shouldClose();
		void setKeyCallback(const GLFWkeyfun& keyProcess);
		void setMouseButtonCallback(const GLFWmousebuttonfun& mouseProcess);
		void setWindowUserPointer(void* ptr);

		bool wasWindowResized();
		void resetWindowResizedFlag();

		void createWindowSurface(VkInstance vk_instance, VkSurfaceKHR *vk_surface_khr);

		VkExtent2D getExtend();

	private:
		static void framebufferResizedCallback(GLFWwindow *window, int width, int height);
		GLFWwindow* window;
		std::string window_name;

		int width;
		int height;
		bool window_resized = false;

		void initWindow();

	};
}