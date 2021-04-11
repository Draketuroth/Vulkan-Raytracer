#ifndef VULKAN_ENUMERATE_H
#define VULKAN_ENUMERATE_H

namespace Vulkan 
{
    template <class Value>
    inline void GetEnumerateVector(VkResult(enumerate) (Platform::Type::Uint32*, Value*), std::vector<Value>& vector)
    {
        Platform::Type::Uint32 count = 0u;
        CheckVKResult(enumerate(&count, nullptr), "Enumerate");

        vector.resize(count);
        CheckVKResult(enumerate(&count, vector.data()), "Enumerate");
    }

    template <class Handle, class Value>
    inline void GetEnumerateVector(Handle handle, void(enumerate)(Handle, Platform::Type::Uint32*, Value*), std::vector<Value>& vector) 
    {
        Platform::Type::Uint32 count = 0u;
        enumerate(handle, &count, nullptr);

        vector.resize(count);
        enumerate(handle, &count, vector.data());
    }

    template <class Handle, class Value>
    inline void GetEnumerateVector(Handle handle, VkResult(enumerate) (Handle, Platform::Type::Uint32*, Value*), std::vector<Value>& vector) 
    {
        Platform::Type::Uint32 count = 0u;
        CheckVKResult(enumerate(handle, &count, nullptr), "Enumerate");

        vector.resize(count);
        CheckVKResult(enumerate(handle, &count, vector.data()), "Enumerate");
    }

    template <class HandleA, class HandleB, class Value>
    inline void GetEnumerateVector(HandleA handleA, HandleB handleB, VkResult(enumerate) (HandleA, HandleB, Platform::Type::Uint32*, Value*), std::vector<Value>& vector) 
    {
        Platform::Type::Uint32 count = 0;
        CheckVKResult(enumerate(handleA, handleB, &count, nullptr), "Enumerate");

        vector.resize(count);
        CheckVKResult(enumerate(handleA, handleB, &count, vector.data()), "Enumerate");
    }

    template <class Value>
    inline std::vector<Value> GetEnumerateVector(VkResult(enumerate) (Platform::Type::Uint32*, Value*))
    {
        std::vector<Value> ret;
        GetEnumerateVector(enumerate, ret);
        return ret;
    }

    template <class Handle, class Value>
    inline std::vector<Value> GetEnumerateVector(Handle handle, void(enumerate)(Handle, Platform::Type::Uint32*, Value*))
    {
        std::vector<Value> initial;
        GetEnumerateVector(handle, enumerate, initial);
        return initial;
    }

    template <class Handle, class Value> 
    inline std::vector<Value> GetEnumerateVector(Handle handle, VkResult(enumerate) (Handle, Platform::Type::Uint32*, Value*)) 
    {
        std::vector<Value> initial;
        GetEnumerateVector(handle, enumerate, initial);
        return initial;
    }

    template <class HandleA, class HandleB, class Value>
    inline std::vector<Value> GetEnumerateVector(HandleA handleA, HandleB handleB, VkResult(enumerate) (HandleA, HandleB, Platform::Type::Uint32*, Value*)) 
    {
        std::vector<Value> initial;
        GetEnumerateVector(handleA, handleB, enumerate, initial);
        return initial;
    }
}

#endif