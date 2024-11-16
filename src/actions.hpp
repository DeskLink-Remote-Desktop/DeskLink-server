#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <cstdint>


void keydown(uint16_t keycode);
void keyup(uint16_t keycode);
void mouse_move(uint16_t x, uint16_t y);
void scroll(uint8_t direction);


#endif // ACTIONS_HPP