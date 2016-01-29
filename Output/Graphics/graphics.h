#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <X11/Xlib.h>
#include "sorted_list.h"
#include "link.h"
#include "queue.h"

#define BACKSPACE 1
#define TAB 2
#define ENTER 3
#define LEFT 4
#define RIGHT 5
#define UP 6
#define DOWN 7
#define DELETE 8

typedef struct graphics_t GUI;
struct graphics_t{
  Display* dsp;
  Window mainWindow;
  int blackColor;
  int whiteColor;
  int bgColor;
  pthread_mutex_t lock;
  pthread_t tid;
  int run;
  Atom wm_protocols;
  Atom wm_delete_window;
  SORTED_LIST* widgets;
  QUEUE* updates;
  GC text;
  GC draw;
  XFontStruct* font;
  LIST* windows;
};

#include "graphics_widget.h"
#include "graphics_window.h"

/*
  These functions need to be called in this order 
  for the main window
*/
GUI* init_gui(char* address);
void set_main_background(GUI* g,int RGB); // Optional 
void set_main_icon(GUI* g,char* filename);//Optional
void create_main_window(GUI* g,char* title);
void set_main_size(GUI* g,int height, int width);

void add_to_main(GUI* g,WIDGET* w);

void show_main(GUI* g);

int gui_running(GUI* g);

void shutdown_gui(GUI* g);
void destroy_gui(GUI* g);


// Helper Functions
int to_gray(int color);
void refresh_main_window(GUI* g);
void update_widget(GUI* g,WIDGET* w);

//Window Fuinctions
void register_window(GUI* g, WINDOW* w);
void unregister_window(GUI* g,WINDOW* w);
#endif
