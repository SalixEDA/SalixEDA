#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

#include "graphics.h"

void initFont();

uint32_t *screenBuffer;
int gWindowWidth;
int gWindowHeight;

int nWindowWidth = 640;
int nWindowHeight = 480;


Display* display;
Window window;
GC gc;
XImage* image;
atomic_int needRedraw;

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

    // Освобождаем старое изображение если есть
    if( image != NULL)
      //Simultaneously screenBuffer will free
      XDestroyImage( image );

    //Allocate new buffer
    screenBuffer = (uint32_t*)malloc( gWindowWidth * gWindowHeight * 4 );
    memset( screenBuffer, 0xff, gWindowWidth * gWindowHeight * 4 );

    memset( screenBuffer, 0x0f, 100 );

    // Создаем XImage из массива пикселей
    image = XCreateImage( display,
                          DefaultVisual( display, DefaultScreen(display)),
                          DefaultDepth( display, DefaultScreen(display)),
                          ZPixmap, 0, (char*)screenBuffer, gWindowWidth, gWindowHeight,
                          32, gWindowWidth * sizeof(int));
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






void winStart() {
  initFont();

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
    640, 480,          // ширина, высота
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

  atomic_init( &needRedraw, 0 );

  // Обрабатываем события
  XEvent event;
  while (1) {

    if( atomic_exchange( &needRedraw, 0 ) ) {
      //Redraw widnow if need
      onWindowSize( nWindowWidth, nWindowHeight );
      goPaint( gWindowWidth, gWindowHeight );
      // Отправляем изображение в окно
      XPutImage( display, window, gc, image, 0, 0, 0, 0, gWindowWidth, gWindowHeight );
      XFlush( display );
      }

    if( XPending(display) <= 0 ) {
      //Событий нет
      usleep( 10000 );
      continue;
      }

    XNextEvent(display, &event);

    //printf( "Event type %d\n", (int)(event.type) );

    switch(event.type) {
      case ConfigureNotify:
        //Resize
        nWindowWidth = event.xconfigure.width;
        nWindowHeight = event.xconfigure.height;
        winRepaint();
        break;

      case Expose:
        // Событие перерисовки окна
        if( event.xexpose.count == 0 )
          winRepaint();
        break;

      case MotionNotify:
        // Движение мыши
        onMouse( event.xmotion.x, event.xmotion.y, gMouseLeft, gMouseRight );
        break;

      case ButtonPress:
        // Нажатие кнопки мыши
        onMouse( gMouseX, gMouseY, event.xbutton.button == Button1 ? 1 : gMouseLeft, event.xbutton.button == Button3 ? 1 : gMouseRight );
        break;

      case ButtonRelease:
        // Отжатие кнопки мыши
        onMouse( gMouseX, gMouseY, event.xbutton.button == Button1 ? 0 : gMouseLeft, event.xbutton.button == Button3 ? 0 : gMouseRight );
        break;

      case KeyPress:
        // Нажатие клавиши
        {
        KeySym key;
        char buffer[32];
        XLookupString(&event.xkey, buffer, sizeof(buffer), &key, NULL);
        int keyCode = 0;
        switch(key) {
          case XK_Home      : keyCode = 0x47; break;
          case XK_Left      : keyCode = 0x4b; break;
          case XK_Right     : keyCode = 0x4d; break;
          case XK_End       : keyCode = 0x4f; break;
          case XK_Delete    : keyCode = 0x53; break;
          case XK_BackSpace : keyCode = 0xe; break;
          case XK_Escape    : keyCode = 0x01; break;
          case XK_Return    : keyCode = 0x1c; break;
          case XK_Tab       : keyCode = 0x0f; break;
          case XK_Up        : keyCode = 0x48; break;
          case XK_Page_Up   : keyCode = 0x49; break;
          case XK_Down      : keyCode = 0x50; break;
          case XK_Page_Down : keyCode = 0x51; break;
          }
        if( keyCode )
          goKeyDown( keyCode );
        }
        break;

      case KeyRelease:
        // Отжатие клавиши
        {
        KeySym key;
        char buffer[32];
        XLookupString(&event.xkey, buffer, sizeof(buffer), &key, NULL);
        }
        break;
      }
    }
  }

// Выход по нажатию любой клавиши
// XCloseDisplay(display);

void winRepaint()
  {
  atomic_store( &needRedraw, 1 );
  }

#endif
