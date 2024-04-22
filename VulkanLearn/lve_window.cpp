#include "lve_window.hpp"
#include <stdexcept>

namespace lve
{
	LveWindow::LveWindow(int width, int height, std::string window_name) : width(width), height(height), window_name(window_name)
	{
		initWindow();
	}

	LveWindow::~LveWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool LveWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void LveWindow::setKeyCallback(const GLFWkeyfun& keyProcess)
	{
		glfwSetKeyCallback(window, keyProcess);
	}

	void LveWindow::setWindowUserPointer(void* ptr)
	{
		glfwSetWindowUserPointer(window, ptr);
	}

	bool LveWindow::wasWindowResized()
	{
		return window_resized;
	}

	void LveWindow::resetWindowResizedFlag()
	{
		window_resized = false;
	}

	void LveWindow::createWindowSurface(VkInstance vk_instance, VkSurfaceKHR* vk_surface_khr)
	{
		if (glfwCreateWindowSurface(vk_instance, window, nullptr, vk_surface_khr) != VK_SUCCESS)
			throw std::runtime_error("Surface not success");
	}

	VkExtent2D LveWindow::getExtend()
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	void LveWindow::framebufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto lve_window = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));

		lve_window->window_resized = true;
		lve_window->width = width;
		lve_window->height = height;
	}

	void LveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
		//setWindowUserPointer(this); //-- Is this required? Already setting windwo user point in first_app.cpp
		glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
	}
}