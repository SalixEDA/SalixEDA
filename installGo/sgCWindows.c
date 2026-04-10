#ifdef _WIN64

#include <windows.h>
#include <stdint.h>
#include <memory.h>

#include "sgCGraphics.h"

void initFont();

uint32_t *screenBuffer;
int gWindowWidth;
int gWindowHeight;



void onWindowSize(HWND hwnd)
  {
  RECT rect;
  GetClientRect(hwnd, &rect);
  int windowWidth = rect.right - rect.left;
  int windowHeight = rect.bottom - rect.top;
  if( gWindowWidth != windowWidth || gWindowHeight != windowHeight ) {
    //Update size
    gWindowWidth = windowWidth;
    gWindowHeight = windowHeight;
    //Free previous buffer
    if( screenBuffer != NULL ) free( screenBuffer );
    //Allocate new buffer
    screenBuffer = (uint32_t*)malloc( gWindowWidth * gWindowHeight * 4 );
    memset( screenBuffer, 0xff, gWindowWidth * gWindowHeight * 4 );
    }
  }



int gMouseX;
int gMouseY;
int gMouseLeft;
int gMouseRight;

void onMouse( int x, int y, int leftButton, int rightButton )
  {
  gMouseX = x;
  gMouseY = y;
  gMouseLeft = leftButton;
  gMouseRight = rightButton;
  goMouse( gMouseX, gMouseY, gMouseLeft, gMouseRight );
  }



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch(msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_SIZE:
      // Обновляем размеры окна при изменении
      onWindowSize(hwnd);
      InvalidateRect(hwnd, NULL, TRUE);
      return 0;

    case WM_MOUSEMOVE:
      // Получаем координаты мыши (в клиентских координатах)
      onMouse( LOWORD(lParam), HIWORD(lParam), gMouseLeft, gMouseRight );
      return 0;

    case WM_LBUTTONDOWN:
      onMouse( gMouseX, gMouseY, 1, gMouseRight );
      return 0;

    case WM_LBUTTONUP:
      onMouse( gMouseX, gMouseY, 0, gMouseRight );
      return 0;

    case WM_RBUTTONDOWN:
      onMouse( gMouseX, gMouseY, gMouseLeft, 1 );
      return 0;

    case WM_RBUTTONUP:
      onMouse( gMouseX, gMouseY, gMouseLeft, 0 );
      return 0;

    case WM_MBUTTONDOWN:
      return 0;

    case WM_MBUTTONUP:
      return 0;

    case WM_KEYDOWN:
      {
      UINT vkCode = (UINT)wParam;
      if( vkCode < 256 ) goKeyDown( vkCode );
      }
      return 0;

    case WM_KEYUP:
      {
      UINT vkCode = (UINT)wParam;
      if( vkCode < 256 ) goKeyUp( vkCode );
      }
      return 0;

    case WM_CHAR:
      // Дополнительно можно обрабатывать символы с учетом раскладки
      // wParam содержит код символа в UTF-16
      goKeyChar( wParam );
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      // Обновляем размеры перед отрисовкой
      onWindowSize(hwnd);

      goPaint( gWindowWidth, gWindowHeight );

      //Copy buffer on screen
      // Создаём структуру BITMAPINFO для описания нашего буфера
      BITMAPINFO bmi = {0};
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = gWindowWidth;
      bmi.bmiHeader.biHeight = -gWindowHeight; // Отрицательная высота = верхний-левый угол (0,0)
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 32;           // 32 бита на пиксель (BGRA)
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biSizeImage = gWindowWidth * gWindowHeight * 4;

      // Копируем пиксели в окно одной операцией
      SetDIBitsToDevice(
          hdc,
          0, 0,                               // Целевые координаты X, Y
          gWindowWidth, gWindowHeight,         // Ширина и высота
          0, 0,                                // Исходные координаты X, Y в DIB
          0,                                    // Начальная линия сканирования
          gWindowHeight,                        // Количество линий
          screenBuffer,                         // Указатель на пиксели
          &bmi,                                  // Информация о формате
          DIB_RGB_COLORS                         // Используем RGB значения
      );

      EndPaint(hwnd, &ps);
      return 0;
      }
    }
  return DefWindowProc(hwnd, msg, wParam, lParam);
  }

HWND hMainWindow;

void winStart( int posx, int posy, int width, int height, const char *title ) {
  initFont();
  HINSTANCE hInstance = GetModuleHandle(NULL); // Получаем hInstance
  const char CLASS_NAME[] = "MainWindowClass";

  // Регистрация класса окна
  WNDCLASS wc = {0};
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClass(&wc);

  // Создание окна
  hMainWindow = CreateWindowEx(
                0,
                CLASS_NAME,
                title,
                WS_OVERLAPPEDWINDOW,
                posx, posy,
                width, height,
                NULL,
                NULL,
                hInstance,
                NULL
                );

  if( !hMainWindow ) return;

  ShowWindow( hMainWindow, SW_SHOWDEFAULT );
  UpdateWindow( hMainWindow );

  // Main event loop
  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    }
  }


void winRepaint()
  {
  InvalidateRect( hMainWindow, NULL, TRUE );
  UpdateWindow( hMainWindow );
  }

void winClose()
  {
  PostMessage( hMainWindow, WM_CLOSE, 0, 0 );
  }

#endif
