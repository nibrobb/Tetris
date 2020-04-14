#include <iostream>
#include <Windows.h>
#include <thread>
using namespace std;


wstring tetramino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

int nScreenWidth = 120;		// Console screen size X (columns)
int nScreenHeight = 30;		// Console screen size Y


int Rotate(int px, int py, int r)
{
	switch (r % 4)
	{
	case 0: return py * 4 + px;			// 0 degrees
	case 1: return 12 + py - (px * 4);	// 90 degrees
	case 2: return 15 - (py * 4) - px;	// 180 degrees
	case 3: return 3 - py + +(px * 4);	// 270 degrees
	}
	return 0;
}

bool DoesPieceFit(int nTetramino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index info field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + px < nFieldHeight)
				{
					if (tetramino[nTetramino][pi] == L'X' && pField[fi] != 0)
						return false; // Fail on first hit
				}
			}
		}

	return true;
}


int main()
{
	//Create assets
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");

	tetramino[1].append(L"..X.");
	tetramino[1].append(L".XX.");
	tetramino[1].append(L".X..");
	tetramino[1].append(L"....");

	tetramino[2].append(L".X..");
	tetramino[2].append(L".XX.");
	tetramino[2].append(L"..X.");
	tetramino[2].append(L"....");

	tetramino[3].append(L"....");
	tetramino[3].append(L".XX.");
	tetramino[3].append(L".XX.");
	tetramino[3].append(L"....");

	tetramino[4].append(L"..X.");
	tetramino[4].append(L".XX.");
	tetramino[4].append(L"..X.");
	tetramino[4].append(L"....");

	tetramino[5].append(L"....");
	tetramino[5].append(L".XX.");
	tetramino[5].append(L"..X.");
	tetramino[5].append(L"..X.");

	tetramino[6].append(L"....");
	tetramino[6].append(L".XX.");
	tetramino[6].append(L".X..");
	tetramino[6].append(L".X..");

	pField = new unsigned char[nFieldWidth * nFieldHeight]; //Create playing-field
	for (int x = 0; x < nFieldWidth; x++) // Bord boundry
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	bool bGameOver = false;


	// TEST ==================================
	int nCurrentPiece = 4;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];

	// TEST ==================================

	while (!bGameOver)
	{
		// GAME TIMING =================================
		this_thread::sleep_for(50ms);


		// INPUT =======================================
		for (int k = 0; k < 4; k++)								// R	L	D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// GAME LOGIC ==================================
		if (bKey[1])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
			{
				nCurrentX = nCurrentX - 1;
			}
		}

		if (bKey[0])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
			{
				nCurrentX = nCurrentX + 1;
			}
		}

		// RENDER OUTPUT ================================


		// Draw field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=║"[pField[y*nFieldWidth + x]];


		// Draw current piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetramino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; // 65 for ACSII A


		// Display frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
	}


	return 0;
}