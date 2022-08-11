#pragma once

#include <stdexcept>

#ifdef _DEBUG

    #define VK_CHECK(x)                                                             \
        if(x != VK_SUCCESS)                                                         \
        {                                                                           \
            throw std::runtime_error(std::string("Error: ") + #x);                  \
        }    
#else
    #define VK_CHECK(x) x;
#endif