#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <Windows.h>

#define APP_NAME	"Window Resizer by Toby Huang"
#define APP_VER		"v1.0.0"

#define SPLIT "-----------------------------"

using namespace std;

struct WndInfo {
	HWND hwnd;
	string desc;
	int width;
	int height;
	WINDOWINFO wndinfo;
};

void plog(string msg)
{
	cout << msg.c_str() << endl;
}

BOOL CALLBACK enumwin_proc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	vector<WndInfo>* infosp = reinterpret_cast<vector<WndInfo>*>(lParam);
	WndInfo info;
	info.hwnd = hwnd;
	char* tstr = new char[256];
	WINDOWINFO wndinfo;
	wndinfo.cbSize = sizeof(WINDOWINFO);
	if (GetWindowText(hwnd, tstr, 256) && GetWindowInfo(hwnd, &wndinfo))
	{
		int w = wndinfo.rcWindow.right - wndinfo.rcWindow.left;
		int h = wndinfo.rcWindow.bottom - wndinfo.rcWindow.top;
		if (w && h)
		{
			info.width = w;
			info.height = h;
			info.desc = string(tstr);
			info.wndinfo = wndinfo;
			infosp->push_back(info);
		}
	}
	delete[] tstr;
	return TRUE;
}

vector<WndInfo> get_windows()
{
	vector<WndInfo> infos;
	EnumWindows(&enumwin_proc, reinterpret_cast<LPARAM>(&infos));
	return infos;
}

void print_window(WndInfo i)
{
	cout << setw(12) << left << to_string((int)i.hwnd);  //HWND
	cout << setw(12) << left << to_string(i.width) + "*" + to_string(i.height);  //Size
	cout << setw(12) << left << i.desc;  //Title
	cout << endl;
}

void print_windows(vector<WndInfo> infos)
{
	cout << SPLIT << endl;
	cout << setw(12) << left << "HWND";
	cout << setw(12) << left << "Size";
	cout << setw(12) << left << "Title";
	cout << endl << SPLIT << endl;
	for (auto i : infos)
	{
		print_window(i);
	}
	cout << SPLIT << endl;
}

bool resize_window(HWND hwnd)
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE) | WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	if (style)
	{
		return SetWindowLong(hwnd, GWL_STYLE, style) &&
			SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else {
		return false;
	}
}

int main()
{

	plog(APP_NAME + string(" ") + APP_VER);
	vector<WndInfo> infos = get_windows();
	if (infos.size() > 0)
	{
		print_windows(infos);
	}
	else {
		plog("Error while enumerating windows.");
	}
	plog("HWND to resize: ");
	int hwnd_i;
	cin >> hwnd_i;
	bool r = resize_window((HWND)hwnd_i);
	if (r)
	{
		plog("Window successfully modified.");
	}
	else
	{
		plog("Error while modifying: code " + to_string(GetLastError()));
	}
}