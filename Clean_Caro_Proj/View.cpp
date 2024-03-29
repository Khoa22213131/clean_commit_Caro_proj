﻿#include "View.h"


//Unicode UTF8 char lib 
#include <fcntl.h>
#include <io.h>

using namespace std;

void FixConsoleWindow()
{
	// [Disable resize property of console]
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);

	ModifyConsoleSize(170, 40);
	// [REMOVE THE SCROLL BAR]
	// get handle to the console window
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// retrieve screen buffer info
	CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
	GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
	// current window size
	short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
	short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
	// current screen buffer size
	short scrBufferWidth = scrBufferInfo.dwSize.X;
	short scrBufferHeight = scrBufferInfo.dwSize.Y;
	// to remove the scrollbar, make sure the window height matches the screen buffer height
	COORD newSize;
	newSize.X = scrBufferWidth;
	newSize.Y = winHeight;
	// set the new screen buffer dimensions
	int Status = SetConsoleScreenBufferSize(hOut, newSize);
	if (Status == 0)
	{
		cout << "SetConsoleScreenBufferSize() failed! Reason : " << GetLastError() << endl;
		exit(Status);
	}

}

void ModifyConsoleSize(int width, int height) {
	// Create a SMALL_RECT structure to define the new console size
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = width - 1;
	rect.Bottom = height - 1;

	// Set the console screen buffer size to match the new size
	COORD size;
	size.X = width;
	size.Y = height;

	// Set the console window size and screen buffer size
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

}

COORD GetConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); // get console_output_handle
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

int getColor(color bgColor, color fontColor) {
	return 16 * bgColor + fontColor;
};
void changeFontColor(color backgroundColor, color fontColor) {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleOutput, getColor(backgroundColor, fontColor));
}

enum objID {
	main_Logo,
	about_Logo,
	help_Logo,
	button,
	background,
	boardCanvas,
	border,
	playerFrame,
	xMark,
	oMark,
	animation_x,
	animation_o
};


objID string_hash(string const& inString) {
	if (inString == "Main_Logo") return main_Logo;
	if (inString == "About_Logo") return about_Logo;
	if (inString == "Help_Logo") return help_Logo;
	if (inString == "Button") return button;
	if (inString == "Background") return background;
	if (inString == "Border") return border;
	if (inString == "BoardCanvas") return boardCanvas;
	if (inString == "PlayerFrame") return playerFrame;
	if (inString == "WinAnimation_X") return animation_x;
	if (inString == "WinAnimation_O") return animation_o;
}

void Button::printButton() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	/*top layer*/
	GotoXY(coord.X, coord.Y);
	wcout << L"╔═";
	for (int i = 0; i < nameLength; i++) {
		wcout << L"═";
	}
	wcout << L"═╗";

	/*mid layer*/
	GotoXY(coord.X, coord.Y + 1);
	wcout << L"║ ";
	wcout << buttonName;
	wcout << L" ║";

	/*bot layer*/
	GotoXY(coord.X, coord.Y + 2);
	wcout << L"╚═";
	for (int i = 0; i < nameLength; i++) {
		wcout << L"═";
	}
	wcout << L"═╝";
	_setmode(_fileno(stdout), _O_TEXT);
}
void Button::playScene() {
	wstring ws(buttonName);
	string temp(ws.begin(), ws.end());
	SceneHandle(temp);
}


