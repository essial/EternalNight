#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

extern HBRUSH brush_black;
extern HFONT font_title;
extern HPEN pen_button_highlight;
extern HPEN pen_button_shadow;

void extras_init();
void extras_cleanup();
wchar_t* read_registry_string(const HKEY hKeyRoot, const LPCWSTR subKey, const LPCWSTR valueName);
int config_read_string(const char* valueName, char* data, DWORD dataSize);
int config_write_string(const char* valueName, const char* data);
