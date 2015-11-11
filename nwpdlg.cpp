#include "nwpdlg.h"
#include <map>
#include <stdexcept>
#include <format>

namespace vsite::nwp {

INT_PTR CALLBACK dialog::proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	static std::map<HWND, dialog*> wmap;
	if(msg == WM_INITDIALOG)
	{
		dialog* pd = reinterpret_cast<dialog*>(lp);
		pd->hw = hw;
		wmap[hw] = pd; 
		return pd->on_init_dialog();
	}
	dialog* pd = wmap[hw];
	if(msg == WM_COMMAND)
	{
		if(LOWORD(wp) == IDOK && pd->on_ok())
			return ::EndDialog(hw, IDOK);
		if(LOWORD(wp) == IDCANCEL)
		{
			pd->on_cancel();
			return ::EndDialog(hw, IDCANCEL);
		}
		return pd->on_command(LOWORD(wp), HIWORD(wp));
	}
	if (msg == WM_DESTROY)
		wmap.erase(hw);
	return 0;
}

int dialog::do_modal(HINSTANCE hInst, HWND parent)
{
	return (int)::DialogBoxParam(hInst, MAKEINTRESOURCE(idd()), parent, proc, 
		reinterpret_cast<LPARAM>(this));
}

void dialog::set_int(int id_ctrl, int val)
{
	::SetDlgItemInt(*this, id_ctrl, val, true);
}

int dialog::get_int(int id_ctrl)
{
	BOOL b;
	int n = ::GetDlgItemInt(*this, id_ctrl, &b, true);
	if(!b) throw std::runtime_error("not a number");
	return n;
}

void dialog::set_text(int id_ctrl, const tstring& s)
{
	::SetDlgItemText(*this, id_ctrl, s.c_str());
}

tstring dialog::get_text(int id_ctrl)
{
    TCHAR s[512];
	::GetDlgItemText(*this, id_ctrl, s, 512);
	return s;
}

void dialog::set_real(int id_ctrl, double d)
{
	::SetDlgItemText(*this, id_ctrl, std::format(_T("{}"), d).c_str());
}

double dialog::get_real(int id_ctrl)
{
	TCHAR s[16], *c;
	::GetDlgItemText(*this, id_ctrl, s, 16);
	double n = _tcstod(s, &c);
	if(c == s) throw std::runtime_error("not a number");
	return n;
}

} // namespace
