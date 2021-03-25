#ifndef CORE_WINDOW_PROPERTIES_H
#define CORE_WINDOW_PROPERTIES_H

namespace Core
{
    struct WindowProperties
    {
		std::string title;
		Platform::Type::Uint width;
		Platform::Type::Uint height;
		bool fullScreen;
		bool resizable;
		bool cursorDisabled;

		WindowProperties(const std::string& windowTitle = "", 
						 Platform::Type::Uint windowWidth = 1280u, 
						 Platform::Type::Uint windowHeight = 760u,
						 bool useFullscreen = false,
						 bool allowResize = true,
						 bool useCursor = false) :
			title(windowTitle),
			width(windowWidth),
			height(windowHeight),
			fullScreen(useFullscreen),
			resizable(allowResize),
			cursorDisabled(useCursor)
		{
		}

		WindowProperties(const WindowProperties& other)
		{
			title = other.title;
			width = other.width;
			height = other.height;
			fullScreen = other.fullScreen;
			resizable = other.resizable;
			cursorDisabled = other.cursorDisabled;
		}
    };
}

#endif