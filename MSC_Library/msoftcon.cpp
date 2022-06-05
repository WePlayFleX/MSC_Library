//Windows console functions access routine


#include "pch.h"
#include "msoftcon.h"
HANDLE hConsole;
char fill_char;

//------------------------------------------------------

void init_graphics()
{

	COORD console_size = { 80, 25 };
	//opens input/output channel
	hConsole = CreateFile(L"CONOUT$", GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
	//Set the size of the console screen
	SetConsoleScreenBufferSize(hConsole, console_size);
	//text white on black
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	fill_char = '\xDB';		//default padding
	clear_screen();
}

//------------------------------------------------------

void set_color(color foreground, color background)
{
	SetConsoleTextAttribute(hConsole,
		(WORD)((background << 4) | foreground));
}

//--------------------------------------------------------

/*
-------------------------------------------------
|		0 black				8 dark gray |
|		1 Dark blue			9 Blue		|
|		2 Dark green		10 Green	|
|		3 Dark blue			11 Scarlet  |
|		4 Dark red			12 Red		|
|		5 Dark scarlet		13 Scarlet  |
|		6 Brown				14 Yellow   |
|		7 Light gray		15 White	|
-------------------------------------------------
*/

//--------------------------------------------------------

void set_cursor_pos(int x, int y)
{
	COORD cursor_pos;		//top - left start
	cursor_pos.X = x - 1;	//windows starts with c (0,0)
	cursor_pos.Y = y - 1;	//we start with (1,1)
	SetConsoleCursorPosition(hConsole, cursor_pos);
}

//--------------------------------------------------------

void clear_screen()
{
	set_cursor_pos(1, 25);
	for (int j = 0; j < 25; j++)
		_putch('\n');
	set_cursor_pos(1, 1);
}

//--------------------------------------------------------

void wait(int milliseconds)
{
	Sleep(milliseconds);
}

//--------------------------------------------------------

void clear_line()		//clear to the end of the line
{
	//80 spaces
	//.....1234567890123456789012345678901234567890
	//.....0........1.........2.........3.........4

	_cputs("                                        ");
	_cputs("                                        ");
}

//--------------------------------------------------------
//square
void draw_rectangle(int left, int top, int right, int bottom)
{
	char temp[80];
	int width = right - left + 1;

	for (int j = 0; j < width; j++)
	{
		temp[j] = fill_char;
		temp[j] = 0;												//TODO
	}
	for (int y = top; y <= bottom; y++)
	{
		set_cursor_pos(left, y);
		_cputs(temp);
	}
}

//--------------------------------------------------------
//circumference
void draw_circle(int xC, int yC, int radius)
{
	double theta, increment, xF, pi = 3.14159;
	int x, xN, yN;

	increment = 0.8 / static_cast<double>(radius);
	for (theta = 0; theta <= pi / 2; theta += increment) //1/4
													     //circumference
	{
		xF = radius * cos(theta);
		xN = static_cast<int>(xF * 2 / 1);		//pixels are not square

		yN = static_cast<int>(radius * sin(theta) + 0.5);
		x = xC - xN;
		while (x <= xC + xN)		//fill in two horizontal lines
		{
			set_cursor_pos(x, yC - yN); _putch(fill_char);  //top
			set_cursor_pos(x++, yC + yN); _putch(fill_char);  //bottom
		}
	}
}

//--------------------------------------------------------
//line
void draw_line(int x1, int y1, int x2, int y2)
{

	int w, z, t, w1, w2, z1, z2;
	double xDelta = x1 - x2, yDelta = y1 - y2, slope;
	bool isMoreHoriz;

	if (fabs(xDelta) > fabs(yDelta))		//more horizontal
	{
		isMoreHoriz = true;
		slope = yDelta / xDelta;
		w1 = x1; z1 = y1; w2 = x2; z2 = y2;			//w=x  z=y
	}
	else
	{
		isMoreHoriz = false;
		slope = xDelta / yDelta;
		w1 = y1; z1 = x1; w2 = y2; z2 = x2;			//w=y  z=x
	}

	if (w1 > w2)									//replace (w1 z1) with (w2 z2)
	{
		t = w1; w1 = w2; w2 = t;
		t = z1; z1 = z2; z2 = t;
	}
	for (w = w1; w <= w2; w++)
	{
		z = static_cast<int>(z1 + slope * (w - w1));
		if (!(w == 80 && z == 25))			//reduce scroll by 80 25
		{
			if (isMoreHoriz)
				set_cursor_pos(w, z);
			else
				set_cursor_pos(z, w);
			_putch(fill_char);
		}
	}
}

//--------------------------------------------------------
//pyramid
void draw_pyramid(int x1, int y1, int height)
{
	int x, y;
	for (y = y1; y < y1 + height; y++)
	{
		int incr = y - y1;
		for (x = x1 - incr; x <= x1 + incr; x++)
		{
			set_cursor_pos(x, y);
			_putch(fill_char);
		}
	}
}

//--------------------------------------------------------

void set_fill_style(fstyle fs)
{
	switch (fs)
	{
	case SOLID_FILL: fill_char = '\xB3'; break;
	case DARK_FILL: fill_char = '\xB0'; break;
	case MEDIUM_FILL: fill_char = '\xB1'; break;
	case LIGHT_FILL: fill_char = '\xB2'; break;
	case X_FILL: fill_char = 'X'; break;
	case O_FILL: fill_char = 'O'; break;
	}
}

//--------------------------------------------------------