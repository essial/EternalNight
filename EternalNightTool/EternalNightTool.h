#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void intialize_main_treeview(void);
