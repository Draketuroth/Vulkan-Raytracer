
#include "Core/Window.h"
#include "Utilities/StbImage.h"

namespace 
{
    static bool glfwInitialized = false;
}

static void GLFWErrorCallback(int error, const Platform::Type::Char* description)
{
    std::cerr << "GLFW Error: " << description << " (code: " << error << ")" << std::endl;
}

namespace Core 
{
    Window::Window(const WindowProperties& props)
    {
        windowData.title = props.title;
        originalWidth = windowData.width = props.width;
        originalHeight = windowData.height = props.height;
        windowData.fullScreen = props.fullScreen;

        glfwSetErrorCallback(GLFWErrorCallback);

        if (!glfwInitialized)
        {
            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW!");
            }

            if (!glfwVulkanSupported()) 
            {
                throw std::runtime_error("Vulkan not supported through GLFW!");
            }

            glfwInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, props.resizable ? GLFW_TRUE : GLFW_FALSE);

        // Query the primary monitor and get the monitor resolution in case fullscreen is enabled.
        GLFWmonitor* const monitor = props.fullScreen ? glfwGetPrimaryMonitor() : nullptr;
        if (monitor != nullptr) 
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            monitorData.width = mode->width;
            monitorData.height = mode->height;
        }

        // Creates the window. If monitor is nullptr, windowed mode will be used at start.
        window = glfwCreateWindow(originalWidth, originalHeight, props.title.c_str(), monitor, nullptr);

        if (window == nullptr) 
        {
            throw std::runtime_error("Failed to create GLFW window!");
        }

        // Load and set the application icon. 
        GLFWimage icon = {};
        icon.pixels = stbi_load("Assets\\Textures\\Vulkan.png", &icon.width, &icon.height, nullptr, 4);
        if (icon.pixels == nullptr)
        {
            throw std::runtime_error("Failed to load window icon!");
        }
        glfwSetWindowIcon(window, 1, &icon);
        stbi_image_free(icon.pixels);

        if (props.cursorDisabled)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        glfwSetWindowUserPointer(window, this);

        // Set GLFW callbacks.
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, Platform::Type::Int width, Platform::Type::Int height)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onResize)
                {
                    userWindow->onResize(width, height);
                }
            });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onClose)
                {
                    userWindow->onClose();
                }
            });

        glfwSetKeyCallback(window, [](GLFWwindow* window, Platform::Type::Int key, Platform::Type::Int scancode, Platform::Type::Int action, Platform::Type::Int mods)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onKey)
                {
                    userWindow->onKey(key, scancode, action, mods);
                }
            });

        glfwSetCharCallback(window, [](GLFWwindow* window, Platform::Type::Uint keycode)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onChar)
                {
                    userWindow->onChar(keycode);
                }
            });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, Platform::Type::Int button, Platform::Type::Int action, Platform::Type::Int mods)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onMouseButton)
                {
                    userWindow->onMouseButton(button, action, mods);
                }
            });

        glfwSetScrollCallback(window, [](GLFWwindow* window, Platform::Type::Double xoffset, Platform::Type::Double yoffset)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onScroll)
                {
                    userWindow->onScroll(xoffset, yoffset);
                }
            });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, Platform::Type::Double xpos, Platform::Type::Double ypos)
            {
                Window* const userWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
                if (userWindow->onCursorPosition)
                {
                    userWindow->onCursorPosition(xpos, ypos);
                }
            });
    }

    Window::~Window()
    {
        if (window != nullptr) 
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        glfwTerminate();
        glfwSetErrorCallback(nullptr);
    }

    void Window::run()
    {
        glfwSetTime(0.0);

        while (!glfwWindowShouldClose(window)) 
        {
            glfwPollEvents();

            if (draw) 
            {
                draw();
            }
        }
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(window, 1);
    }

    void Window::waitForEvents() const
    {
        glfwWaitEvents();
    }

    void Window::toggleFullscreen()
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (!windowData.fullScreen)
        {
            // Store previous window position for repositioning when exiting fullscreen mode.
            glfwGetWindowPos(window, &windowData.x, &windowData.y);

            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            windowData.fullScreen = !windowData.fullScreen;
        }
        else
        {
            windowData.width = originalWidth;
            windowData.height = originalHeight;
            glfwSetWindowMonitor(window, nullptr, windowData.x, windowData.y, windowData.width, windowData.height, mode->refreshRate);
            windowData.fullScreen = !windowData.fullScreen;
        }
    }

    void Window::toggleCaptureMouse()
    {
        if (!windowData.mouseCaptured)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            windowData.mouseCaptured = !windowData.mouseCaptured;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            windowData.mouseCaptured = false;
        }
    }

    void Window::setCaptureMouse(bool enabled)
    {
        if (enabled)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        windowData.mouseCaptured = enabled;
    }

    void Window::setVSync(bool enabled)
    {
        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
        windowData.vsync = enabled;
    }

    bool Window::isVSync() const
    {
        return windowData.vsync;
    }

    bool Window::isMouseCaptured() const
    {
        return windowData.mouseCaptured;
    }

    bool Window::isMinimized() const
    {
        const VkExtent2D size = getFrameBufferSize();
        return size.height == 0 && size.width == 0;
    }

    VkExtent2D Window::getFrameBufferSize() const
    {
        Platform::Type::Int width = 0;
        Platform::Type::Int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        return VkExtent2D{ static_cast<Platform::Type::Uint32>(width), static_cast<Platform::Type::Uint32>(height) };
    }

    VkExtent2D Window::getWindowSize()
    {
        Platform::Type::Int width = 0;
        Platform::Type::Int height = 0;
        glfwGetWindowSize(window, &width, &height);

        return VkExtent2D{ static_cast<Platform::Type::Uint32>(width), static_cast<Platform::Type::Uint32>(height) };
    }

    std::vector<const Platform::Type::Char*> Window::getRequiredInstanceExtensions() const
    {
        Platform::Type::Uint32 glfwExtensionCount = 0;
        const Platform::Type::Char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        return std::vector<const Platform::Type::Char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
    }

    Platform::Type::Double Window::getTime() const
    {
        return glfwGetTime();
    }

    const Platform::Type::Char* Window::getKeyName(Platform::Type::Int key, Platform::Type::Int scancode) const
    {
        return glfwGetKeyName(key, scancode);
    }

    Platform::Type::Float Window::getContentScale()
    {
        Platform::Type::Float x = 0.0f;
        Platform::Type::Float y = 0.0f;
        glfwGetWindowContentScale(window, &x, &y);

        return x;
    }

    Platform::Type::Uint Window::getMonitorWidth()
    {
        return monitorData.width;
    }

    Platform::Type::Uint Window::getMonitorHeight()
    {
        return monitorData.height;
    }

    GLFWwindow* Window::getHandle() const
    {
        return window;
    }
}