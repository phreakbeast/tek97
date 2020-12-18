#include "tek_platform.hpp"

#include <stdio.h>
#include <time.h>

static u32 g_width;
static u32 g_height;
static bool g_close_requested = 0;
static bool g_fullscreen = false;

static HDC g_hdc;
static HWND g_hwnd;
static HGLRC g_hglrc;

static bool g_keys[512];
static bool g_buttons[3];

static int g_mouse_pos_x;
static int g_mouse_pos_y;
static int g_wheel;

#define MAX_LETTERS 16

static TekInputLetter g_letters[MAX_LETTERS];
static u32 g_num_letters = 0;

static int get_key(Key key);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void tek_window_destroy()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(g_hglrc);
}

bool tek_window_open(u32 width, u32 height, const char* title, bool fullscreen)
{
	g_hdc = NULL;
	g_hwnd = NULL;
	g_hglrc = NULL;
	g_width = 0;
	g_height = 0;
	g_close_requested = 0;
	g_fullscreen = fullscreen;

	for (int i = 0; i < 512; ++i)
	{
		g_keys[i] = 0;
	}

	g_buttons[0] = 0;
	g_buttons[1] = 0;
	g_buttons[2] = 0;

	g_width = width;
	g_height = height;

	RECT window_rect;
	window_rect.left = (long)0;
	window_rect.right = (long)width;
	window_rect.top = (long)0;
	window_rect.bottom = (long)height;

	//Window class
	printf("registering window class ...\n");
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	if (!RegisterClass(&wc))
	{
		MessageBoxA(NULL, "Error: couldn't register window class", "ERROR", MB_OK);
		printf("Error: Failed to register window class\n");
		return 0;
	}

	//Display settings
	DEVMODE dev;
	memset(&dev, 0, sizeof(dev));
	dev.dmSize = sizeof(dev);
	dev.dmPelsWidth = width;
	dev.dmPelsHeight = height;
	dev.dmBitsPerPel = 32;
	dev.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	DWORD dwExStyle;
	DWORD dwStyle;
	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;

		ChangeDisplaySettings(&dev, CDS_FULLSCREEN);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
	}

	AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle);

	//Window
	printf("creating window handle ...\n");
	g_hwnd = CreateWindowEx(dwExStyle, title, title,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
		CW_USEDEFAULT, 0,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		NULL, NULL,
		NULL,
		NULL);

	if (!g_hwnd)
	{
		MessageBoxA(NULL, "Error: Failed to create base window before Video Acceleration", "ERROR", MB_OK);
		printf("Error: Failed to create base window before Video Acceleration\n");
		return false;
	}

	//Opengl Context creation
	g_hdc = GetDC(g_hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	printf("choose pixel format...\n");
	int nPixelFormat = ChoosePixelFormat(g_hdc, &pfd);
	if (nPixelFormat == 0)
	{
		MessageBoxA(NULL, "Failed to choose pixel format", "ERROR", MB_OK);
		printf("Failed to choose pixel format\n");
		return false;
	}

	int bResult = SetPixelFormat(g_hdc, nPixelFormat, &pfd);
	if (!bResult)
	{
		MessageBoxA(NULL, "Failed to set pixel format", "ERROR", MB_OK);
		printf("Failed to set pixel format\n");
		return false;
	}


	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	HGLRC tempOpenGLContext = wglCreateContext(g_hdc);
	wglMakeCurrent(g_hdc, tempOpenGLContext);

	printf("creating opengl context...\n");
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB) 
	{
		g_hglrc = wglCreateContextAttribsARB(g_hdc, NULL, attributes);
		wglMakeCurrent(NULL, NULL);
		wglMakeCurrent(g_hdc, g_hglrc);

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		//VSYNC NOT LOCKED TO 60
		wglSwapIntervalEXT(0);
	}
	else
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempOpenGLContext);
		return 0;
	}

	ShowWindow(g_hwnd, SW_SHOW);
	UpdateWindow(g_hwnd);

	return true;
}

