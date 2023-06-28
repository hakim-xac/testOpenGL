#pragma once
#include <string>
#include <memory>
#include <map>
#include <stb/stb_image.h>
#include <glad/glad.h>

namespace KHAS {
    class Texture final
    {
        using value_type = unsigned int;
    public:
        enum class FormatTexture { RGB, RGBA };
    public:
        explicit Texture(std::string&& filename
            , FormatTexture input_format = FormatTexture::RGB) noexcept;
        explicit Texture(std::string&& filename
            , FormatTexture input_format
            , FormatTexture output_format) noexcept;
        [[nodiscard]] const value_type& getID() const & noexcept;
        [[nodiscard]] const int& getWidth() const & noexcept;
        [[nodiscard]] const int& getHeight() const & noexcept;
        [[nodiscard]] bool isLoad() noexcept;
        [[nodiscard]] const std::string& getPath() const& noexcept;
        operator bool() const & noexcept;
    private:
        void init(FormatTexture input_format, FormatTexture output_format) noexcept;
    private:
        value_type id_{};
        int width_{};
        int height_{};
        int channels_{};
        bool is_load_{};
        std::string path_{};
    private:
        inline static std::map< FormatTexture, GLint> format_textures_{
            { FormatTexture::RGB, GL_RGB }
            , { FormatTexture::RGBA, GL_RGBA }
        };
    };

}