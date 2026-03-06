#include "Application.h"
#include <iostream> // Şimdilik log yerine bunu kullanalım garanti olsun

namespace BerkEngine {

    Application::Application() {
    }

    Application::~Application() {
    }

    // BURAYA DİKKAT: Başına 'Application::' koymazsan dışarıdan erişilemez!
    void Application::Run() {
        std::cout << "BerkEngine Kosmaya Basladi!" << std::endl;
        while (true) {
            // Motorun ana döngüsü
        }
    }

}