void tek_window_toggle_fullscreen()
{

}

bool tek_window_update()
{
	g_wheel = 0;
	g_num_letters = 0;

	MSG msg = { 0 };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void tek_window_swap_buffers()
{
	SwapBuffers(g_hdc);
}

bool tek_window_should_close()
{
	return g_close_requested;
}

static void track_mouse(HWND hwnd)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 1;
	tme.hwndTrack = hwnd;
	TrackMouseEvent(&tme);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int is_tracking = 0;

	switch (message)
	{
	case WM_SIZE: {
		g_width = LOWORD(lParam);
		g_height = HIWORD(lParam);
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
		g_close_requested = 1;
	}break;
	case WM_KEYDOWN: {
		g_keys[wParam] = 1;
	}break;
	case WM_KEYUP: {
		g_keys[wParam] = 0;
	}break;
	case WM_CHAR: {
		TekInputLetter l;
		switch (wParam)
		{
		case 0x08: {  // backspace
			l.type = INPUT_LETTER_BACKSPACE;
		}break;
		case 0x1B: {  // escape
			l.type = INPUT_LETTER_ESCAPE;
		}break;
		case 0x09: {  // tab
			l.type = INPUT_LETTER_TAB;
		}break;
		case 0x0D: {  // carriage return
			l.type = INPUT_LETTER_ENTER;
		}break;
		default: //printable character
		{
			char letter = (char)wParam;
			l.type = INPUT_LETTER_LETTER;
			l.letter = letter;
		}
		break;
		}
		if (g_num_letters < MAX_LETTERS)
		{
			g_letters[g_num_letters++] = l;
		}
	}break;
	case WM_MOUSEMOVE: {
		if (!is_tracking)
		{
			track_mouse(hWnd);
			is_tracking = 1;
		}		

		g_mouse_pos_x = LOWORD(lParam);
		g_mouse_pos_y = HIWORD(lParam);
		//printf("%d %d\n", g_mouse_pos_x, g_mouse_pos_y);
		
	}break;
	case WM_MOUSEHOVER: {
		//printf("enter\n");
	}break;
	case WM_MOUSELEAVE: {
		//printf("leave\n");
		is_tracking = 0;
	}break;
	case WM_LBUTTONDOWN: {
		g_buttons[0] = 1;
	}break;
	case WM_LBUTTONUP: {
		g_buttons[0] = 0;
	}break;
	case WM_MBUTTONDOWN: {
		g_buttons[1] = 1;
	}break;
	case WM_MBUTTONUP: {
		g_buttons[1] = 0;
	}break;
	case WM_RBUTTONDOWN: {
		g_buttons[2] = 1;
	}break;
	case WM_RBUTTONUP: {
		g_buttons[2] = 0;
	}break;
	case WM_MOUSEWHEEL: {
		int z_delta = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
		g_wheel = z_delta;
	}break;	
	case WM_ACTIVATE: {
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//printf("unfocus\n");
		}
		else
		{
			//printf("focus\n");
		}
	}break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool tek_window_is_key_down(Key key)
{
	int mapped_key = get_key(key);
	return g_keys[mapped_key];
}

bool tek_window_is_mouse_down(MouseButton button)
{
	switch (button)
	{
	case MOUSE_BUTTON_LEFT:
		return g_buttons[0];
	case MOUSE_BUTTON_MIDDLE:
		return g_buttons[1];
	case MOUSE_BUTTON_RIGHT:
		return g_buttons[2];
	default:
		break;
	}
	return 0;
}

int tek_window_get_mouse_x()
{
	return g_mouse_pos_x;
}

int tek_window_get_mouse_y()
{
	return g_mouse_pos_y;
}

int tek_window_get_mouse_wheel()
{
	return g_wheel;
}

void tek_window_set_cursor_pos(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	ClientToScreen(g_hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void tek_window_show_cursor(int val)
{
	ShowCursor(val);
}

TekInputLetter* tek_window_get_letter(int* letters)
{
	if (g_num_letters == 0)
	{
		letters = NULL;
		return NULL;
	}
	else
	{
		*letters = g_num_letters;
		return g_letters;
	}
	return NULL;
}

static int get_key(Key key)
{
	//TODO: change to table lookup
	switch (key)
	{
	case KEY_SPACE: {
		return VK_SPACE;
	}break;
	case KEY_APOSTROPHE: {
		return VK_OEM_7;
	}break;
	case KEY_COMMA: {  /* , */
		return VK_OEM_COMMA;
	}break;
	case KEY_MINUS: {  /* - */
		return VK_OEM_MINUS;
	}break;
	case KEY_PERIOD: {  /* . */
		return VK_OEM_PERIOD;
	}break;
	case KEY_SLASH: {  /* / */
		return VK_OEM_2;
	}break;
	case KEY_0: {
		return 0x30;
	}break;
	case KEY_1: {
		return 0x31;
	}break;
	case KEY_2: {
		return 0x32;
	}break;
	case KEY_3: {
		return 0x33;
	}break;
	case KEY_4: {
		return 0x34;
	}break;
	case KEY_5: {
		return 0x35;
	}break;
	case KEY_6: {
		return 0x36;
	}break;
	case KEY_7: {
		return 0x37;
	}break;
	case KEY_8: {
		return 0x38;
	}break;
	case KEY_9: {
		return 0x39;
	}break;
	case KEY_SEMICOLON: {
		return VK_OEM_1;
	}break;
		/*case 0x41: {
		return KEY_EQUAL;
		}break;*/ //TODO: find scancode
	case KEY_A: {
		return 0x41;
	}break;
	case KEY_B: {
		return 0x42;
	}break;
	case KEY_C: {
		return 0x43;
	}break;
	case KEY_D: {
		return 0x44;
	}break;
	case KEY_E: {
		return 0x45;
	}break;
	case KEY_F: {
		return 0x46;
	}break;
	case KEY_G: {
		return 0x47;
	}break;
	case KEY_H: {
		return 0x48;
	}break;
	case KEY_I: {
		return 0x49;
	}break;
	case KEY_J: {
		return 0x4A;
	}break;
	case KEY_K: {
		return 0x4B;
	}break;
	case KEY_L: {
		return 0x4C;
	}break;
	case KEY_M: {
		return 0x4D;
	}break;
	case KEY_N: {
		return 0x4E;
	}break;
	case KEY_O: {
		return 0x4F;
	}break;
	case KEY_P: {
		return 0x50;
	}break;
	case KEY_Q: {
		return 0x51;
	}break;
	case KEY_R: {
		return 0x52;
	}break;
	case KEY_S: {
		return 0x53;
	}break;
	case KEY_T: {
		return 0x54;
	}break;
	case KEY_U: {
		return 0x55;
	}break;
	case KEY_V: {
		return 0x56;
	}break;
	case KEY_W: {
		return 0x57;
	}break;
	case KEY_X: {
		return 0x58;
	}break;
	case KEY_Y: {
		return 0x59;
	}break;
	case KEY_Z: {
		return 0x5A;
	}break;
	case KEY_LEFT_BRACKET: {
		return VK_OEM_4;
	}break;
	case KEY_BACKSLASH: {
		return VK_OEM_5;
	}break;
	case KEY_RIGHT_BRACKET: {  /* ] */
		return VK_OEM_6;
	}break;
	case KEY_ESCAPE: {
		return VK_ESCAPE;
	}break;
	case KEY_ENTER: {
		return VK_RETURN;
	}break;
	case KEY_TAB: {
		return VK_TAB;
	}break;
	case KEY_BACKSPACE: {
		return VK_BACK;
	}break;
	case KEY_INSERT: {
		return VK_INSERT;
	}break;
	case KEY_DELETE: {
		return VK_DELETE;
	}break;
	case KEY_RIGHT: {
		return VK_RIGHT;
	}break;
	case KEY_LEFT: {
		return VK_LEFT;
	}break;
	case KEY_DOWN: {
		return VK_DOWN;
	}break;
	case KEY_UP: {
		return VK_UP;
	}break;
	case KEY_PAGE_UP: {
		return VK_PRIOR;
	}break;
	case KEY_PAGE_DOWN: {
		return VK_NEXT;
	}break;
	case KEY_HOME: {
		return VK_HOME;
	}break;
	case KEY_END: {
		return VK_END;
	}break;
	case KEY_CAPS_LOCK: {
		return VK_CAPITAL;
	}break;
	case KEY_SCROLL_LOCK: {
		return VK_SCROLL;
	}break;
		/*case 0x41: {
		return KEY_NUM_LOCK;
		}break;*///TODO: find scancode
	case KEY_PRINT_SCREEN: {
		return VK_PRINT;
	}break;
	case KEY_PAUSE: {
		return VK_PAUSE;
	}break;
	case KEY_F1: {
		return VK_F1;
	}break;
	case KEY_F2: {
		return VK_F2;
	}break;
	case KEY_F3: {
		return VK_F3;
	}break;
	case KEY_F4: {
		return VK_F4;
	}break;
	case KEY_F5: {
		return VK_F5;
	}break;
	case KEY_F6: {
		return VK_F6;
	}break;
	case KEY_F7: {
		return VK_F7;
	}break;
	case KEY_F8: {
		return VK_F8;
	}break;
	case KEY_F9: {
		return VK_F9;
	}break;
	case KEY_F10: {
		return VK_F10;
	}break;
	case KEY_F11: {
		return VK_F11;
	}break;
	case KEY_F12: {
		return VK_F12;
	}break;
	case KEY_KP_0: {
		return VK_NUMPAD0;
	}break;
	case KEY_KP_1: {
		return VK_NUMPAD1;
	}break;
	case KEY_KP_2: {
		return VK_NUMPAD2;
	}break;
	case KEY_KP_3: {
		return VK_NUMPAD3;
	}break;
	case KEY_KP_4: {
		return VK_NUMPAD4;
	}break;
	case KEY_KP_5: {
		return VK_NUMPAD5;
	}break;
	case KEY_KP_6: {
		return VK_NUMPAD6;
	}break;
	case KEY_KP_7: {
		return VK_NUMPAD7;
	}break;
	case KEY_KP_8: {
		return VK_NUMPAD8;
	}break;
	case KEY_KP_9: {
		return VK_NUMPAD9;
	}break;
	case KEY_KP_DECIMAL: {
		return VK_DECIMAL;
	}break;
	case KEY_KP_DIVIDE: {
		return VK_DIVIDE;
	}break;
	case KEY_KP_MULTIPLY: {
		return VK_MULTIPLY;
	}break;
	case KEY_KP_SUBTRACT: {
		return VK_SUBTRACT;
	}break;
	case KEY_KP_ADD: {
		return VK_ADD;
	}break;
	case KEY_LEFT_SHIFT: {
		return VK_LSHIFT;
	}break;
	case KEY_LEFT_CONTROL: {
		return VK_LCONTROL;
	}break;
	case KEY_LEFT_ALT: {
		return VK_LMENU;
	}break;
	case KEY_LEFT_SUPER: {
		return VK_LWIN;
	}break;
	case KEY_RIGHT_SHIFT: {
		return VK_RSHIFT;
	}break;
	case KEY_RIGHT_CONTROL: {
		return VK_RCONTROL;
	}break;
	case KEY_RIGHT_ALT: {
		return VK_RMENU;
	}break;
	case KEY_RIGHT_SUPER: {
		return VK_RWIN;
	}break;
	case KEY_MENU: {
		return VK_APPS;
	}break;
	default:
		break;
	}
	return -1;
}

