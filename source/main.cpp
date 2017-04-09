#include<Windows.h>
#include <stdio.h>
#include <algorithm>  
#include <string>
#include "res.h"
#include <cstdlib>
#include <ctime>
enum wynikGry
{
	wygrywaKolko,
	wygrywaKrzyzyk,
	remis,
	nierozstrzygniety
};
bool isGameOn = false;
bool isFirstPlayerTurn = true;
int Array[9];
CHAR Text[20];
void f(HWND hwndDig, WPARAM wParam);
void setMotion(bool &isFirstPlayerTurn, HWND hwndButton, HWND hwndDig, WPARAM wParam);
int minimax(int board[9], int depth, bool isMax);
int computerMove(int board[9]);
void checGameResult(wynikGry a, HWND hwndStatic2);
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
			case IDC_BUTTON1: case IDC_BUTTON2: case IDC_BUTTON3:
			case IDC_BUTTON4: case IDC_BUTTON5: case IDC_BUTTON6:
			case IDC_BUTTON7: case IDC_BUTTON8: case IDC_BUTTON9:f(hwndDig, wParam); return TRUE;
			case IDC_BUTTON11:
				HWND hCtl = GetDlgItem(hwndDig, IDC_CHECK1);
				if (isGameOn == false && a == nierozstrzygniety)
				{
					isGameOn = true;
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
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
					EnableWindow(hCtl, FALSE);
				}
				else
				{
					isGameOn = false;
					a = nierozstrzygniety;
					HWND hwndButton11 = GetDlgItem(hwndDig, IDC_BUTTON11);
					wsprintf(Text, "Start");
					SetWindowText(hwndButton11, Text);
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
					wsprintf(Text, "Gra sie konczy");
					SetWindowText(hwndStatic2, Text);
					EnableWindow(hCtl, TRUE);
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
	HWND hwndCheck = GetDlgItem(hwndDig, IDC_CHECK1);
	if (isGameOn == true)
	{
		if (BST_CHECKED == IsDlgButtonChecked(hwndDig, IDC_CHECK1))
		{
			if (Array[LOWORD(wParam) % 1001] == 0)
			{
			setMotion(isFirstPlayerTurn, hwndButton, hwndDig, wParam);
				int move = computerMove(Array);
				if (a == nierozstrzygniety && move != -1)
				{
					HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1 + move);
					setMotion(isFirstPlayerTurn, hwndButton1, hwndDig, IDC_BUTTON1 + move);
				}
			}
		}
		else
		{
			setMotion(isFirstPlayerTurn, hwndButton, hwndDig, wParam);
		}
	}
	a = CheckResult();
	checGameResult(a, hwndStatic2);
}

wynikGry CheckResult()
{
	for (int i = 0; i < 3; i++)
	{
		if (Array[3 * i] == Array[3 * i + 1] && Array[3 * i] == Array[3 * i + 2])
		{
			if (Array[3 * i] == 1)
				return wygrywaKrzyzyk;
			if (Array[3 * i] == -1)
				return wygrywaKolko;
		}

		if (Array[i] == Array[i + 3] && Array[i] == Array[i + 6])
		{
			if (Array[i] == 1)
				return wygrywaKrzyzyk;
			if (Array[i] == -1)
				return wygrywaKolko;
		}
	}
	if ((Array[0] == Array[4] && Array[0] == Array[8]) || (Array[2] == Array[4] && Array[6] == Array[4]))
	{
		if (Array[4] == -1)
			return wygrywaKolko;
		if (Array[4] == 1)
			return wygrywaKrzyzyk;
	}
	for (int i = 0; i < 9; i++)
		if (Array[i] == 0)
			return nierozstrzygniety;
	return remis;
}

void setMotion(bool &isFirstPlayerTurn, HWND hwndButton, HWND hwndDig, WPARAM wParam)
{
	HWND hCtl1 = GetDlgItem(hwndDig, IDC_RADIO1);
	HWND hCtl2 = GetDlgItem(hwndDig, IDC_RADIO2);
	if (Array[LOWORD(wParam) % 1001] == 0)
	{
		if (isFirstPlayerTurn == true)
		{
			wsprintf(Text, "X");
			SetWindowText(hwndButton, Text);
			isFirstPlayerTurn = !isFirstPlayerTurn;
			Array[LOWORD(wParam) % 1001] = 1;
			CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
			EnableWindow(hCtl1, FALSE);
			EnableWindow(hCtl2, TRUE);
		}
		else
		{
			wsprintf(Text, "O");
			SetWindowText(hwndButton, Text);
			isFirstPlayerTurn = !isFirstPlayerTurn;
			Array[LOWORD(wParam) % 1001] = -1;
			CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
			EnableWindow(hCtl2, FALSE);
			EnableWindow(hCtl1, TRUE);
		}
	}
}
void checGameResult(wynikGry a, HWND hwndStatic2)
{
	switch (a)
	{
	case wygrywaKolko:
		wsprintf(Text, "Wygrywa kolko");
		SetWindowText(hwndStatic2, Text);
		//isGameOn = false;
		a = nierozstrzygniety;
		break;
	case wygrywaKrzyzyk:
		wsprintf(Text, "Wygrywa krzyzyk");
		SetWindowText(hwndStatic2, Text);
		//isGameOn = false;
		a = nierozstrzygniety;
		break;
	case remis:
		wsprintf(Text, "Remis");
		SetWindowText(hwndStatic2, Text);
		//isGameOn = false;
		a = nierozstrzygniety;
		break;
	case nierozstrzygniety:;
	default:;
	}
}
int computerMove(int board[9])
{
	int bestVal = -1000;
	int bestMove = -1;
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 0)
		{
			board[i] = isFirstPlayerTurn == true ? 1 : -1;
			int moveVal = minimax(board, 0, false);
			board[i] = 0;
			if (moveVal > bestVal)
			{
				bestMove = i;
				bestVal = moveVal;
			}
		}
	}
	return bestMove;
}
bool isMovesLeft(int board[9])
{
	for (int i = 0; i<9; i++)
			if (board[i] == 0)
				return true;
	return false;
}
int minimax(int board[9], int depth, bool isMax)
{
	if (isMovesLeft(board) == false)
		return 0;
	if (isMax)
	{
		int best = -1000;
		for (int i = 0; i < 9; i++)
		{
			if (board[i] == 0)
			{
				board[i] = isFirstPlayerTurn == true ? 1 : -1;
				best = max(best, minimax(board, depth + 1, !isMax));
				board[i] = 0;
			}
		}
		return best;
	}
	else
	{
		int best = 1000;
		for (int i = 0; i < 9; i++)
		{
			if (board[i] == 0)
			{
				board[i] = isFirstPlayerTurn == true ? 1 : -1;

				best = min(best, minimax(board, depth + 1, !isMax));
				board[i] = 0;
			}
		}
		return best;
	}
}