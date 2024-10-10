#pragma once
#include "AboutDialog.h"
#include "Extras.h"
#include "resource.h"

INT_PTR CALLBACK about_dialog_proc(const HWND hDlg, const UINT message, const WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG:
    {
        SendMessage(GetDlgItem(hDlg, IDC_STATIC_LOGO_TEXT), WM_SETFONT, (WPARAM)font_title, TRUE);
        return TRUE;
    }
    case WM_CTLCOLORSTATIC:
    {
        const HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(255, 255, 255));
        SetBkMode(hdcStatic, TRANSPARENT);
        return (INT_PTR)brush_black;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT rcFill;
        GetClientRect(hDlg, &rcFill);
        rcFill.bottom -= 46;
        const HDC hdc = BeginPaint(hDlg, &ps);
        FillRect(hdc, &ps.rcPaint, brush_black);

        // Draw button shadow at the top
        SelectObject(hdc, pen_button_shadow);
        MoveToEx(hdc, 0, rcFill.top, NULL);
        LineTo(hdc, ps.rcPaint.right, rcFill.top);

        // Fill bottom part of the dialog
        rcFill.top = rcFill.bottom + 1;
        rcFill.bottom = ps.rcPaint.bottom;
        FillRect(hdc, &rcFill, (HBRUSH)(COLOR_BTNFACE + 1));

        // Draw button highlight line using windows button highlight color
        SelectObject(hdc, pen_button_highlight);
        MoveToEx(hdc, 0, rcFill.top, NULL);
        LineTo(hdc, ps.rcPaint.right, rcFill.top);



        EndPaint(hDlg, &ps);
        return TRUE;
    }
    
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        return FALSE;
    }
    default:
        return FALSE;
    }
}
