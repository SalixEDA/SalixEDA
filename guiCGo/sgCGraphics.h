#include <stdint.h>

//Window system init and start event loop
extern void winStart( int posx, int posy, int width, int height, const char *title );
extern void winRepaint();
extern void winClose();

//Drawing primitives
extern void drawHLine2( int x1, int x2, int yt, int yb, uint32_t color );
extern void drawVLine2( int xl, int xr, int y1, int y2, uint32_t color );
extern void drawRectangle( int x, int y, int w, int h, uint32_t color );
extern void drawRect( int x, int y, int w, int h, uint32_t color );
extern int  textWidth( int size, const int *str, int len );
extern int  textLimit( int width, int size, const int *str, int len );
extern void drawText( int align, int px, int py, int size, const int *str, int len, uint32_t color );
extern void drawRoundRectangle( int px, int py, int w, int h, int r, uint32_t color );
extern void drawRoundRect( int px, int py, int w, int h, int r, uint32_t color );
extern void drawUpRoundRectangle( int px, int py, int w, int h, int r, uint32_t color );
extern void drawUpRoundRect( int px, int py, int w, int h, int r, uint32_t color );
extern void drawImage( int x, int y, int w, int h, const int *image, int imgX, int imgY, int imgWidth, int imgHeight );

//Functions must be declared in go part. They calling from window system event loop as event handler
extern void goPaint( int width, int height );
extern void goMouse( int x, int y, int leftButton, int rightButton );
extern void goKeyDown( int code );
extern void goKeyUp( int code );
extern void goKeyChar( int code );
