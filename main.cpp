#include "main.h"
#include "rc.h"

main_window* g_main_window = nullptr;

int size_dialog::idd() const {
	return IDD_SIZE;
}

bool size_dialog::on_init_dialog() {
	if (g_main_window) {
		set_int(IDC_EDIT1, g_main_window->get_board_width());
		set_int(IDC_EDIT2, g_main_window->get_board_height());
	}
	return true;
}

bool size_dialog::on_ok() {
	try {
		int new_width = get_int(IDC_EDIT1);
		int new_height = get_int(IDC_EDIT2);

		if (new_width > 0 && new_height > 0 && new_width <= 20 && new_height <= 20) {
			if (g_main_window) {
				g_main_window->set_board_width(new_width);
				g_main_window->set_board_height(new_height);
			}
			return true;
		}
	}
	catch (...) {
	}
	return false;
}

void main_window::on_paint(HDC hdc) {
	RECT client_rect;
	GetClientRect(*this, &client_rect);

	int cell_width = (client_rect.right - client_rect.left) / board_width;
	int cell_height = (client_rect.bottom - client_rect.top) / board_height;

	HBRUSH brush1 = CreateSolidBrush(color1);
	HBRUSH brush2 = CreateSolidBrush(color2);

	for (int row = 0; row < board_height; row++) {
		for (int col = 0; col < board_width; col++) {
			HBRUSH current_brush = ((row + col) % 2 == 0) ? brush1 : brush2;
			HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);

			RECT cell_rect;
			cell_rect.left = col * cell_width;
			cell_rect.top = row * cell_height;
			cell_rect.right = (col + 1) * cell_width;
			cell_rect.bottom = (row + 1) * cell_height;

			FillRect(hdc, &cell_rect, current_brush);

			SelectObject(hdc, old_brush);
		}
	}

	DeleteObject(brush1);
	DeleteObject(brush2);
}

void main_window::on_command(int id) {
	switch (id) {
	case ID_SIZE:
	{
		size_dialog dlg;
		if (dlg.do_modal(GetModuleHandle(NULL), *this) == IDOK) {
			InvalidateRect(*this, NULL, TRUE);
		}
	}
	break;
	case ID_COLOR:
	{
		CHOOSECOLOR cc = {};
		static COLORREF custom_colors[16] = {};

		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = *this;
		cc.rgbResult = color1;
		cc.lpCustColors = custom_colors;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (ChooseColor(&cc)) {
			color1 = cc.rgbResult;
			InvalidateRect(*this, NULL, TRUE);
		}
	}
	break;
	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void main_window::on_destroy() {
	::PostQuitMessage(0);
}

int WINAPI WinMain(_In_ HINSTANCE hi, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	vsite::nwp::application app;
	main_window wnd;
	g_main_window = &wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (UINT_PTR)LoadMenu(hi, MAKEINTRESOURCE(IDM_MAIN)));
	int result = app.run();
	g_main_window = nullptr;
	return result;
}