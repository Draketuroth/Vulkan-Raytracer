#ifndef CORE_WINDOW_H
#define CORE_WINDOW_H

#include "Core/WindowProperties.h"

namespace Core
{
	struct WindowData
	{
		std::string title;
		Platform::Type::Uint width;
		Platform::Type::Uint height;
		Platform::Type::Int x;
		Platform::Type::Int y;
		bool vsync;
		bool mouseCaptured;
		bool fullScreen;
	};

	struct MonitorData
	{
		Platform::Type::Uint width;
		Platform::Type::Uint height;
	};

    class Window final
    {
	public:
		explicit Window(const WindowProperties& props = WindowProperties());
		~Window();

		void run();
		void close();
		void waitForEvents() const;
		void toggleFullscreen();
		void toggleCaptureMouse();
		void setCaptureMouse(bool enabled);
		void setVSync(bool enabled);
		bool isVSync() const;
		bool isMouseCaptured() const;
		bool isMinimized() const;

		VkExtent2D getFrameBufferSize() const;
		VkExtent2D getWindowSize();

		std::vector<const Platform::Type::Char*> getRequiredInstanceExtensions() const;

		Platform::Type::Double getTime() const;
		const Platform::Type::Char* getKeyName(Platform::Type::Int key, Platform::Type::Int scancode) const;
		Platform::Type::Float getContentScale();
		Platform::Type::Uint getMonitorWidth();
		Platform::Type::Uint getMonitorHeight();
		
		GLFWwindow* getHandle() const;

		std::function<void()> draw;
		std::function<void()> onClose;
		std::function<void(Platform::Type::Uint keycode)> onChar;
		std::function<void(Platform::Type::Int width, Platform::Type::Int height)> onResize;
		std::function<void(Platform::Type::Int key, Platform::Type::Int scancode, Platform::Type::Int action, Platform::Type::Int mods)> onKey;
		std::function<void(Platform::Type::Double xpos, Platform::Type::Double ypos)> onCursorPosition;
		std::function<void(Platform::Type::Int button, Platform::Type::Int action, Platform::Type::Int mods)> onMouseButton;
		std::function<void(Platform::Type::Double xoffset, Platform::Type::Double yoffset)> onScroll;

	private:
		Platform::Type::Int originalWidth;
		Platform::Type::Int originalHeight;

		MonitorData monitorData = {};
		WindowData windowData = {};
		GLFWwindow* window = nullptr;
    };
}

#endif