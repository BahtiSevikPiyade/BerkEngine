#pragma once

#include <memory>
#include "Scene/Scene.h"

// Forward declaration - glfw3.h include etmeye gerek kalmaz
struct GLFWwindow; 

namespace BerkEngine {

    class Camera;
    class Shader;
    class Mesh;

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        
        inline static Application& Get() { return *s_Instance; }
        inline GLFWwindow* GetWindow() const { return m_Window; }
        
        // Aktif sahneye her yerden erişebilmek için (Örn: SandboxApp)
        inline std::shared_ptr<Scene> GetActiveScene() { return m_ActiveScene; }

    private:
        GLFWwindow* m_Window;
        bool m_Running = true;
        
        // Motorun ana sahnesi
        std::shared_ptr<Scene> m_ActiveScene;

        // Rendering resources
        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<Shader> m_Shader;
        std::shared_ptr<Mesh> m_CubeMesh;

        static Application* s_Instance;
    };

    // Client (Sandbox) tarafında implement edilecek olan fonksiyon
    Application* CreateApplication();
}