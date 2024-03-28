/*****************************************************************************
 * File: circle.cpp
 * Author: Mohamed Ehab
 * Last Modified: 28/3/2024
 * Description: This file contains implementations of various algorithms to draw circles.
******************************************************************************/


/*****************************************************************************
*								Headers
******************************************************************************/
#include <Windows.h>
#include <math.h>
/*****************************************************************************
*							Function Prototypes
******************************************************************************/
/*
 * Description: The first algorithm to draw circle, using the second octet where the |slope| <= 1
 *				Increases x by 1 then calculate the y value, then draw the 8 points calculated from the
 *				point we've got.
 */
void drawCircle1(HDC hdc, int xc, int yc, int r, COLORREF color);
/*
 * Description: The second algorithm to draw circle (Polar Algorithm) using only r and theta
 *				this algorithm draws only the first octet by increamenting the angle (theta) then
 * 				calculating x and y then draw the 8 points calcculated from the point we've got.
 */
void drawCircle2(HDC hdc, int xc, int yc, int r, COLORREF color);
/*
 * Description: The third algorithm to draw the circle using the same concept in
 *				the polar algorithm but this one is Iterative Polar, by calculating the difference between the points
 *				and add them to the point we have each iteration to get the new point, then draw the 8 points calculated
 *				from the new point we've got.
 */
void drawCircle3(HDC hdc, int xc, int yc, int r, COLORREF color);
/*
 * Description: When this function is give one point on the circle it can calculate
 *				8 more points on the cirlce and draw them, So, by using this function we
 *				will need to give it only one octet of the circle and the function will draw the
 *				whole circle.
 */
void draw8points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);
/*
 * Description: Bresenham's algorithm to get rid of the floating point number for more optimization and
 *				faster execution by calculating the midpoint and the change and draw the point based on the
 *				midpoint position then adding the change.
 */
void drawCircleBresenham1(HDC hdc, int xc, int yc, int r, COLORREF color);
/*
 * Description: The second Bresenham's algorithm and the fastest of all, works by calculating the second order change
 *				(the change of the change) which makes the algorithm have no floating point numbers or multiplications or divisions
 *				inside the loop, only integer addition and subtraction.
*/
void drawCircleBresenham2(HDC hdc, int xc, int yc, int r, COLORREF color);

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	int x, y;
	switch (m)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		x = LOWORD(lp);
		y = HIWORD(lp);
		drawCircleBresenham2(hdc, x, y, 100, RGB(77, 100, 99));

		ReleaseDC(hwnd, hdc);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, m, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hInstance = hi;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"Hello World", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hi, 0);
	ShowWindow(hwnd, ns);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


/*
 * Description: The first algorithm to draw circle, using the second octet where the |slope| <= 1
				Increases x by 1 then calculate the y value, then draw the 8 points calculated from the
				point we've got.
 */
void drawCircle1(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	int x = 0;
	int y = r;
	draw8points(hdc, xc, yc, x, y, color);
	while (x < y) 
	{
		x++;
		y = round(sqrt((r * r) - (x * x)));
		draw8points(hdc, xc, yc, x, y, color);
	}
}

/*
 * Description: The second algorithm to draw circle (Polar Algorithm) using only r and theta
				this algorithm draws only the first octet by increamenting the angle (theta) then
				calculating x and y then draw the 8 points calcculated from the point we've got.
 */
void drawCircle2(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	int x = r, y = 0;
	double theta = 0, d_theta = 1.0 / r;
	draw8points(hdc, xc, yc, x, y, color);
	while (x > y)
	{
		theta += d_theta;
		x = round(r * cos(theta));
		y = round(r * sin(theta));
		draw8points(hdc, xc, yc, x, y, color);
	}
}	

/*
 * Description: The third algorithm and the best of the three to draw the circle using the same concept in
 *				the polar algorithm but this one is Iterative Polar, by calculating the difference between the points
 *				and add them to the point we have each iteration to get the new point, then draw the 8 points calculated
 *				from the new point we've got.
 */
void drawCircle3(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	double x = r, y = 0, d_theta = 1.0 / r;
	double c = cos(d_theta), s = sin(d_theta);
	while (x > y)
	{
		double temp = (x * c) - (y * s);
		y = (x * s) + (y * c);
		x = temp;
		draw8points(hdc, xc, yc, round(x), round(y), color);
	}
}

/*
 * Description: Bresenham's algorithm to get rid of the floating point number for more optimization and 
 *				faster execution by calculating the midpoint and the change and draw the point based on the
 *				midpoint position then adding the change. 
*/
void drawCircleBresenham1(HDC hdc, int xc, int yc, int r, COLORREF color) 
{
	int x = 0, y = r;
	draw8points(hdc, xc, yc, x, y, color);
	int d = 1 - r;
	while (x < y) 
	{
		if (d >= 0) 
		{
			d += (2 * (x - y)) + 5;
			y--;
		}
		else 
		{
			d += (2 * x) + 3;
		}
		x++;
		draw8points(hdc, xc, yc, x, y, color);
	}
}

/*
 * Description: The second Bresenham's algorithm and the fastest of all, works by calculating the second order change
 *				(the change of the change) which makes the algorithm have no floating point numbers or multiplications or divisions
 *				inside the loop, only integer addition and subtraction.
*/
void drawCircleBresenham2(HDC hdc, int xc, int yc, int r, COLORREF color) 
{
	int x = 0, y = r;
	draw8points(hdc, xc, yc, x, y, color);
	int d = 1 - r;
	int changeOne = 3, changeTwo = 5 - (2 * r);
	while (x < y)
	{
		if (d < 0)
		{
			d += changeOne;
			changeTwo += 2;
		}
		else 
		{
			d += changeTwo;
			changeTwo += 4;
			y--;
		}
		changeOne += 2;
		x++;
		draw8points(hdc, xc, yc, x, y, color);
	}
}


/* 
 * Description: When this function is give one point on the circle it can calculate 
 *				8 more points on the cirlce and draw them, So, by using this function we
 *				will need to give it only one octet of the circle and the function will draw the
 *				whole circle.
 */
void draw8points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc + y, yc + x, color);
	SetPixel(hdc, xc - y, yc + x, color);
	SetPixel(hdc, xc - y, yc - x, color);
	SetPixel(hdc, xc + y, yc - x, color);
}

