#include <stdint.h>
#include "sans.h"
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

extern uint32_t *screenBuffer;
extern int gWindowWidth;
extern int gWindowHeight;

stbtt_fontinfo font;

void initFont()
  {
  stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
  }


static void fillLine( int x1, int x2, int y, uint32_t color )
  {
  if( y < 0 || y >= gWindowHeight ) return;
  if( x1 >= gWindowWidth || x2 < 0 ) return;
  if( x1 < 0 ) x1 = 0;
  if( x2 > gWindowWidth ) x2 = gWindowWidth;
  uint32_t *ptr = screenBuffer + y * gWindowWidth + x1;
  while( x1 < x2 ) {
    *ptr++ = color;
    x1++;
    }
  }

void drawRectangle( int x, int y, int w, int h, uint32_t color )
  {
  for( int i = y; i < h; i++ )
    fillLine( x, x + w, y + i, color );
  }


// Структура цвета RGBA
typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} ColorRGBA;



typedef union {
    ColorRGBA mColor;
    uint32_t  mUInt;
  } ColorUInt;



// Функция установки пикселя с проверкой границ
static void putPixel(int x, int y, uint32_t color) {
  if( x < 0 || x >= gWindowWidth || y < 0 || y >= gWindowHeight) return;
  // Здесь запись в ваш framebuffer
  screenBuffer[y * gWindowWidth + x] = color;
  }





// Смешивание цвета с учетом прозрачности
static void blendPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t weight ) {
  if( x < 0 || x >= gWindowWidth || y < 0 || y >= gWindowHeight) return;

  // Здесь должно быть чтение текущего пикселя и смешивание
  ColorUInt background;
  background.mUInt = screenBuffer[y * gWindowWidth + x];
  // Простейшая линейная интерполяция для каждого канала
  uint8_t inv_weight = 255 - weight;
  background.mColor.r = (uint8_t)((r * weight + background.mColor.r * inv_weight) / 255);
  background.mColor.g = (uint8_t)((g * weight + background.mColor.g * inv_weight) / 255);
  background.mColor.b = (uint8_t)((b * weight + background.mColor.b * inv_weight) / 255);
  background.mColor.a = 255; // Предполагаем, что фон непрозрачный
  screenBuffer[y * gWindowWidth + x] = background.mUInt;
  }


void drawHLine2( int x1, int x2, int yt, int yb, uint32_t color )
  {
  for( int x = x1; x < x2; x++ ) {
    putPixel( x, yt, color );
    putPixel( x, yb, color );
    }
  }

void drawVLine2( int xl, int xr, int y1, int y2, uint32_t color )
  {
  for( int y = y1; y < y2; y++ ) {
    putPixel( xl, y, color );
    putPixel( xr, y, color );
    }
  }


void drawRect( int x, int y, int w, int h, uint32_t color )
  {
  drawHLine2( x, x + w, y, y + h - 1, color );
  drawVLine2( x, x + w - 1, y, y + h, color );
  }


int textWidth( int size, const int *str, int len )
  {
  float scale = stbtt_ScaleForPixelHeight(&font, size);
  //Calculate string box width
  int w = 0;
  for( int i = 0; i < len; i++ ) {
    int advance,lsb;
    stbtt_GetCodepointHMetrics(&font, str[i], &advance, &lsb);
    if( i+1 < len )
      w += scale * (advance + stbtt_GetCodepointKernAdvance( &font, str[i], str[i+1] ) );
    else
      w += scale * advance;
    }
  return w;
  }



void drawText( int align, int px, int py, int size, const int *str, int len, uint32_t color )
  {
  unsigned char *bitmap;
  ColorUInt exColor;
  exColor.mUInt = color;
  float scale = stbtt_ScaleForPixelHeight(&font, size);
  if( align != 0 ) {
    int horz = align & 3;
    if( horz ) {
      //Calculate string box width
      int w = 0;
      for( int i = 0; i < len; i++ ) {
        int advance,lsb;
        stbtt_GetCodepointHMetrics(&font, str[i], &advance, &lsb);
        if( i+1 < len )
          w += scale * (advance + stbtt_GetCodepointKernAdvance( &font, str[i], str[i+1] ) );
        else
          w += scale * advance;
        }
      if( horz == 1 )
        //Horizontal center
        px -= w / 2;
      else
        //Horizontal right
        px -= w;
      }
    int vert = (align >> 2) & 3;
    if( vert == 1 )
      //Vertical center
      py += size / 4;
    else if( vert == 2 )
      //Vertical top
      py += size / 2;
    }
  for( int i = 0; i < len; i++ ) {
    int w,h,c = str[i];
    int advance,lsb,xoff,yoff;
    stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);
    bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, c, &w, &h, &xoff, &yoff);
    for( int y = 0; y < h; ++y ) {
      for( int x = 0; x < w; ++x )
        blendPixel( px + x + xoff, py + y + yoff, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, bitmap[y*w+x] );
      }
    free( bitmap );
    if( i+1 < len )
      px += scale*( advance + stbtt_GetCodepointKernAdvance( &font, c, str[i+1] ) );
    }
  }



