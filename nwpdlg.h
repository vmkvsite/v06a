#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
typedef std::basic_string<TCHAR> tstring;

namespace vsite::nwp {

class dialog
{
	HWND hw;
public:
	static INT_PTR CALLBACK proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	operator HWND() const { return hw; }
	int do_modal(HINSTANCE, HWND parent = 0);
protected:
//	operations
	void set_int(int id_ctrl, int val);
	int get_int(int id_ctrl); // throws std::runtime_error
	void set_text(int id_ctrl, const tstring& val);
	tstring get_text(int id_ctrl);
	void set_real(int id_ctrl, double val); // throws std::runtime_error
	double get_real(int id_ctrl);

//	overridables
	virtual int idd() const = 0;
	virtual bool on_init_dialog() { return false; }
	virtual bool on_command(int /*id*/, int /*code*/) { return false; }
	virtual bool on_ok() { return true; }
	virtual void on_cancel() { }
};

} // namespace
