#include "nwpwin.h"
#include <windowsx.h>
#include <format>

namespace {
POINT get_point(LPARAM lp)
{
	POINT p;
	p.x = GET_X_LPARAM(lp);
	p.y = GET_Y_LPARAM(lp);
	return p;
}
} // namespace

namespace vsite::nwp {

int application::run()
{
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0)){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

tstring window::class_name()
{
	return {};
}

bool window::register_class(const tstring& name)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = proc;
	wc.lpszClassName = name.c_str();
	wc.cbWndExtra = sizeof(window*);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

	return ::RegisterClass(&wc) != 0;
}

tstring window::generate_class_name()
{
	static int n = 1;
	return std::format(_T("NWP{}"), n++);
}

bool window::create(HWND parent, DWORD style, LPCTSTR caption, UINT_PTR id_or_menu,
	int x, int y, int width, int height)
{
	tstring cn = class_name();
	if (cn.empty())
		register_class(cn = generate_class_name());
	hw = ::CreateWindow(cn.c_str(), caption, style, x, y, width, height, parent, (HMENU)id_or_menu, 0, this);
	return hw != 0;
}

window::operator HWND()
{
	return hw;
}

LRESULT CALLBACK window::proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	if(msg == WM_CREATE){
		CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lp);
		window* pw = reinterpret_cast<window*>(pcs->lpCreateParams);
		::SetWindowLongPtr(hw, 0, reinterpret_cast<LONG_PTR>(pw));
		pw->hw = hw;
		return pw->on_create(pcs);
	}

	window* pw = reinterpret_cast<window*>(::GetWindowLongPtr(hw, 0));
	switch (msg)
	{
		case WM_COMMAND:		pw->on_command(LOWORD(wp)); return 0;
		case WM_DESTROY:		pw->on_destroy(); return 0;
		case WM_KEYDOWN:		pw->on_key_down(LOWORD(wp)); return 0;
		case WM_KEYUP:			pw->on_key_up(LOWORD(wp)); return 0;
		case WM_CHAR:			pw->on_char((TCHAR)wp); return 0;
		case WM_LBUTTONDOWN:	pw->on_left_button_down(get_point(lp)); return 0;
		case WM_RBUTTONDOWN:	pw->on_right_button_down(get_point(lp)); return 0;
		case WM_LBUTTONUP: 		pw->on_left_button_up(get_point(lp)); return 0;
		case WM_LBUTTONDBLCLK:	pw->on_left_button_double_click(get_point(lp)); return 0;
		case WM_TIMER:			pw->on_timer(LOWORD(wp)); return 0;
		case WM_PAINT:			
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hw, &ps);
			pw->on_paint(hdc);
			::EndPaint(hw, &ps);
			return 0;

	}
	return ::DefWindowProc(hw, msg, wp, lp);
}

} // namespace
