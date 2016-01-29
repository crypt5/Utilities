#ifndef GRAPHICS_WINDOW_H_
#define GRAPHICS_WINDOW_H_

#include <X11/Xlib.h>
#include "sorted_list.h"
#include "queue.h"

#define POPUP_TYPE_INFO 1
#define POPUP_TYPE_ERROR 2
#define POPUP_TYPE_WARNING 3

typedef struct window_t WINDOW;
struct window_t{
  Window w;
  SORTED_LIST* widgets;
  QUEUE* updates;
};

#include "graphics.h"

// Pass negitive number to use main background color
WINDOW* create_window(GUI* g,char* title, int bgColor);
void set_window_icon(GUI* g, WINDOW* w,char* filename);
void set_window_size(GUI* g,WINDOW* win, int height, int width);
void add_widget_to_window(WINDOW* win, WIDGET* w);
void update_window_widget(WINDOW* win, WIDGET* w);
void set_window_visible(GUI* g,WINDOW* win,int visible);
void refresh_window(GUI* g,WINDOW* win);
void destroy_window(GUI* g,WINDOW* win);

/* Common dialog windows commonly used in a program */
/* All calls are blocking to receive input */
int ok_popup(GUI* g,char* message, char* title,int popup_type);
#endif
