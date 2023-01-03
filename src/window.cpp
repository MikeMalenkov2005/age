#include "include/window.hpp"

#include <windows.h>
#include <windowsx.h>
#include <map>

#define AGE_WINDOW_CLASS "AgeWindowClass"

namespace age
{
	namespace wnd
	{
		std::map<HWND, Window*> windowMap;
		Window* current = NULL;

		LRESULT AgeWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Window* window = windowMap[hWnd];
			if (window == NULL) return DefWindowProc(hWnd, msg, wParam, lParam);
			switch (msg)
			{
			case WM_MOUSEMOVE:
				window->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MOUSEWHEEL:
				window->OnMouseScroll(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_LBUTTONDOWN:
				window->OnMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_LBUTTONUP:
				window->OnMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_RBUTTONDOWN:
				window->OnMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_RBUTTONUP:
				window->OnMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MBUTTONDOWN:
				window->OnMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MBUTTONUP:
				window->OnMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_XBUTTONDOWN:
				window->OnMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2 + GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_XBUTTONUP:
				window->OnMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2 + GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_KEYDOWN:
				window->OnKeyDown(wParam, false);
				break;
			case WM_KEYUP:
				window->OnKeyUp(wParam, false);
				break;
			case WM_CHAR:
				window->OnCharPrint(wParam, false);
				break;
			case WM_SYSKEYDOWN:
				window->OnKeyDown(wParam, true);
				break;
			case WM_SYSKEYUP:
				window->OnKeyUp(wParam, true);
				break;
			case WM_SYSCHAR:
				window->OnCharPrint(wParam, true);
				break;
			default:
				return DefWindowProcA(hWnd, msg, wParam, lParam);
			}
		}

		int Init()
		{
			WNDCLASSA wc = { 0 };
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = (WNDPROC)AgeWindowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = NULL;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = AGE_WINDOW_CLASS;
			if (!RegisterClassA(&wc)) {
				MessageBoxA(NULL, "RegisterClass() failed: Cannot register window class.", "Error", MB_OK);
				return 1;
			}
		}

		Window::Window(std::string name, int x, int y, int width, int height)
		{
			this->window = CreateWindowA(AGE_WINDOW_CLASS, name.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, NULL, NULL);
			if (this->window == NULL) {
				MessageBoxA(NULL, "CreateWindow() failed: Cannot create a window.", "Error", MB_OK);
			}
			this->shouldClose = false;
			windowMap[(HWND)(this->window)] = this;
		}

		Window::~Window()
		{
			if (current == this)
			{
				current = NULL;
				wglMakeCurrent(NULL, NULL);
			}
			ReleaseDC((HWND)(this->window), (HDC)(this->dc));
			wglDeleteContext((HGLRC)(this->rc));
			DestroyWindow((HWND)(this->dc));
		}

		void Window::SetFullscreen(bool fullscreen)
		{
			if (this->fullscreen != fullscreen)
			{
				this->fullscreen = fullscreen;
				this->UpdateWindowParams();
			}
		}

		bool Window::IsFullscreen()
		{
			return this->fullscreen;
		}

		void Window::SetVisible(bool visible)
		{
			if (this->visible != visible)
			{
				this->visible = visible;
				this->UpdateWindowParams();
			}
		}

		bool Window::IsVisible()
		{
			return this->visible;
		}

		void Window::UpdateWindowParams()
		{
			DWORD exStyle = 0L;
			DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			if (this->fullscreen)
			{
				style = style | WS_POPUP;
				exStyle = exStyle | WS_EX_TOPMOST;
			}
			else style = style | WS_OVERLAPPEDWINDOW;
			if (this->visible) style = style | WS_VISIBLE;
			SetWindowLongPtrA((HWND)(this->window), GWL_STYLE, style);
			SetWindowLongPtrA((HWND)(this->window), GWL_EXSTYLE, exStyle);
		}

		void Window::HandleEvents()
		{
			MSG msg;
			while (PeekMessageA(&msg, (HWND)(this->window), 0, 0, PM_NOREMOVE))
			{
				if (GetMessageA(&msg, (HWND)(this->window), 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				}
				else this->shouldClose = true;
			}
		}

		bool Window::ShouldClose()
		{
			return this->shouldClose;
		}

		GLenum Window::MakeContextCurrent()
		{
			if (this != current)
			{
				current = this;
				wglMakeCurrent((HDC)(this->dc), (HGLRC)(this->rc));
				return glewInit();
			}
			return GLEW_OK;
		}
	};
};
