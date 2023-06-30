#include "Keyboard.h"
#include <algorithm>

namespace KHAS {

	bool Keyboard::keys_[GLFW_KEY_LAST]{};
	bool Keyboard::keys_changed_[GLFW_KEY_LAST]{};

	void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action != GLFW_RELEASE) {
			if (!keys_[key]) keys_[key] = true;
		}
		else keys_[key] = false;

		keys_changed_[key] = action != GLFW_REPEAT;
	}

	bool Keyboard::key(int key) {
		return keys_[key];
	}

	bool Keyboard::keyChange(int key) {
		bool ret{ keys_changed_[key] };
		keys_changed_[key] = false;
		return ret;
		//return std::exchange(keys_changed_[key], false);
	}

	bool Keyboard::keyWentUp(int key) {
		return !keys_[key] && keyChange(key);
	}

	bool Keyboard::keyWentDown(int key) {
		return keys_[key] && keyChange(key);
	}
}