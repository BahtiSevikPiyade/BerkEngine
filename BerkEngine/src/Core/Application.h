#pragma once

namespace BerkEngine {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // Bu fonksiyonu Sandbox tarafında biz tanımlayacağız
    Application* CreateApplication();
}