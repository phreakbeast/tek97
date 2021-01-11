#include "tek_keys.hpp"

char tek_printable_char(Key key, bool shift)
{
    switch (key)
    {
	case KEY_SPACE: return ' ';
	case KEY_APOSTROPHE:
	    if (shift)
		return '"';
	    else
		return '\'';
	case KEY_COMMA:
	    if (shift)
		return '<';
	    else
		return ',';
	case KEY_MINUS:
	    if (shift)
		return '_';
	    else
		return '-';
	case KEY_PERIOD:
	    if (shift)
		return '>';
	    else
		return '.';
	case KEY_SLASH:
	    if (shift)
		return '?';
	    else
		return '/';
	case KEY_0:
	    if (shift)
		return ')';
	    else
		return '0';
	case KEY_1:
	    if (shift)
		return '!';
	    else
		return '1';
	case KEY_2:
	    if (shift)
		return '@';
	    else
		return '2';
	case KEY_3:
	    if (shift)
		return '#';
	    else
		return '3';
	case KEY_4:
	    if (shift)
		return '$';
	    else
		return '4';
	case KEY_5:
	    if (shift)
		return '%';
	    else
		return '5';
	case KEY_6:
	    if (shift)
		return '^';
	    else
		return '6';
	case KEY_7:
	    if (shift)
		return '&';
	    else
		return '7';
	case KEY_8:
	    if (shift)
		return '*';
	    else
		return '8';
	case KEY_9:
	    if (shift)
		return '(';
	    else
		return '9';
	case KEY_SEMICOLON:
	    if (shift)
		return ':';
	    else
		return ';';
	case KEY_EQUAL:
	    if (shift)
		return '+';
	    else
		return '=';/* = */
	case KEY_A:
	    if (shift)
		return 'A';
	    else
		return 'a';
	case KEY_B:
	    if (shift)
		return 'B';
	    return 'b';
	case KEY_C:
	    if (shift)
		return 'C';
	    return 'c';
	case KEY_D:
	    if (shift)
		return 'D';
	    else
		return 'd';
	case KEY_E:
	    if (shift)
		return 'E';
	    else
		return 'e';
	case KEY_F:
	    if (shift)
		return 'F';
	    else
		return 'f';
	case KEY_G:
	    if (shift)
		return 'G';
	    else
		return 'g';
	case KEY_H:
	    if (shift)
		return 'H';
	    else
		return 'h';
	case KEY_I:
	    if (shift)
		return 'I';
	    else
		return 'i';
	case KEY_J:
	    if (shift)
		return 'J';
	    else
		return 'j';
	case KEY_K:
	    if (shift)
		return 'K';
	    else
		return 'k';
	case KEY_L:
	    if (shift)
		return 'L';
	    else
		return 'l';
	case KEY_M:
	    if (shift)
		return 'M';
	    else
		return 'm';
	case KEY_N:
	    if (shift)
		return 'N';
	    else
		return 'n';
	case KEY_O:
	    if (shift)
		return 'O';
	    else
		return 'o';
	case KEY_P:
	    if (shift)
		return 'P';
	    else
		return 'p';
	case KEY_Q:
	    if (shift)
		return 'Q';
	    else
		return 'q';
	case KEY_R:
	    if (shift)
		return 'R';
	    else
		return 'r';
	case KEY_S:
	    if (shift)
		return 'S';
	    else
		return 's';
	case KEY_T:
	    if (shift)
		return 'T';
	    else
		return 't';
	case KEY_U:
	    if (shift)
		return 'U';
	    else
		return 'u';
	case KEY_V:
	    if (shift)
		return 'V';
	    else
		return 'v';
	case KEY_W:
	    if (shift)
		return 'W';
	    else
		return 'w';
	case KEY_X:
	    if (shift)
		return 'X';
	    else
		return 'x';
	case KEY_Y:
	    if (shift)
		return 'Y';
	    else
		return 'y';
	case KEY_Z:
	    if (shift)
		return 'Z';
	    else
		return 'z';
	case KEY_LEFT_BRACKET:
	    if (shift)
		return '{';
	    else
		return '[';
	case KEY_BACKSLASH:
	    if (shift)
		return '|';
	    else
		return '\\';
	case KEY_RIGHT_BRACKET:
	    if (shift)
		return '}';
	    else
		return ']';
	case KEY_ESCAPE: return 0;
	case KEY_ENTER: return 0;
	case KEY_TAB: return 0;
	case KEY_BACKSPACE: return 0;
	case KEY_INSERT: return 0;
	case KEY_DELETE: return 0;
	case KEY_RIGHT: return 0;
	case KEY_LEFT: return 0;
	case KEY_DOWN: return 0;
	case KEY_UP: return 0;
	case KEY_PAGE_UP: return 0;
	case KEY_PAGE_DOWN: return 0;
	case KEY_HOME: return 0;
	case KEY_END: return 0;
	case KEY_CAPS_LOCK: return 0;
	case KEY_SCROLL_LOCK: return 0;
	case KEY_NUM_LOCK: return 0;
	case KEY_PRINT_SCREEN: return 0;
	case KEY_PAUSE: return 0;
	case KEY_F1: return 0;
	case KEY_F2: return 0;
	case KEY_F3: return 0;
	case KEY_F4: return 0;
	case KEY_F5: return 0;
	case KEY_F6: return 0;
	case KEY_F7: return 0;
	case KEY_F8: return 0;
	case KEY_F9: return 0;
	case KEY_F10: return 0;
	case KEY_F11: return 0;
	case KEY_F12: return 0;
	case KEY_KP_0: return '0';
	case KEY_KP_1: return '1';
	case KEY_KP_2: return '2';
	case KEY_KP_3: return '3';
	case KEY_KP_4: return '4';
	case KEY_KP_5: return '5';
	case KEY_KP_6: return '6';
	case KEY_KP_7: return '7';
	case KEY_KP_8: return '8';
	case KEY_KP_9: return '9';
	case KEY_KP_DECIMAL: return '.';
	case KEY_KP_DIVIDE: return '/';
	case KEY_KP_MULTIPLY: return '*';
	case KEY_KP_SUBTRACT: return '-';
	case KEY_KP_ADD: return '+';
	case KEY_LEFT_SHIFT: return 0;
	case KEY_LEFT_CONTROL: return 0;
	case KEY_LEFT_ALT: return 0;
	case KEY_LEFT_SUPER: return 0;
	case KEY_RIGHT_SHIFT: return 0;
	case KEY_RIGHT_CONTROL: return 0;
	case KEY_RIGHT_ALT: return 0;
	case KEY_RIGHT_SUPER: return 0;
	case KEY_MENU: return 0;
	default: return 0;
    }
    return 0;
}
