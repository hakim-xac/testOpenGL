#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>


namespace KHAS {


	////////////////////////////////////////////////////////////////////////////////////////////

	Shader::Shader(std::string&& vertex_path, std::string&& fragment_path)
		: id_{}
	{
		auto vertex_id_opt{ compileShader(std::move(vertex_path), GL_VERTEX_SHADER) };
		auto fragment_id_opt{ compileShader(std::move(fragment_path), GL_FRAGMENT_SHADER) };

		id_ = glCreateProgram();
		if (!id_.has_value() || !vertex_id_opt.has_value() || !fragment_id_opt.has_value()) return;


		glAttachShader(id_.value(), vertex_id_opt.value());
		glAttachShader(id_.value(), fragment_id_opt.value());
		glLinkProgram(id_.value());
		if (!isProgramCompile(id_.value())) return;
		glDeleteShader(vertex_id_opt.value());
		glDeleteShader(fragment_id_opt.value());

	}

	void Shader::use() noexcept
	{
		if (id_.has_value())
			glUseProgram(id_.value());
	}

	void Shader::setBool(std::string&& name, bool value) const
	{
		if (!id_.has_value()) return;
		const std::string tmp_name{ std::move(name) };
		glUniform1i(glGetUniformLocation(id_.value(), tmp_name.c_str()), static_cast<int>(value));
	}

	void Shader::setInt(std::string&& name, int value) const
	{
		if (!id_.has_value()) return;
		const std::string tmp_name{ std::move(name) };
		glUniform1i(glGetUniformLocation(id_.value(), tmp_name.c_str()), value);
	}

	void Shader::setfloat(std::string&& name, float value) const
	{
		if (!id_.has_value()) return;
		const std::string tmp_name{ std::move(name) };
		glUniform1f(glGetUniformLocation(id_.value(), tmp_name.c_str()), value);
	}

	void Shader::setMat4(std::string&& name, glm::mat4 value) const
	{
		if (!id_.has_value()) return;
		glUniformMatrix4fv(glGetUniformLocation(id_.value(), name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	Shader::operator bool() const& noexcept
	{
		return id_.has_value();
	}

	std::optional<Shader::value_type> Shader::getId() const& noexcept
	{
		return id_;
	}

	bool Shader::isInit() & noexcept
	{
		return id_.has_value();
	}

	bool Shader::isShaderCompile(value_type shader_id) const& noexcept
	{
		GLint  success{};
		constexpr int max_buffer{ 1024 };
		char info_log[max_buffer]{};
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (success) [[likely]] return true;

		glGetShaderInfoLog(shader_id, max_buffer, NULL, info_log);
		std::cout << "ERROR: Shader::isShaderCompile\r\n"
			<< "shader_id: " << shader_id << "\r\n"
			<< "gl_log: " << info_log << "\r\n";

		return false;
	}

	bool Shader::isProgramCompile(value_type shader_id) & noexcept
	{
		GLint  success{};
		constexpr int max_buffer{ 1024 };
		char info_log[max_buffer]{};

		glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
		if (success) [[likely]] return true;

		glGetShaderInfoLog(shader_id, max_buffer, NULL, info_log);
		std::cout << "ERROR: Shader::isShaderCompile\r\n"
			<< "shader_id: " << shader_id << "\r\n"
			<< "gl_log: " << info_log << "\r\n";
		return false;
	}

	std::optional<Shader::value_type> Shader::compileShader(std::string&& shader_path, GLenum type) const& noexcept
	{
		std::ifstream shader_file{ std::move(shader_path) };
		if (!shader_file.is_open()) return std::nullopt;

		std::stringstream ss;
		ss << shader_file.rdbuf();

		const std::string shader_code_buffer{ ss.str() };
		const GLchar* shader_code{ shader_code_buffer.c_str() };

		Shader::value_type id{ glCreateShader(type) };
		glShaderSource(id, 1, &shader_code, NULL);
		glCompileShader(id);

		if (!isShaderCompile(id)) return std::nullopt;
		return id;
	}
}