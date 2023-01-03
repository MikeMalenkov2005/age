#ifndef AGE_WINDOW_HPP
#define AGE_WINDOW_HPP

#include "agedef.hpp"

namespace age
{
	namespace wnd {
		int AGE_API Init();

		class AGE_API Window
		{
		private:
			void *window, *dc, *rc, *savedParams;
			bool fullscreen;
			bool visible;
			bool shouldClose;
		public:
			Window(std::string name, int x, int y, int width, int height);
			~Window();
			void SetFullscreen(bool fullscreen);
			bool IsFullscreen();
			void SetVisible(bool visible);
			bool IsVisible();
			void HandleEvents();
			void Close();
			void OnClose();
			bool ShouldClose();
			GLenum MakeContextCurrent();
			void (*OnMouseMove)(int x, int y, int mode);
			void (*OnMouseScroll)(int x, int y, int scroll, int mode);
			void (*OnMouseButtonDown)(int x, int y, int button, int mode);
			void (*OnMouseButtonUp)(int x, int y, int button, int mode);
			void (*OnKeyDown)(int keyCode, bool alt);
			void (*OnKeyUp)(int keyCode, bool alt);
			void (*OnCharPrint)(int charCode, bool alt);
		};
	}
}

#endif
