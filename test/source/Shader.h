#pragma once
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace KHAS {


	class Shader final
	{
		using value_type = unsigned int;
	public:
		explicit Shader(std::string&& vertex_path, std::string&& fragment_path);
		void use() noexcept;

		void setBool(std::string&& name, bool value) const;
		void setInt(std::string&& name, int value) const;
		void setfloat(std::string&& name, float value) const;
		void setMat4(std::string&& name, glm::mat4 val) const;

		operator bool() const& noexcept;
		std::optional<value_type> getId() const& noexcept;
		[[nodiscard]] bool isInit() & noexcept;
		[[nodiscard]] bool isShaderCompile(value_type shader_id) const& noexcept;
		[[nodiscard]] bool isProgramCompile(value_type shader_id) & noexcept;
	private:
		std::optional<value_type> compileShader(std::string&& shader_path, GLenum type) const& noexcept;

	private:
		std::optional<value_type> id_{};

	};

}