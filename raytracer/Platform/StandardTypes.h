#ifndef PLATFORM_STANDARD_TYPES_H
#define PLATFORM_STANDARD_TYPES_H

#include <memory>

namespace Platform
{
    namespace Type
    {
        typedef int             Int;
        typedef unsigned int    Uint;
        typedef uint8_t         Uint8;
        typedef int32_t         Int32;
        typedef uint32_t        Uint32;
        typedef uint64_t        Uint64;
        typedef char            Char;
        typedef double          Double;
        typedef float           Float;
    }

    namespace Pointer 
    {
        template<typename T>
        using Scope = std::unique_ptr<T>;
    }
}

#endif