// [define visualizer func]
void Visualizer::printLogo(string str) {
	COORD currentCoord = GetConsoleCursorPosition();
	switch (string_hash(str))
	{
	case main_Logo:
		for (int i = 0; i < sizeof(mainLogo) / sizeof(mainLogo[0]); i++) {
			wcout << mainLogo[i];
			GotoXY(currentCoord.X, currentCoord.Y + 1);
			currentCoord.Y++; //update the new pointer coordinate
		}
		break;
	case about_Logo:
		for (int i = 0; i < sizeof(aboutLogo) / sizeof(aboutLogo[0]); i++) {
			wcout << aboutLogo[i];
			GotoXY(currentCoord.X, currentCoord.Y + 1);
			currentCoord.Y++; //update the new pointer coordinate
		}
		break;
	case help_Logo:
		for (int i = 0; i < sizeof(aboutLogo) / sizeof(helpLogo[0]); i++) {
			wcout << helpLogo[i];
			GotoXY(currentCoord.X, currentCoord.Y + 1);
			currentCoord.Y++; //update the new pointer coordinate
		}
		break;

	default:
		break;
	}
}
void Visualizer::printBackground(int width, int height) {
	SetConsoleTextAttribute(hConsoleOutput, 15);
	//wcout << L"Width" << csbi.dwSize.X << L"Height" << csbi.dwSize.Y << endl; //test width and height of console
	GotoXY(0, 0);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			wcout << fullBlock;
		}
	}
	SetConsoleTextAttribute(hConsoleOutput, 240); // 240 for black font white background

}
void Visualizer::printMenuBorder() {//hard-code [saber]
	_setmode(_fileno(stdout), _O_U16TEXT);
	GotoXY(0, 0);
	wcout << L"╔";
	for (int i = 1; i <= 66; i++) {
		GotoXY(i, 0);
		wcout << L"═";
	}
	wcout << L"╗";
	GotoXY(101, 0);
	wcout << L"╔";
	for (int i = 102; i <= 168; i++) {
		GotoXY(i, 0);
		wcout << L"═";
	}
	wcout << L"╗";

	GotoXY(0, 1);
	wcout << L"║";
	GotoXY(66, 1);
	wcout << L"╔";
	GotoXY(67, 1);
	wcout << L"╝";
	GotoXY(101, 1);
	wcout << L"╚";
	GotoXY(102, 1);
	wcout << L"╗";
	GotoXY(169, 1);
	wcout << L"║";

	for (int i = 2; i <= 5; i++) {
		GotoXY(0, i);
		wcout << L"║";
		GotoXY(66, i);
		wcout << L"║";
		GotoXY(102, i);
		wcout << L"║";
		GotoXY(169, i);
		wcout << L"║";
	}

	GotoXY(0, 6);
	wcout << L"║";
	GotoXY(66, 6);
	wcout << L"╚";
	GotoXY(67, 6);
	wcout << L"╗";
	GotoXY(101, 6);
	wcout << L"╔";
	GotoXY(102, 6);
	wcout << L"╝";
	GotoXY(169, 6);
	wcout << L"║";

	GotoXY(0, 7);
	wcout << L"║";
	GotoXY(67, 7);
	wcout << L"╚";
	for (int i = 68; i <= 100; i++) {
		GotoXY(i, 7);
		wcout << L"═";
	}
	GotoXY(101, 7);
	wcout << L"╝";
	GotoXY(169, 7);
	wcout << L"║";

	for (int i = 8; i <= 38; i++) {
		GotoXY(0, i);
		wcout << L"║";
		GotoXY(169, i);
		wcout << L"║";
	}

	GotoXY(0, 39);
	wcout << L"╚";
	for (int i = 1; i <= 168; i++) {
		GotoXY(i, 39);
		wcout << L"═";
	}
	wcout << L"╝";
	_setmode(_fileno(stdout), _O_TEXT);
}
void Visualizer::printBorder() {
	GotoXY(0, 0);
	wcout << L"╔";
	for (int i = 1; i <= 168; i++) {
		GotoXY(i, 0);
		wcout << L"═";
	}
	wcout << L"╗";
	for (int i = 1; i <= 38; i++) {
		GotoXY(0, i);
		wcout << L"║";
		GotoXY(169, i);
		wcout << L"║";
	}

	GotoXY(0, 39);
	wcout << L"╚";
	for (int i = 1; i <= 168; i++) {
		GotoXY(i, 39);
		wcout << L"═";
	}
	wcout << L"╝";
}
void Visualizer::printPlayerFrame(char str) {
	_setmode(_fileno(stdout), _O_U16TEXT);
	SetConsoleTextAttribute(hConsoleOutput, 240);
	COORD currentPos = GetConsoleCursorPosition();
	int width = 40, height = 31; // ┏━┓ -> width = 1;

	GotoXY(currentPos.X, currentPos.Y); // top left
	wcout << L"┏";
	for (int i = 1; i <= width; i++) { // print column
		GotoXY(currentPos.X + i, currentPos.Y);
		wcout << L"━";
		GotoXY(currentPos.X + i, currentPos.Y + height + 1);
		wcout << L"━";
	}
	GotoXY(currentPos.X + width + 1, currentPos.Y); // top right
	wcout << L"┓";
	for (int i = 1; i <= height; i++) { // print column
		GotoXY(currentPos.X, currentPos.Y + i);
		wcout << L"┃";
		GotoXY(currentPos.X + width + 1, currentPos.Y + i);
		wcout << L"┃";
	}
	GotoXY(currentPos.X, currentPos.Y + height + 1); // bottom left
	wcout << L"┗";
	GotoXY(currentPos.X + width + 1, currentPos.Y + height + 1); // bottom left
	wcout << L"┛";

	switch (str)
	{
	case 'X':
		avtXCoor.X = currentPos.X + (width + 1) / 2 - 6;
		avtXCoor.Y = currentPos.Y + 1;
		GotoXY(avtXCoor.X, avtXCoor.Y);
		printAvatar('X', 1);
		break;
	case 'O':
		avtOCoor.X = currentPos.X + (width + 1) / 2 - 7;
		avtOCoor.Y = currentPos.Y + 1;
		GotoXY(avtOCoor.X, avtOCoor.Y);
		printAvatar('O', 0);
	default:
		break;
	}

	_setmode(_fileno(stdout), _O_TEXT);
}

