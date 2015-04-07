#ifndef GRAPHICS_SEPERATOR_H_
#define GRAPHICS_SEPERATOR_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_seperator(int x,int y,int width);
void destroy_seperator(GUI* g,WIDGET* w);

void set_seperator_color(WIDGET* w, int ARGB);
void set_seperator_thickness(WIDGET* w,int thickness);
void set_seperator_visible(WIDGET* w,int visible);

int get_seperator_color(WIDGET* w);
int get_seperator_thickness(WIDGET* w);
int get_seperator_visible(WIDGET* w);

#endif