// Рисование окружности с антиалиасингом (алгоритм Ву)
void drawRoundRectangle( int px, int py, int w, int h, int r, uint32_t color )
  {
  if( 2*r > w || 2*r > h ) return;
  int xc = px + r;
  int yc = py + r;
  int x = 0;
  int y = r;
  w -= r * 2;
  h -= r * 2;
  float d = 1.0f - r;  // начальное значение для определения расстояния до окружности

  ColorUInt exColor;
  exColor.mUInt = color;

  while (x <= y) {
    // Рисуем 8 октантов с антиалиасингом

    // Основные точки (с полной яркостью)
    fillLine( xc - x, xc + x + w, yc - y, color );
    fillLine( xc - x, xc + x + w, yc + y + h, color );
    fillLine( xc - y, xc + y + w, yc - x, color );
    fillLine( xc - y, xc + y + w, yc + x + h, color );

    // Вычисляем альфу для сглаживания следующего пикселя
    float alpha = d - 0.5f;
    if( alpha < 0 ) alpha = -alpha;
    if( alpha < 1.0f ) {
      // Сглаживаем переход
      uint8_t weight = 255 * alpha;
      if (x < y) {
        blendPixel(xc + x + 1 + w, yc + y + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight );
        blendPixel(xc - x - 1, yc + y + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc + x + 1 + w, yc - y, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - x - 1, yc - y, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        }
      if (x != y) {
        blendPixel(xc + y + 1 + w, yc + x + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - y - 1, yc + x + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc + y + 1 + w, yc - x, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - y - 1, yc - x, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        }
      }

    x++;

    if (d < 0) {
      d += 2.0f * x + 1.0f;
      }
    else {
      y--;
      d += 2.0f * (x - y) + 1.0f;
      }
    }

  //Fill central part
  for( int i = 0; i < h; i++ )
    fillLine( xc - r, xc + r + w, yc + i, color );
  }

void drawRoundRect( int px, int py, int w, int h, int r, uint32_t color )
  {
  if( 2*r > w || 2*r > h ) return;
  int xc = px + r;
  int yc = py + r;
  int x = 0;
  int y = r;
  w -= r * 2;
  h -= r * 2;
  float d = 1.0f - r;  // начальное значение для определения расстояния до окружности

  ColorUInt exColor;
  exColor.mUInt = color;

  while (x <= y) {
    // Рисуем 8 октантов с антиалиасингом

    // Основные точки (с полной яркостью)
    putPixel( xc - x, yc - y, color );
    putPixel( xc + x + w, yc - y, color );

    putPixel( xc - x, yc + y + h, color );
    putPixel( xc + x + w, yc + y + h, color );

    putPixel( xc - y, yc - x, color );
    putPixel( xc + y + w, yc - x, color );

    putPixel( xc - y, yc + x + h, color );
    putPixel( xc + y + w, yc + x + h, color );

    // Вычисляем альфу для сглаживания следующего пикселя
    float alpha = d - 0.5f;
    if( alpha < 0 ) alpha = -alpha;
    if( alpha < 1.0f ) {
      // Сглаживаем переход
      uint8_t weight = 255 * alpha;
      if (x < y) {
        blendPixel(xc + x + 1 + w, yc + y + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight );
        blendPixel(xc - x - 1, yc + y + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc + x + 1 + w, yc - y, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - x - 1, yc - y, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        }
      if (x != y) {
        blendPixel(xc + y + 1 + w, yc + x + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - y - 1, yc + x + h, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc + y + 1 + w, yc - x, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        blendPixel(xc - y - 1, yc - x, exColor.mColor.r, exColor.mColor.g, exColor.mColor.b, weight);
        }
      }

    x++;

    if (d < 0) {
      d += 2.0f * x + 1.0f;
      }
    else {
      y--;
      d += 2.0f * (x - y) + 1.0f;
      }
    }

  //Fill central part
  drawHLine2( xc, xc + w, py, py + h + 2 * r, color );
  drawVLine2( px, px + w + 2 * r, yc, yc + h, color );
  }
