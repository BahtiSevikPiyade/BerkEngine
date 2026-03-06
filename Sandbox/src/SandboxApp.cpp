#include <BerkEngine.h>

class Sandbox : public BerkEngine::Application {
public:
    Sandbox() {
    }

    ~Sandbox() {
    }
};

int main() {
    BerkEngine::Log::Init();
    BE_INFO("Sandbox Uygulamasi Baslatildi!");

    auto app = new Sandbox();
    app->Run();
    delete app;

    return 0;
}