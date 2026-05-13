#include "bepch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>

#include "Application.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderTypes.h"
#include "Core/Log.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace BerkEngine {

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        std::cerr << ">>> [APP] Constructor başladı" << std::endl;
        std::cerr.flush();
        s_Instance = this;

        // Logger'ı başlat
        Log::Init();
        std::cerr << ">>> [APP] Logger::Init() tamamlandı" << std::endl;
        std::cerr.flush();
        BE_CORE_INFO("BerkEngine başlatılıyor...");

        if (!glfwInit()) {
            BE_CORE_ERROR("GLFW başlatılamadı!");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, "BerkEngine - ECS Cube", NULL, NULL);
        if (!m_Window) {
            BE_CORE_ERROR("GLFW penceresi oluşturulamadı!");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            BE_CORE_ERROR("GLAD OpenGL context yüklenemedi!");
            return;
        }

        // Pencereyi göster
        glfwShowWindow(m_Window);
        glfwFocusWindow(m_Window);

        BE_CORE_INFO("GLFW ve OpenGL başlatıldı - v{}.{}", glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MAJOR), glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MINOR));

        // Renderer'i başlat (viewport, GL flags, etc.)
        Renderer::Init(RenderBackendType::OpenGL, 1280, 720);

        // --- ECS SAHNESİNİ BAŞLAT ---
        m_ActiveScene = std::make_shared<Scene>();
        BE_CORE_INFO("Scene oluşturuldu");

        // Camera oluştur ve sahneye ekle
        m_Camera = std::make_unique<Camera>(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
        m_Camera->SetPosition(glm::vec3(2.0f, 2.0f, 2.5f));
        m_Camera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        m_ActiveScene->SetCamera(m_Camera.get());
        BE_CORE_INFO("Camera oluşturuldu");
        
        // Shader oluştur ve Renderer'a kayıt et
        auto ResolveAsset = [](const std::string& relativePath) -> std::string {
            namespace fs = std::filesystem;
            fs::path current = fs::current_path();
            for (int i = 0; i < 6; ++i) {
                fs::path candidate = current / relativePath;
                if (fs::exists(candidate))
                    return candidate.string();
                if (current.has_parent_path())
                    current = current.parent_path();
                else
                    break;
            }
            return std::string();
        };

        std::string vertPath = ResolveAsset("Sandbox/assets/shaders/vertex.glsl");
        std::string fragPath = ResolveAsset("Sandbox/assets/shaders/fragment.glsl");
        BE_CORE_INFO("Vertex shader yolu: {}", vertPath.empty() ? "(bulunamadı)" : vertPath);
        BE_CORE_INFO("Fragment shader yolu: {}", fragPath.empty() ? "(bulunamadı)" : fragPath);

        m_Shader = Shader::Create(vertPath.empty() ? "" : vertPath, fragPath.empty() ? "" : fragPath);
        Renderer::SetShader(m_Shader.get());
        m_ActiveScene->SetShader(m_Shader.get());
        BE_CORE_INFO("Shader oluşturuldu");

        // Mesh oluştur (Cube)
        {
            auto cubeUnique = Mesh::CreateCube();
            m_CubeMesh = std::shared_ptr<Mesh>(cubeUnique.release());
        }
        BE_CORE_INFO("Mesh (Cube) oluşturuldu");

        // Küp Entity'si oluştur
        auto cube = m_ActiveScene->CreateEntity("3D Cube");
        
        // Mesh ve Transform Component ekle
        auto& meshComp = cube.AddComponent<MeshComponent>();
        meshComp.MeshPtr = m_CubeMesh.get();
        
        auto& tc = cube.GetComponent<TransformComponent>();
        tc.Transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    }

    Application::~Application() {
        BE_CORE_INFO("Application yok ediliyor");
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::Run() {
        // Delta Time hesaplama için zaman takibi
        float lastFrameTime = 0.0f;

        BE_CORE_INFO("Application::Run() başlatılıyor");

        while (m_Running) {
            // Zaman hesaplama
            float time = (float)glfwGetTime();
            float timestep = time - lastFrameTime;
            lastFrameTime = time;

            // Pencere kapatma kontrolü
            if (glfwWindowShouldClose(m_Window)) {
                BE_CORE_INFO("Pencere kapatıldı");
                m_Running = false;
            }

            // 1. Sahne Güncelleme (ECS & Render burada tetikleniyor)
            // Renderer::BeginFrame() ve Clear() Scene::OnUpdate içinde çalışır
            try {
                m_ActiveScene->OnUpdate(timestep);
            } catch (const std::exception& e) {
                BE_CORE_ERROR("Scene::OnUpdate exception: {}", e.what());
                break;
            }

            // 2. Buffer Değişimi ve Olayları Dinleme
            glfwPollEvents();
            glfwSwapBuffers(m_Window);
        }

        // Renderer'i kapat
        Renderer::Shutdown();
        BE_CORE_INFO("Application::Run() bitti");
    }
}