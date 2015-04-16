#ifndef GRAPHICS_WINDOW_H_
#define GRAPHICS_WINDOW_H_

#include <X11/Xlib.h>
#include "link.h"
#include "queue.h"

typedef struct window_t WINDOW;
struct window_t{
  Window w;
  LIST* widgets;
  QUEUE* updates;
};

#include "graphics.h"

// Pass negitive number to use main background
WINDOW* create_window(GUI* g,char* title, int bgColor);

void set_window_icon(GUI* g, WINDOW* w,char* filename);//TODO

void set_window_size(GUI* g,WINDOW* win, int height, int width);//TODO

void add_widget_to_window(WINDOW* win, WIDGET* w);//TODO

void set_window_visible(GUI* g,WINDOW* win,int visible);//TODO

void destroy_window(GUI* g,WINDOW* win);
#endif
