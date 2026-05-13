#include <BerkEngine.h>
#include <cstdio>
#include <iostream>
#include <fstream>

class Sandbox : public BerkEngine::Application {
public:
    Sandbox() {}
    ~Sandbox() {}
};

BerkEngine::Application* BerkEngine::CreateApplication() {
    return new Sandbox();
}

// Main fonksiyonun EntryPoint.h içindeyse oraya, değilse buraya:
int main() {
    // C:\temp'e main entry'iy yaz
    FILE* tempFile = fopen("C:\\temp\\ber_engine_start.txt", "w");
    if (tempFile) {
        fprintf(tempFile, "MAIN EXECUTED\n");
        fprintf(tempFile, "Creating app...\n");
        fclose(tempFile);
    }
    
    // Hemen stderr yaz - test etmek için
    std::cerr << ">>> SANDBOX MAIN BAŞLADI" << std::endl;
    std::cerr.flush();
    
    // Dosyaya log yazmayı dene
    std::ofstream logFile("sandbox_debug.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << ">>> HATA: sandbox_debug.txt açılamadı" << std::endl;
    } else {
        std::cerr << ">>> OK: sandbox_debug.txt açıldı" << std::endl;
        logFile << "\n============== SANDBOX START ==============" << std::endl;
        logFile.flush();
    }
    std::cerr.flush();
    
    try {
        std::cerr << ">>> CreateApplication çağrılıyor..." << std::endl;
        std::cerr.flush();
        logFile << "1. CreateApplication..." << std::endl;
        logFile.flush();
        
        auto app = BerkEngine::CreateApplication();
        
        std::cerr << ">>> Application::Run() çağrılıyor..." << std::endl;
        std::cerr.flush();
        logFile << "2. Application::Run()..." << std::endl;
        logFile.flush();
        
        app->Run();
        
        std::cerr << ">>> App mübaşir siliniyor..." << std::endl;
        std::cerr.flush();
        logFile << "3. delete app..." << std::endl;
        logFile.flush();
        
        delete app;
        
        std::cerr << ">>> BAŞARI - Çıkılıyor" << std::endl;
        std::cerr.flush();
        logFile << "=== SUCCESS ===" << std::endl;
        logFile.flush();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << ">>> EXCEPTION: " << e.what() << std::endl;
        std::cerr.flush();
        logFile << "EXCEPTION: " << e.what() << std::endl;
        logFile.flush();
        return 1;
    } catch (...) {
        std::cerr << ">>> UNKNOWN EXCEPTION" << std::endl;
        std::cerr.flush();
        logFile << "UNKNOWN EXCEPTION" << std::endl;
        logFile.flush();
        return 1;
    }
}