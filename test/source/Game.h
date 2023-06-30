#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>

namespace KHAS {

    class Game final
    {
    private:
        enum class SHADER_TYPE { VERTEX, FRAGMENT };
    public:
        Game(int width, int height, std::string&& window_name);
        void init();
    private:
        [[nodiscard]] bool initGLFWWindow() noexcept;
        [[nodiscard]] bool initGladLoadGLL() noexcept;
        void loop() noexcept;
        void processInput() noexcept;
        void showErrors() noexcept;

        static void display_callbck(GLFWwindow* window, int width, int height);
    private:
        int window_width_{};
        int window_height_{};
        std::string window_name_{};
        GLFWwindow* window_p_{};
        bool is_init_{};
        std::vector<std::string> log_errors_;
        float overlay_{};
        float x_{};
        float y_{};
        float z_{};
        float speed_{};

    };


}