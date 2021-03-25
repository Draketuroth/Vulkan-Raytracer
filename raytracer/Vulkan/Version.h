#ifndef VULKAN_VERSION_H
#define VULKAN_VERSION_H

namespace Vulkan 
{
    struct Version final
    {
        Version(const Platform::Type::Uint32 version) :
            major(VK_VERSION_MAJOR(version)),
            minor(VK_VERSION_MINOR(version)),
            patch(VK_VERSION_PATCH(version))
        {

        }

        Version(const Platform::Type::Uint32 version, const Platform::Type::Uint32 vendorId) :
            major(VK_VERSION_MAJOR(version)),
            minor(VK_VERSION_MINOR(version) >> (vendorId == 0x10DE ? 2 : 0)),
            patch(VK_VERSION_PATCH(version) >> (vendorId == 0x10DE ? 4 : 0))
        {
        
        }

        const Platform::Type::Uint major;
        const Platform::Type::Uint minor;
        const Platform::Type::Uint patch;

        friend std::ostream& operator << (std::ostream& out, const Version& version) 
        {
            return out << version.major << "." << version.minor << "." << version.patch;
        }
    };


}

#endif
