#include "Game.h"
#include "Texture.h"
#include "Shader.h"
#include "Keyboard.h"
#include <fstream>

#include <stb/stb_image.h>

namespace KHAS {
	Game::Game(int width, int height, std::string&& window_name)
		: window_width_{ width }
		, window_height_{ height }
		, window_name_{ window_name }
		, window_p_{ nullptr }
		, overlay_{0.5f}
		, x_{0.0f}
		, y_{0.0f}
		, z_{0.0f}
		, speed_{0.1f}
	{
		init();
		if (is_init_) loop();
		else glfwTerminate();

		showErrors();
	}

	void Game::init()
	{
		/* Error buffer. Size 50 */
		log_errors_.reserve(50);
		is_init_ = false;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (!initGLFWWindow()) return;
		if (!initGladLoadGLL()) return;

		glViewport(0, 0, window_width_, window_height_);
		glfwSetFramebufferSizeCallback(window_p_, Game::display_callbck);
		glfwSetKeyCallback(window_p_, Keyboard::keyCallback);


		is_init_ = true;
	}

	bool Game::initGLFWWindow() noexcept
	{
		window_p_ = glfwCreateWindow(window_width_, window_height_, window_name_.c_str(), NULL, NULL);
		if (window_p_ == nullptr)
		{
			log_errors_.emplace_back("Failed to create GLFW window");
			return false;
		}

		glfwMakeContextCurrent(window_p_);
		return true;
	}

	bool Game::initGladLoadGLL() noexcept
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			log_errors_.emplace_back("Failed to initialize GLAD");
			return false;
		}
		return true;
	}

	void Game::loop() noexcept {

		float vertices[]{
			// position					texture background		texture ship
			-1.0f,		-1.0f, 0.0f,	0.0f, 0.0f,				0.0f, 0.0f // bottom left
			, -1.0f,	1.0f, 0.0f,		0.0f, 1.0f,				0.0f, 1.0f // top left
			, 1.0f,		-1.0f, 0.0f,	1.0f, 0.0f,				1.0f, 0.0f // bottom right
			, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f,				1.0f, 1.0f // top right
		};
		unsigned int indices[]{
			0, 1, 2, 3, 1, 2
		};

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// positions 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(vertices[0]), (void*)0);
		glEnableVertexAttribArray(0);

		// texture background
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(vertices[0]), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// texture ship
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(vertices[0]), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		Texture background{ "source/images/background.jpg" };
		if (!background) {
			log_errors_.emplace_back("Can not create Texture! path: " + background.getPath());
			return;
		}
		Texture ship{ "source/images/ship2.png", Texture::FormatTexture::RGBA };
		if (!ship) {
			log_errors_.emplace_back("Can not create Texture! path: " + ship.getPath());
			return;
		}

		Shader shader{ "assets/vertex_core.glsl", "assets/fragment_core.glsl" };
		if (!shader) {
			log_errors_.emplace_back("Can not create shader!");
			return;
		}

		shader.use();
		shader.setInt("texture_background", 0);
		shader.setInt("texture_ship", 1);
		auto start_time{ glfwGetTime() };
		while (!glfwWindowShouldClose(window_p_))
		{
			/* input processing */
			processInput();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, background.getID());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, ship.getID());


			glm::mat4 model{ glm::mat4(1.0f) };
			glm::mat4 view{ glm::mat4(1.0f) };
			glm::mat4 projection{ glm::mat4(1.0f) };

			//model = glm::rotate(model, (float)start_time * glm::radians(-55.0f), glm::vec3(0.5f));
			view = glm::translate(view, glm::vec3(-x_, -y_, -z_));
			//projection = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f, 100.0f);

			shader.use();
			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			shader.setfloat("overlay", overlay_);


			glBindVertexArray(VAO);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


			/* update display*/
			glfwSwapBuffers(window_p_);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Game::processInput() noexcept
	{

		if(Keyboard::key(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window_p_, true);

		if (Keyboard::keyWentUp(GLFW_KEY_UP)) {
			overlay_ += speed_;
			if (overlay_ > 1.0f) overlay_ = 1.0f;
		}
		else if (Keyboard::keyWentDown(GLFW_KEY_DOWN)) {
			overlay_ -= speed_;
			if (overlay_ < 0.0f) overlay_ = 0.0f;
		}

		if (Keyboard::key(GLFW_KEY_W)) {
			y_ -= speed_;
		}
		else if (Keyboard::key(GLFW_KEY_S)) {
			y_ += speed_;
		}
		if (Keyboard::key(GLFW_KEY_A)) {
			x_ += speed_;
		}
		else if (Keyboard::key(GLFW_KEY_D)) {
			x_ -= speed_;
		}


	}

	void Game::showErrors() noexcept
	{
		if (log_errors_.size() == 0) return;
		std::ofstream out{ "error_log.txt", std::ios::out };
		for (auto&& elem : log_errors_) {
			out << elem << "\n";
		}
		out << std::endl;
	}


	void Game::display_callbck(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}