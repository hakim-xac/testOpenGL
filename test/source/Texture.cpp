#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace KHAS {
    Texture::Texture(std::string&& filename, FormatTexture input_format) noexcept
        : id_{}, width_{}, height_{}, is_load_{}, path_{std::move(filename) }
    {
        init(input_format, input_format);
    }
    Texture::Texture(std::string&& filename, FormatTexture input_format, FormatTexture output_format) noexcept
        : id_{}, width_{}, height_{}, is_load_{}, path_{ std::move(filename) }
    {
        init(input_format, output_format);
    }

    const Texture::value_type& Texture::getID() const & noexcept
    {
        return id_;
    }

    const int& Texture::getWidth() const& noexcept
    {
        return width_;
    }

    const int& Texture::getHeight() const& noexcept
    {
        return height_;
    }

    bool Texture::isLoad() noexcept
    {
        return is_load_;
    }

    const std::string& Texture::getPath() const& noexcept
    {
        return path_;
    }

    Texture::operator bool() const& noexcept
    {
        return is_load_;
    }

    void Texture::init(FormatTexture input_format, FormatTexture output_format) noexcept
    {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data{ stbi_load(path_.c_str(), &width_, &height_, &channels_, 0) };
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format_textures_[input_format], width_, height_, 0, format_textures_[output_format], GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            is_load_ = true;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}