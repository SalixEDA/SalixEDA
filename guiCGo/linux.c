#ifdef __linux__

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

#include "graphics.h"

void initFont();

uint32_t *screenBuffer;
int gWindowWidth = 640;
int gWindowHeight = 480;

Display* display;
Window window;
GC gc;
XImage* image;

/*    int mouse_x;
    int mouse_y;
    int mouse_button;
    char key_pressed[256];
*/

void onWindowSize( int windowWidth, int windowHeight )
  {
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


void winStart() {
  // Открываем соединение с X сервером
  display = XOpenDisplay(NULL);
  if( display == NULL ) {
    fprintf(stderr, "Не удалось открыть X display\n");
    return;
    }

  // Получаем корневое окно
  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);

  // Создаем окно
  window = XCreateSimpleWindow(
    display,           // соединение с X сервером
    root,              // родительское окно
    100, 100,          // позиция x, y
    gWindowWidth,
    gWindowHeight,     // ширина, высота
    1,                 // ширина границы
    BlackPixel(display, screen),  // цвет границы
    WhitePixel(display, screen)   // цвет фона
    );

  // Устанавливаем заголовок окна
  XStoreName(display, window, "SalixEDA setup program");

  // Подписываемся на события
  XSelectInput(display, window,
                 ExposureMask |           // События перерисовки
                 KeyPressMask |           // Нажатия клавиш
                 KeyReleaseMask |         // Отжатия клавиш
                 ButtonPressMask |        // Нажатия кнопок мыши
                 ButtonReleaseMask |      // Отжатия кнопок мыши
                 PointerMotionMask |      // Движение мыши
                 StructureNotifyMask);    // Изменение размера окна

  // Создаем графический контекст
  gc = XCreateGC( display, window, 0, NULL );

  // Показываем окно
  XMapWindow(display, window);

  // Обрабатываем события
  XEvent event;
  while (1) {
    XNextEvent(display, &event);

    switch (event.type) {
      case Expose:
        // Событие перерисовки окна
        if( event.xexpose.count == 0 ) {

          // Отправляем изображение в окно
          XPutImage( display, window, gc, image, 0, 0, 0, 0, gWindowWidth, gWindowHeight );
          }
        break;

      case KeyPress:
        // Выход по нажатию любой клавиши
        XCloseDisplay(display);
        return;
      }
    }
  }

void winRepaint()
  {
  }

#endif
