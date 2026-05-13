#pragma once

// Standart C++ Kütüphaneleri (Sık kullandıklarımız)
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// Veri Yapıları
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Windows'a özel hızlı erişim
#ifdef BE_PLATFORM_WINDOWS
    // Reduce Windows header pollution and avoid min/max macro collisions
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    // Disable CRT deprecation warnings for legacy APIs (fopen etc.)
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
    // Suppress macro redefinition warnings from vendor headers when including Windows.h
    __pragma(warning(push))
    __pragma(warning(disable:4005))
    #include <Windows.h>
#   pragma(warning(pop))
#endif
