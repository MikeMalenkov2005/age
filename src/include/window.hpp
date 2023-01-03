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
			void *window, *dc, *rc;
			bool fullscreen;
			bool visible;
			bool shouldClose;
			void UpdateWindowParams();
		public:
			Window(std::string name, int x, int y, int width, int height);
			~Window();
			void SetFullscreen(bool fullscreen);
			bool IsFullscreen();
			void SetVisible(bool visible);
			bool IsVisible();
			void HandleEvents();
			bool ShouldClose();
			GLenum MakeContextCurrent();
			void virtual OnMouseMove(int x, int y, int mode);
			void virtual OnMouseScroll(int x, int y, int scroll, int mode);
			void virtual OnMouseButtonDown(int x, int y, int button, int mode);
			void virtual OnMouseButtonUp(int x, int y, int button, int mode);
			void virtual OnKeyDown(int keyCode, bool alt);
			void virtual OnKeyUp(int keyCode, bool alt);
			void virtual OnCharPrint(int charCode, bool alt);
		};
	}
}

#endif
