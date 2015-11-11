#pragma once

#include <windows.h>
#include <string>
#include <tchar.h>

using tstring = std::basic_string<TCHAR>;

namespace vsite::nwp {

class application
{
public:
	int run();
};

class window
{
	HWND hw{ 0 };

protected:
	virtual tstring class_name();
	bool register_class(const tstring& name);
	tstring generate_class_name();

public:
	bool create(HWND parent, DWORD style, LPCTSTR caption=0, UINT_PTR id_or_menu=0,
		int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int width=CW_USEDEFAULT, int height=CW_USEDEFAULT);

	operator HWND();
	static LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//	messages
protected:
	virtual int on_create(CREATESTRUCT*) { return 0; }
	virtual void on_command(int) { }
	virtual void on_destroy() { }

	virtual void on_key_down(int) { }
	virtual void on_key_up(int) { }
	virtual void on_char(TCHAR) { }
	virtual void on_left_button_down(POINT) { }
	virtual void on_right_button_down(POINT) { }
	virtual void on_left_button_up(POINT) { }
	virtual void on_left_button_double_click(POINT) { }

	virtual void on_timer(int) { }

	virtual void on_paint(HDC) { }
};

} // namespace
