#include<Windows.h>
#include<stdio.h>
#include "res.h"
#include <cstdlib>
#include <ctime>

bool isGameOn = false;
bool isFirstPlayerTurn = true;
int Array[9];
CHAR Text[20];
void f(HWND hwndDig, WPARAM wParam);
enum wynikGry
{
	wygrywaKolko,
	wygrywaKrzyzyk,
	remis,
	nierozstrzygniety
};
wynikGry CheckResult();
wynikGry a = nierozstrzygniety;
INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON2:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON3:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON4:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON5:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON6:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON7:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON8:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON9:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON11:
				if (isGameOn == false || a != nierozstrzygniety)
				{
					isGameOn = true;
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
					// CHAR Text[500];
					wsprintf(Text, "Gra sie zaczyna");
					SetWindowText(hwndStatic2, Text);
					for (int i = 0; i < 9; i++)
					{
						HWND hwndButton = GetDlgItem(hwndDig, IDC_BUTTON1 + i);
						Array[i] = 0;
						wsprintf(Text, "");
						SetWindowText(hwndButton, Text);
					}
					HWND hwndButton11 = GetDlgItem(hwndDig, IDC_BUTTON11);
					wsprintf(Text, "Stop");
					SetWindowText(hwndButton11, Text);
					CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

				}
				else
				{
					isGameOn = false;
					HWND hwndButton11 = GetDlgItem(hwndDig, IDC_BUTTON11);
					wsprintf(Text, "Start");
					SetWindowText(hwndButton11, Text);
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
					wsprintf(Text, "Gra sie konczy");
					SetWindowText(hwndStatic2, Text);
				}
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	case WM_CLOSE:
		DestroyWindow(hwndDig);
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLIne, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}


void f(HWND hwndDig, WPARAM wParam)
{
	HWND hwndButton = GetDlgItem(hwndDig, LOWORD(wParam)), hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
	//CHAR Text[500];
	if (isGameOn == true)
	{
		if (GetWindowTextLength(hwndButton) == 0)
		{
			if (isFirstPlayerTurn == true)
			{
				wsprintf(Text, "X");
				SetWindowText(hwndButton, Text);
				isFirstPlayerTurn = false;
				Array[LOWORD(wParam) % 1001] = 1;
				CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
			}
			else
			{
				wsprintf(Text, "O");
				SetWindowText(hwndButton, Text);
				isFirstPlayerTurn = true;
				Array[LOWORD(wParam) % 1001] = 2;
				CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
			}
		}
		a = CheckResult();
		switch (a)
		{
		case wygrywaKolko:
			wsprintf(Text, "Wygrywa kolko");
			SetWindowText(hwndStatic2, Text);
			isGameOn = false;
			break;
		case wygrywaKrzyzyk:
			wsprintf(Text, "Wygrywa krzyzyk");
			SetWindowText(hwndStatic2, Text);
			isGameOn = false;
			break;
		case remis:
			wsprintf(Text, "Remis");
			SetWindowText(hwndStatic2, Text);
			isGameOn = false;
			break;
		case nierozstrzygniety:;
		default:;
		}
	}
}
wynikGry CheckResult()
{
	for (int i = 0; i < 3; i++)
	{
		if (Array[3 * i] == Array[3 * i + 1] && Array[3 * i] == Array[3 * i + 2])
		{
			if (Array[3 * i] == 1)
				return wygrywaKrzyzyk;
			if (Array[3 * i] == 2)
				return wygrywaKolko;
		}

		if (Array[i] == Array[i + 3] && Array[i] == Array[i + 6])
		{
			if (Array[i] == 1)
				return wygrywaKrzyzyk;
			if (Array[i] == 2)
				return wygrywaKolko;
		}
	}
	if ((Array[0] == Array[4] && Array[0] == Array[8]) || (Array[2] == Array[4] && Array[6] == Array[4]))
	{
		if (Array[4] == 2)
			return wygrywaKolko;
		if (Array[4] == 1)
			return wygrywaKrzyzyk;
	}
	for (int i = 0; i < 9; i++)
		if (Array[i] == 0)
			return nierozstrzygniety;
	return remis;
}