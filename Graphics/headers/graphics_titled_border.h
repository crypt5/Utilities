#ifndef GRAPHICS_TITLED_BORDER_H_
#define GRAPHICS_TITLED_BORDER_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_titled_border(char* title,int x,int y,int height,int width);
void destroy_titled_border(GUI* g,WIDGET* w);

void set_titled_border_text(WIDGET *w,char* text);
void set_titled_border_color(WIDGET* w,int ARGB);
void set_titled_border_text_color(WIDGET* w,int ARGB);
void set_titled_border_thickness(WIDGET* w,int thick);
void set_titled_border_visible(WIDGET* w,int visible);

char* get_titled_border_text(WIDGET* w);
int get_titled_border_color(WIDGET* w);
int get_titled_border_text_color(WIDGET* w);
int get_titled_border_thickness(WIDGET* w);
int get_titled_border_visible(WIDGET* w);

#endif
