#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Dwmapi.lib")

#include "EternalNightTool.h"
#include "AboutDialog.h"
#include "Extras.h"
#include "resource.h"
#include <commctrl.h>
#include <dwmapi.h>
#include <stdlib.h>
#include <EternalNightCommon.h>

HWND h_status_bar;
HWND h_tree_view;
wchar_t* diablo_install_path;



int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, const int nShowCmd)
{
    diablo_install_path = read_registry_string(HKEY_CURRENT_USER, L"SOFTWARE\\Blizzard Entertainment\\Diablo II", L"InstallPath");
    if (diablo_install_path == NULL)
    {
        diablo_install_path = read_registry_string(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Blizzard Entertainment\\Diablo II", L"InstallPath");
    }
    if (diablo_install_path == NULL) {
        MessageBox(NULL, L"Diablo II not installed", L"Error", MB_ICONERROR | MB_OK);
        return -1;
    }
    const wchar_t class_name[] = L"EternalNightTool";
    WNDCLASS wc = {};

    wc.lpfnWndProc = window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = class_name;

    RegisterClass(&wc);

    const HMENU h_menu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU_MAIN));

    const HWND hwnd = CreateWindowEx(
        0,
        class_name,
        L"Eternal Night Tool",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 600,
        NULL, // Parent window
        h_menu, // Menu
        hInstance,
        NULL // Additional application data
    );

    if (hwnd == NULL) {
      MessageBox(NULL, L"Window Creation Failed!", L"Error!",
                 MB_ICONEXCLAMATION | MB_OK);
      return -1;
    }

    enc_initialize_library();
    extras_init();

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    extras_cleanup();
    enc_finalize_library();

    return 0;
}

LRESULT CALLBACK window_proc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        h_status_bar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hwnd, NULL, GetModuleHandle(NULL), NULL);
        h_tree_view = CreateWindowEx(0, WC_TREEVIEW, L"TreeView", WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS, 0, 0, 0, 0, hwnd, NULL, GetModuleHandle(NULL), NULL);

        intialize_main_treeview();
        return 0;
    }
    case WM_DESTROY:
    {
        if (diablo_install_path != NULL)
        {
            free(diablo_install_path);
        }
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE:
    {
        SendMessage(h_status_bar, WM_SIZE, 0, 0);
        RECT rc, tool_rect;
        GetClientRect(hwnd, &rc);
        GetWindowRect(h_status_bar, &tool_rect);
        const int toolbar_height = tool_rect.bottom - tool_rect.top;
        SetWindowPos(h_tree_view, NULL, 0, 0, rc.right, rc.bottom - toolbar_height, SWP_NOZORDER);

        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        const HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            PostQuitMessage(0);
            break;
        case ID_HELP_ABOUTETERNALNIGHT:
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwnd, about_dialog_proc);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void intialize_main_treeview(void)
{
    // Clear existing items
    TreeView_DeleteAllItems(h_tree_view);

    // Add root node
    TVINSERTSTRUCT tvis = {};
    tvis.hParent = NULL;
    tvis.hInsertAfter = TVI_ROOT;
    tvis.item.mask = TVIF_TEXT;
    tvis.item.pszText = L"Virtual File System";
    TreeView_InsertItem(h_tree_view, &tvis);
}
