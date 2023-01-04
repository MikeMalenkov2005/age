#ifndef AGE_WINDOW_HPP
#define AGE_WINDOW_HPP

#include "agedef.hpp"

namespace age
{
	namespace wnd {
		int AGE_API Init();

		struct Rectangle
		{
			int x, y, width, height;
		};

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
			Rectangle GetBounds();
			void SetBounds(Rectangle bounds);
			GLenum MakeContextCurrent();
			void (*OnMouseMove)(Window* window, int x, int y, int mode);
			void (*OnMouseScroll)(Window* window, int x, int y, int scroll, int mode);
			void (*OnMouseButtonDown)(Window* window, int x, int y, int button, int mode);
			void (*OnMouseButtonUp)(Window* window, int x, int y, int button, int mode);
			void (*OnKeyDown)(Window* window, int keyCode, bool alt);
			void (*OnKeyUp)(Window* window, int keyCode, bool alt);
			void (*OnCharPrint)(Window* window, int charCode, bool alt);
		};
	}
}

#endif
