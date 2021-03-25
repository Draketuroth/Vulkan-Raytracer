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
    inline void GetEnumerateVector(Handle handle, VkResult(enumerate) (Handle, Platform::Type::Uint32*, Value*), std::vector<Value>& vector) 
    {
        Platform::Type::Uint32 count = 0u;
        CheckVKResult(enumerate(handle, &count, nullptr), "Enumerate");

        vector.resize(count);
        CheckVKResult(enumerate(handle, &count, vector.data()), "Enumerate");
    }

    template <class Value>
    inline std::vector<Value> GetEnumerateVector(VkResult(enumerate) (Platform::Type::Uint32*, Value*))
    {
        std::vector<Value> ret;
        GetEnumerateVector(enumerate, ret);
        return ret;
    }
}

#endif