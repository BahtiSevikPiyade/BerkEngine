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

// Windows'a özel hızlı erişim (Sadece Windows derliyorsak)
#ifdef BE_PLATFORM_WINDOWS
    #include <Windows.h>
#endif