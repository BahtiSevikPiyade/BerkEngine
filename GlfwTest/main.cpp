#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

// GLFW callback
static void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    FILE* log_file = fopen("C:\\temp\\glfw_test.txt", "w");
    if (!log_file) {
        std::cerr << "Cannot open C:\\temp\\glfw_test.txt" << std::endl;
        return 1;
    }

    fprintf(log_file, "=== GLFW TEST START ===\n");
    fflush(log_file);
    std::cout << "=== GLFW TEST START ===" << std::endl;

    glfwSetErrorCallback(error_callback);

    fprintf(log_file, "1. glfwInit()...\n");
    fflush(log_file);
    if (!glfwInit()) {
        fprintf(log_file, "FAILED: glfwInit()\n");
        fflush(log_file);
        std::cerr << "FAILED: glfwInit()" << std::endl;
        fclose(log_file);
        return 1;
    }
    fprintf(log_file, "OK: glfwInit()\n");
    fflush(log_file);

    fprintf(log_file, "2. Setting context hints...\n");
    fflush(log_file);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    fprintf(log_file, "3. Creating window...\n");
    fflush(log_file);
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Test", NULL, NULL);
    if (!window) {
        fprintf(log_file, "FAILED: glfwCreateWindow()\n");
        fflush(log_file);
        std::cerr << "FAILED: glfwCreateWindow()" << std::endl;
        glfwTerminate();
        fclose(log_file);
        return 1;
    }
    fprintf(log_file, "OK: Window created\n");
    fflush(log_file);

    fprintf(log_file, "4. Making context current...\n");
    fflush(log_file);
    glfwMakeContextCurrent(window);

    fprintf(log_file, "5. Loading GLAD...\n");
    fflush(log_file);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(log_file, "FAILED: gladLoadGLLoader()\n");
        fflush(log_file);
        std::cerr << "FAILED: gladLoadGLLoader()" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        fclose(log_file);
        return 1;
    }
    fprintf(log_file, "OK: GLAD loaded\n");
    fflush(log_file);

    fprintf(log_file, "6. Showing window...\n");
    fflush(log_file);
    glfwShowWindow(window);

    fprintf(log_file, "7. Running for 3 seconds...\n");
    fflush(log_file);
    double end_time = glfwGetTime() + 3.0;
    while (!glfwWindowShouldClose(window) && glfwGetTime() < end_time) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    fprintf(log_file, "=== SUCCESS ===\n");
    fflush(log_file);
    std::cout << "=== SUCCESS ===" << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();
    fclose(log_file);

    return 0;
}
