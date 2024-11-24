#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <cstdint>


enum class Action : uint8_t {
	Keydown = 0,
	Keyup = 1,
	MouseMove = 2,
	Scroll = 3
};

void keydown(uint16_t keycode);
void keyup(uint16_t keycode);
void mouse_move(uint16_t x, uint16_t y);
void scroll(uint8_t direction);


#endif // ACTIONS_HPP