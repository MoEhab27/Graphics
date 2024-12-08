/*****************************************************************************
 * File: Ellipse.cpp
 * Author: Mohamed Ehab
 * Last Modified: 28/3/2024
 * Description: This file contains implementation of Bresenham's ellipse drawing algorithm.
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
 * Description: Bresenham's algorithm to get rid of the floating point number for more optimization and
 *				faster execution by calculating the midpoint and the change and draw the point based on the
 *				midpoint position then adding the change.
 */
void drawEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF color);
/*
 * Description: When this function is give one point on the ellipse it can calculate
 *				3 more points on the ellipse and draw them, So, by using this function we
 *				will need to give it only one quarter of the ellipse and the function will draw the
 *				whole ellipse.
 */
void draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF color); 
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
		drawEllipse(hdc, x, y, 100, 50, RGB(77, 100, 99));
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
 * Description: Bresenham's algorithm to get rid of the floating point number for more optimization and
 *				faster execution by calculating the midpoint and the change and draw the point based on the
 *				midpoint position then adding the change.
 */
void drawEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{
	int a_square = a * a, b_square = b * b;
	/* Quarter 2 */
	int x = 0, y = b;
	draw4points(hdc, xc, yc, x, y, color);
	int d = b_square + (a_square * -(b));
	while ((x * b_square) <= (y * a_square)) 
	{
		if (d < 0) 			/* The point is inside the ellipse */
		{
			d += b_square * ((2 * x) + 3);
		}
		else 				/* The point is on or outside the ellipse */
		{
			d += (b_square * ((2 * x) + 3)) + (a_square * ((-2 * y) + 2)); /* Edit the decision parameter */
			y--;
		}
		x++;
		draw4points(hdc, xc, yc, x, y, color);		/* Draw the newly calculated points */
	}
	/* Quarter 1 */
	x = a;
	y = 0;
	draw4points(hdc, xc, yc, x, y, color);
	d = a_square + (b_square * (-a));
	while ((x * b_square) > (y * a_square)) 
	{
		if (d < 0) 			/* The point is inside the ellipse */
		{
			d += a_square * ((2 * y) + 3);
		}
		else 				/* The point is on or outside the ellipse */
		{
			d += (a_square * ((2 * y) + 3)) + (b_square * ((-2 * x) + 2)); /* Edit the decision parameter */
			x--;
		}
		y++;
		draw4points(hdc, xc, yc, x, y, color);		/* Draw the newly calculated points */
	}
}

/*
 * Description: When this function is give one point on the ellipse it can calculate
 *				3 more points on the ellipse and draw them, So, by using this function we
 *				will need to give it only one quarter of the ellipse and the function will draw the
 *				whole ellipse.
 */
void draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc + x, yc - y, color);
}

//DONE
