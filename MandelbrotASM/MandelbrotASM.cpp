#include <iostream>
#include <Windows.h>

const int MAX_ITERATIONS = 1000;
const int WIDTH = 800;
const int HEIGHT = 600;

unsigned int pixels[WIDTH * HEIGHT];

BITMAPINFO bitmapInfo;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            SetDIBitsToDevice(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, 0, HEIGHT, pixels, &bitmapInfo, DIB_RGB_COLORS);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Zadeklaruj funkcję asemblera, aby C++ wiedział, że istnieje
extern "C" {
    long long CalculateMath(long long a, long long b);
}

int main() {
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = WIDTH;
	bitmapInfo.bmiHeader.biHeight = -HEIGHT;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        pixels[i] = 0x00000133;
    }
	const wchar_t CLASS_NAME[] = L"MandelbrotWindowClass";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Mandelbrot Set",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
	);

    if(hwnd == NULL) {
        std::cerr << "Failed to create window." << std::endl;
        return 0;
	}

    ShowWindow(hwnd, SW_SHOW);
	MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}

    //long long result = CalculateMath(10, 25);
    //std::cout << "Wynik z Asemblera: " << result << std::endl;
    return 0;
}