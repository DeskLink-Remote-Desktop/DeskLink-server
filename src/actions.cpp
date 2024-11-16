#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
// Find the thing to include
#elif defined(__APPLE__)
// Find the thing to include
#endif

#include "actions.hpp"


void keydown(const uint16_t keycode) {
	if (keycode != 0 && keycode < 4) {
		// 1: Left 2: Right 3: Middle

		const uint8_t event_code = 1 << ((keycode << 1) - 1);

		#ifdef _WIN32

		POINT coords;
		GetCursorPos(&coords);

		mouse_event(event_code, coords.x, coords.y, 0, 0);

		#endif
	} else {
		#ifdef _WIN32
		keybd_event(keycode, 0, 0x0000 /* KEYDOWN */, 0); // It's the absence of keyup that means pressing the key down.
		#endif
	}

}

void keyup(const uint16_t keycode) {
	if (keycode != 0 && keycode < 4) {
		// 1: Left 2: Right 3: Middle

		const uint8_t event_code = 1 << (keycode << 1);

		#ifdef _WIN32

		POINT coords;
		GetCursorPos(&coords);

		mouse_event(event_code, coords.x, coords.y, 0, 0);

		#endif
	} else {
		#ifdef _WIN32
		keybd_event(keycode, 0, KEYEVENTF_KEYUP, 0);
		#endif
	}

}

void mouse_move(const uint16_t x, const uint16_t y) {
	#ifdef _WIN32

	SetCursorPos(x, y);

	#endif
}

void scroll(const uint8_t direction) {
	#ifdef _WIN32

	const int8_t end_dir = (direction) ? -1 : 1;

	POINT coords;
	GetCursorPos(&coords);

	mouse_event(MOUSEEVENTF_WHEEL, coords.x, coords.y, end_dir * WHEEL_DELTA, 0);

	#endif
}