void Visualizer::printAvatar(char str, int color) {
	_setmode(_fileno(stdout), _O_U16TEXT);
	COORD currentPos;
	switch (str) {
	case 'X':
		currentPos = avtXCoor;
		break;
	case 'O':
		currentPos = avtOCoor;
		break;
	defaut:
		break;
	}
	int colorX, colorO;
	if (color == 0) {
		// define line color here
		colorX = colorO = 248;
	}
	else {
		// define bright color here
		colorX = 244;
		colorO = 241;
	}

	switch (str)
	{
	case 'X':
		SetConsoleTextAttribute(hConsoleOutput, colorX);
		for (int i = 0; i < 8; i++) { // [hard-code]
			GotoXY(currentPos.X, currentPos.Y + i);
			wcout << avt_x[i];
		}
		break;
	case 'O':
		SetConsoleTextAttribute(hConsoleOutput, colorO);
		for (int i = 0; i < 7; i++) { // [hard-code]
			GotoXY(currentPos.X, currentPos.Y + i);
			wcout << avt_o[i];
		}
		break;
	default:
		break;
	}
	_setmode(_fileno(stdout), _O_TEXT);
}


void Visualizer::printBackgroundAnimation(int index, int coorX, int coorY) {
	GotoXY(coorX, coorY);
	if (index / 5 == 0) {
		SetConsoleTextAttribute(hConsoleOutput, 244); //244
		wcout << big_x[index % 5] << L"\n";
	}
	else {
		SetConsoleTextAttribute(hConsoleOutput, 241); //241
		wcout << big_o[index % 5] << L"\n";
	}
}

