#include <windows.h>
#include <tchar.h>
#include "nwpwin.h"
#include "nwpdlg.h"

class size_dialog : public vsite::nwp::dialog {
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};

class main_window : public vsite::nwp::window {
private:
	int board_width = 8;
	int board_height = 8;
	COLORREF color1 = RGB(255, 255, 0);
	COLORREF color2 = RGB(255, 255, 255);

public:
	int get_board_width() const { return board_width; }
	int get_board_height() const { return board_height; }
	void set_board_width(int width) { board_width = width; }
	void set_board_height(int height) { board_height = height; }
	COLORREF get_color1() const { return color1; }
	void set_color1(COLORREF color) { color1 = color; }

protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};