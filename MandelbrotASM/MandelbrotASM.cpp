#include <iostream>
#include <Windows.h>

const int MAX_ITERATIONS = 1000;
const int WIDTH = 800;
const int HEIGHT = 600;

unsigned int pixels[WIDTH * HEIGHT];

const int PALETTE_SIZE = 1000;
unsigned int palette[PALETTE_SIZE];

void initializePalette() {
    for (int i = 0; i < PALETTE_SIZE; ++i) {
        double t = (double)i / (double)(PALETTE_SIZE - 1);
            
        int r = (int)(9.0 * (1.0 - t) * t * t * t * 255.0);
        int g = (int)(15.0 * (1.0 - t) * (1.0 - t) * t * t * 255.0);
        int b = (int)(8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t * 255.0);

        palette[i] = (r << 16) | (g << 8) | b;
    }
}
void CalculateMandelbrotCPP(unsigned int* pixelArray, int width, int height) {
    // These define the physical coordinates of the fractal we want to view
    double min_real = -2.0;
    double max_real = 0.47;
    double min_imag = -1.12;
    double max_imag = 1.12;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Map the screen pixel to a complex number (C)
            double c_real = min_real + (x / (double)width) * (max_real - min_real);
            double c_imag = min_imag + (y / (double)height) * (max_imag - min_imag);

            double z_real = 0.0;
            double z_imag = 0.0;
            int iter = 0;

            // The core feedback loop
            while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < MAX_ITERATIONS) {
                double temp_real = z_real * z_real - z_imag * z_imag + c_real;
                z_imag = 2.0 * z_real * z_imag + c_imag;
                z_real = temp_real;
                iter++;
            }

            // Color mapping
            int pixelIndex = (y * width) + x;

            if (iter == MAX_ITERATIONS) {
                // Point is inside the set; color it solid black
                pixelArray[pixelIndex] = 0x00000000;
            }
            else {
                // Point escaped; grab a color from our palette.
                // Using bitwise AND (& 1023) is a blazing fast alternative to modulo (%) 
                // to ensure the index wraps around seamlessly.
                pixelArray[pixelIndex] = palette[iter & 1023];
            }
        }
    }
}

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
	initializePalette();

	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = WIDTH;
	bitmapInfo.bmiHeader.biHeight = -HEIGHT;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

    //for (int i = 0; i < HEIGHT * WIDTH; i++) {
    //    pixels[i] = 0x00000133;
    //}

	CalculateMandelbrotCPP(pixels, WIDTH, HEIGHT);

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