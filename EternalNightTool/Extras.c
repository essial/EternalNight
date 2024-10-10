#include "Extras.h"

#include <stdlib.h>

HBRUSH brush_black;
HFONT font_title;
HPEN pen_button_highlight;
HPEN pen_button_shadow;

void extras_init()
{
    brush_black = CreateSolidBrush(RGB(0, 0, 0));
    font_title = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial");
    pen_button_highlight = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
    pen_button_shadow = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
}

void extras_cleanup()
{
    DeleteObject(brush_black);
    DeleteObject(font_title);
    DeleteObject(pen_button_highlight);
    DeleteObject(pen_button_shadow);
}

wchar_t* read_registry_string(const HKEY hKeyRoot, const LPCWSTR subKey, const LPCWSTR valueName)
{
    HKEY hKey;
    DWORD dataSize = 0;
    DWORD valueType;

    if (RegOpenKeyExW(hKeyRoot, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExW(hKey, valueName, NULL, &valueType, NULL, &dataSize) == ERROR_SUCCESS && valueType == REG_SZ)
        {
            wchar_t* valueData = malloc(dataSize);
            if (valueData)
            {
                if (RegQueryValueExW(hKey, valueName, NULL, &valueType, (LPBYTE)valueData, &dataSize) == ERROR_SUCCESS)
                {
                    RegCloseKey(hKey);
                    return valueData;
                }
                free(valueData);
            }
        }
        RegCloseKey(hKey);
    }
    return NULL;
}

int config_read_string(const char* valueName, char* data, DWORD dataSize)
{
    HKEY key;
    const char* subkey = "SOFTWARE\\EternalNight\\Tool";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_READ, &key) != ERROR_SUCCESS)
    {
        return -1;
    }

    if (RegQueryValueExA(key, valueName, NULL, NULL, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
    {
        RegCloseKey(key);
        return -2;
    }

    RegCloseKey(key);
    return 0;
}

int config_write_string(const char* valueName, const char* data)
{
    HKEY key;
    const char* subkey = "SOFTWARE\\EternalNight\\Tool";

    if (RegCreateKeyExA(HKEY_CURRENT_USER, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL) != ERROR_SUCCESS)
    {
        return -1;
    }

    if (RegSetValueExA(key, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1) != ERROR_SUCCESS)
    {
        RegCloseKey(key);
        return -2;
    }

    RegCloseKey(key);
    return 0;
}
