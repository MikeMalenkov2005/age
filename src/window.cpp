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

		struct WinParams
		{
			DWORD styleEx;
			DWORD style;
			RECT rect;
			bool maximized;
		};

		bool WinSwapBuffers(HDC hdc)
		{
			WINBOOL ans = SwapBuffers(hdc);
			return ans != 0;
		}

		LRESULT AgeWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Window* window = windowMap[hWnd];
			if (window == NULL) return DefWindowProc(hWnd, msg, wParam, lParam);
			switch (msg)
			{
			case WM_MOUSEMOVE:
				if (window->OnMouseMove != NULL) window->OnMouseMove(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MOUSEWHEEL:
				if (window->OnMouseScroll != NULL) window->OnMouseScroll(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_LBUTTONDOWN:
				if (window->OnMouseButtonDown != NULL) window->OnMouseButtonDown(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_LBUTTONUP:
				if (window->OnMouseButtonUp != NULL) window->OnMouseButtonUp(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_RBUTTONDOWN:
				if (window->OnMouseButtonDown != NULL) window->OnMouseButtonDown(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_RBUTTONUP:
				if (window->OnMouseButtonUp != NULL) window->OnMouseButtonUp(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MBUTTONDOWN:
				if (window->OnMouseButtonDown != NULL) window->OnMouseButtonDown(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_MBUTTONUP:
				if (window->OnMouseButtonUp != NULL) window->OnMouseButtonUp(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2, GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_XBUTTONDOWN:
				if (window->OnMouseButtonDown != NULL) window->OnMouseButtonDown(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2 + GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_XBUTTONUP:
				if (window->OnMouseButtonUp != NULL) window->OnMouseButtonUp(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 2 + GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
				break;
			case WM_KEYDOWN:
				if (window->OnKeyDown != NULL) window->OnKeyDown(window, wParam, false);
				break;
			case WM_KEYUP:
				if (window->OnKeyUp != NULL) window->OnKeyUp(window, wParam, false);
				break;
			case WM_CHAR:
				if (window->OnCharPrint != NULL) window->OnCharPrint(window, wParam, false);
				break;
			case WM_SYSKEYDOWN:
				if (window->OnKeyDown != NULL) window->OnKeyDown(window, wParam, true);
				break;
			case WM_SYSKEYUP:
				if (window->OnKeyUp != NULL) window->OnKeyUp(window, wParam, true);
				break;
			case WM_SYSCHAR:
				if (window->OnCharPrint != NULL) window->OnCharPrint(window, wParam, true);
				break;
			case WM_QUIT:
			case WM_DESTROY:
			case WM_CLOSE:
				window->OnClose();
				break;
			}
			return DefWindowProcA(hWnd, msg, wParam, lParam);
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
			return 0;
		}

		Window::Window(std::string name, int x, int y, int width, int height)
		{
			this->window = CreateWindowA(AGE_WINDOW_CLASS, name.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, NULL, NULL);
			if (this->window == NULL) {
				MessageBoxA(NULL, "CreateWindow() failed: Cannot create a window.", "Error", MB_OK);
			}
			this->dc = GetDC((HWND)(this->window));

			PIXELFORMATDESCRIPTOR pfd = { 0 };
			memset(&pfd, 0, sizeof(pfd));
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cAlphaBits = 8;

			int pf = ChoosePixelFormat((HDC)(this->dc), &pfd);
			if (!pf) {
				MessageBoxA(NULL, "ChoosePixelFormat() failed: Cannot find a suitable pixel format.", "Error", MB_OK);
			}
			if (!SetPixelFormat((HDC)(this->dc), pf, &pfd)) {
				MessageBoxA(NULL, "SetPixelFormat() failed: Cannot set format specified.", "Error", MB_OK);
			}

			DescribePixelFormat((HDC)(this->dc), pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
			this->rc = wglCreateContext((HDC)(this->dc));

			this->shouldClose = false;
			this->visible = false;
			this->fullscreen = false;
			this->savedParams = new WinParams();

			WinParams* saved = (WinParams*)(this->savedParams);
			saved->maximized = !!IsZoomed((HWND)(this->window));
			if (saved) SendMessageA((HWND)(this->window), WM_SYSCOMMAND, SC_RESTORE, 0);
			saved->style = GetWindowLongA((HWND)(this->window), GWL_STYLE);
			saved->styleEx = GetWindowLongA((HWND)(this->window), GWL_EXSTYLE);
			GetWindowRect((HWND)(this->window), &(saved->rect));

			windowMap[(HWND)(this->window)] = this;

			this->OnMouseMove = NULL;
			this->OnMouseScroll = NULL;
			this->OnMouseButtonDown = NULL;
			this->OnMouseButtonUp = NULL;
			this->OnKeyDown = NULL;
			this->OnKeyUp = NULL;
			this->OnCharPrint = NULL;
		}

		Window::~Window()
		{
			if (current == this)
			{
				current = NULL;
				wglMakeCurrent(NULL, NULL);
			}
			windowMap.erase((HWND)(this->window));
			ReleaseDC((HWND)(this->window), (HDC)(this->dc));
			wglDeleteContext((HGLRC)(this->rc));
			DestroyWindow((HWND)(this->dc));
		}

		void Window::SetFullscreen(bool fullscreen)
		{
			if (this->fullscreen == fullscreen) return;
			WinParams* saved = (WinParams*)(this->savedParams);
			if (!(this->fullscreen))
			{
				saved->maximized = !!IsZoomed((HWND)(this->window));
				if (saved) SendMessageA((HWND)(this->window), WM_SYSCOMMAND, SC_RESTORE, 0);
				saved->style = GetWindowLongA((HWND)(this->window), GWL_STYLE);
				saved->styleEx = GetWindowLongA((HWND)(this->window), GWL_EXSTYLE);
				GetWindowRect((HWND)(this->window), &(saved->rect));
			}
			this->fullscreen = fullscreen;
			if (this->fullscreen)
			{
				SetWindowLongA((HWND)(this->window), GWL_STYLE, saved->style & ~(WS_OVERLAPPEDWINDOW));
				SetWindowLongA((HWND)(this->window), GWL_EXSTYLE, (saved->styleEx | WS_EX_TOPMOST) & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
				MONITORINFO monitor_info;
				monitor_info.cbSize = sizeof(monitor_info);
				GetMonitorInfo(MonitorFromWindow((HWND)(this->window), MONITOR_DEFAULTTONEAREST), &monitor_info);
				RECT wrect = monitor_info.rcMonitor;
				SetWindowPos((HWND)(this->window), NULL, wrect.left, wrect.top, wrect.right - wrect.left, wrect.bottom - wrect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongA((HWND)(this->window), GWL_STYLE, saved->style);
				SetWindowLongA((HWND)(this->window), GWL_EXSTYLE, saved->styleEx);
				RECT wrect = saved->rect;
				SetWindowPos((HWND)(this->window), NULL, wrect.left, wrect.top, wrect.right - wrect.left, wrect.bottom - wrect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				if (saved->maximized) SendMessage((HWND)(this->window), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
		}

		bool Window::IsFullscreen()
		{
			return this->fullscreen;
		}

		void Window::SetVisible(bool visible)
		{
			if (this->visible == visible) return;
			this->visible = visible;
			if (this->visible) ShowWindow((HWND)(this->window), SW_SHOW);
			else ShowWindow((HWND)(this->window), SW_HIDE);
		}

		bool Window::IsVisible()
		{
			return this->visible;
		}

		void Window::HandleEvents()
		{
			if (this->shouldClose) return;
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

		void Window::Close()
		{
			CloseWindow((HWND)(this->window));
		}

		void Window::OnClose()
		{
			this->shouldClose = true;
		}

		bool Window::ShouldClose()
		{
			return this->shouldClose;
		}

		Rectangle Window::GetInnerBounds()
		{
			RECT rect = { 0 };
			GetClientRect((HWND)(this->window), &rect);
			return { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
		}

		Rectangle Window::GetBounds()
		{
			RECT rect = { 0 };
			GetWindowRect((HWND)(this->window), &rect);
			return { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
		}

		void Window::SetBounds(Rectangle bounds)
		{
			WinParams* saved = (WinParams*)(this->savedParams);
			if (!(this->fullscreen))
			{
				saved->maximized = !!IsZoomed((HWND)(this->window));
				if (saved) SendMessageA((HWND)(this->window), WM_SYSCOMMAND, SC_RESTORE, 0);
				saved->style = GetWindowLongA((HWND)(this->window), GWL_STYLE);
				saved->styleEx = GetWindowLongA((HWND)(this->window), GWL_EXSTYLE);
				GetWindowRect((HWND)(this->window), &(saved->rect));
			}
			saved->rect.left = bounds.x;
			saved->rect.top = bounds.y;
			saved->rect.right = bounds.x + bounds.width;
			saved->rect.bottom = bounds.y + bounds.height;
			if (!(this->fullscreen))
			{
				if (saved->maximized) SendMessageA((HWND)(this->window), WM_SYSCOMMAND, SC_RESTORE, 0);
				SetWindowPos((HWND)(this->window), NULL, bounds.x, bounds.y, bounds.width, bounds.height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}
			saved->maximized = false;
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

		bool Window::SwapBuffers()
		{
			return WinSwapBuffers((HDC)(this->dc));
		}
	};
};