void Visualizer::printWinAnimation(char avt, int waveWidth, int initNumChar, COORD animPivot) {
	_setmode(_fileno(stdout), _O_U16TEXT);
	int vtcShift = 0, hrztShift = 0; // verticalShift
	switch (avt) {
	case 'X':
			initNumChar += 2;
			if (initNumChar % 20 == 0) {
				initNumChar = 0;
			}
			SetConsoleTextAttribute(hConsoleOutput, 240);
			/*GotoXY(animPivot.X, animPivot.Y);
			for (int j = 0; j < 8; j++) {
				GotoXY(animPivot.X, animPivot.Y + j);
				for (int i = 0; i < 160; i++) {
					wcout << L" ";
				}
			}*/
			GotoXY(animPivot.X, animPivot.Y);
			for (int j = 0; j < 157; j++) {
				wcout << L" ";
			}
			GotoXY(animPivot.X, animPivot.Y + 8);
			for (int j = 0; j < 157; j++) {
				wcout << L" ";
			}


			GotoXY(animPivot.X, animPivot.Y);
			for (int i = 0; i < 8; i++) {// [hard-code]
				SetConsoleTextAttribute(hConsoleOutput, colorArr[(i + initNumChar) % 8]);
				GotoXY(animPivot.X, animPivot.Y + i);
				hrztShift = vtcShift;
				for (int j = 0; j < 157; j++) {// [hard-code]
					if ((j + abs(waveWidth - initNumChar)) % waveWidth == 0) {
						hrztShift = 1 - hrztShift;
						COORD currentPos = GetConsoleCursorPosition();
						GotoXY(currentPos.X, animPivot.Y + i + hrztShift);
					}
					wcout << winnerXBanner[i][j];
				}
			}

			Sleep(100);
		break;
	case 'O':
			initNumChar += 2;
			if (initNumChar % 20 == 0) {
				initNumChar = 0;
			}
			SetConsoleTextAttribute(hConsoleOutput, 240);
			GotoXY(animPivot.X, animPivot.Y);
			for (int j = 0; j < 150; j++) {
				wcout << L" ";
			}
			GotoXY(animPivot.X, animPivot.Y + 8);
			for (int j = 0; j < 150; j++) {
				wcout << L" ";
			}
			GotoXY(animPivot.X, animPivot.Y);
			for (int i = 0; i < 8; i++) {// [hard-code]
				SetConsoleTextAttribute(hConsoleOutput, colorArr[(i + initNumChar) % 8]);
				GotoXY(animPivot.X, animPivot.Y + i);
				hrztShift = vtcShift;
				for (int j = 0; j < 146; j++) {// [hard-code]
					if ((j + (waveWidth - initNumChar)) % waveWidth == 0) {
						hrztShift = 1 - hrztShift;
						COORD currentPos = GetConsoleCursorPosition();
						GotoXY(currentPos.X, animPivot.Y + i + hrztShift);
					}
					wcout << winnerOBanner[i][j];
				}
			}

			Sleep(100);
		break;
	default:
		break;
	}


	_setmode(_fileno(stdout), _O_TEXT);
}

void Visualizer::printButton() { //[saber]
	for (auto i : buttons) {
		i.printButton();
	}
}

void Visualizer::printBoardCanvas(int numCell) {
	SetConsoleTextAttribute(hConsoleOutput, 240);
	COORD currentCoord = GetConsoleCursorPosition();
	_setmode(_fileno(stdout), _O_TEXT);
	for (int i = 0; i < numCell; i++) { // deploy horizontal label
		GotoXY((currentCoord.X + 2) + i * 4, currentCoord.Y - 1);
		cout << to_string(i + 1);
	}

	for (int i = 0; i < numCell; i++) { // deploy vertical label
		GotoXY(currentCoord.X - 2, (currentCoord.Y + 1) + i * 2);
		cout << to_string(i + 1);
	}

	_setmode(_fileno(stdout), _O_U16TEXT); // Chuyển sang UTF-16 để có khả năng in ra kí hiệu unicode
	GotoXY(currentCoord.X, currentCoord.Y); wcout << L"╔";
	GotoXY(currentCoord.X + 4 * numCell, currentCoord.Y); wcout << L"╗";
	GotoXY(currentCoord.X, currentCoord.Y + 2 * numCell); wcout << L"╚";
	GotoXY(currentCoord.X + 4 * numCell, currentCoord.Y + 2 * numCell); wcout << L"╝";
	for (int i = 0; i <= numCell - 1; i++) {
		for (int j = 0; j <= numCell; j++) {
			GotoXY(currentCoord.X + 1 + 4 * i, currentCoord.Y + 2 * j); wcout << L"═══";
		}
	}
	for (int i = 0; i <= numCell; i++) {
		for (int j = 0; j <= numCell - 1; j++) {
			GotoXY(currentCoord.X + 4 * i, currentCoord.Y + 1 + 2 * j); wcout << L"║";
		}
	}
	for (int i = 0; i < numCell - 1; i++) {
		for (int j = 1; j < numCell; j++) {
			GotoXY(currentCoord.X + 4 + 4 * i, currentCoord.Y + 2 * j); wcout << L"╬";
		}
	}
	for (int i = 1; i < numCell; i++) {
		GotoXY(currentCoord.X + 4 * i, currentCoord.Y + 2 * numCell); wcout << L"╩";
		GotoXY(currentCoord.X + 4 * i, currentCoord.Y); wcout << L"╦";
		GotoXY(currentCoord.X, currentCoord.Y + 2 * i); wcout << L"╠";
		GotoXY(currentCoord.X + 4 * numCell, currentCoord.Y + 2 * i); wcout << L"╣";
	}
	_setmode(_fileno(stdout), _O_TEXT);
}

Visualizer visualizer;

void DrawObject(string objName) {
	_setmode(_fileno(stdout), _O_U16TEXT);//set to UTF16 text cout
	switch (string_hash(objName))
	{
	case main_Logo:
		visualizer.printLogo("Main_Logo");
		break;
	case about_Logo:
		visualizer.printLogo("About_Logo");
		break;
	case help_Logo:
		visualizer.printLogo("Help_Logo");
		break;
	case button:
		visualizer.printButton();
		break;
	case background:
		visualizer.printBackground(170, 40);
		break;
	case boardCanvas:
		visualizer.printBoardCanvas(16);
		break;
	case border:
		visualizer.printBorder();
		break;
	case playerFrame:
		GotoXY(5, 3);
		visualizer.printPlayerFrame('X');
		GotoXY(120, 3);
		visualizer.printPlayerFrame('O');
		break;
	
	default:
		break;
	}
	_setmode(_fileno(stdout), _O_TEXT);//return to normal cout

  /*wcout << L"  █████╗ █████╗ ██████╗  ██████╗\n";
	wcout << L"██╔════╝██╔══██╗██╔══██╗██╔═══██╗\n";
	wcout << L"██║     ███████║██████╔╝██║   ██║\n";
	wcout << L"██║     ██╔══██║██╔══██╗██║   ██║\n";
	wcout << L"╚██████╗██║  ██║██║  ██║╚██████╔╝\n";
	wcout << L" ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝\n";*/
}

void SceneHandle(string sceneName) {
	if (sceneName == "MAIN MENU") {
		StartMenu();
		return;
	}
	if (sceneName == "PLAY") {
		StartPlay();
		return;
	}
	if (sceneName == "LOAD") {
		StartLoad();
		return;
	}
	if (sceneName == "OPTIONS") {
		StartMenu();
		return;
	}
	if (sceneName == "HELP") {
		StartHelp();
		return;
	}
	if (sceneName == "ABOUT") {
		StartAbout();
		return;
	}
	if (sceneName == "EXIT") {
		StartExit();
		return;
	}
	if (sceneName == "PLAYER VERSUS COMPUTER") {
		StartMatchScene("PVE");
		return;
	}
	if (sceneName == "PLAYER VERSUS PLAYER") {
		StartMatchScene("PVP");
		return;
	}
	if (sceneName == "WinScene_X") {
		StartWinScene('X');
		return;
	}
	if (sceneName == "WinScene_O") {
		StartWinScene('O');
		return;
	}
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

