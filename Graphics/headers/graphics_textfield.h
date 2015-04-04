#ifndef GRAPHICS_TEXTFIELD_H_
#define GRAPHICS_TEXTFIELD_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_textfield(int x,int y,int max_length);
void destroy_textfield(GUI* g,WIDGET* w);

void set_textfield_text(WIDGET* w,char* text);
void set_textfield_text_color(WIDGET* w,int ARGB);
void set_textfield_background_color(WIDGET* w,int ARGB);
void set_textfield_enable(WIDGET* w, int enable);
void set_textfield_visible(WIDGET* w,int visible);
void set_textfield_editable(WIDGET* w, int edit);

char* get_textfield_text(WIDGET* w);
int get_textfield_text_color(WIDGET* w);
int get_textfield_background_color(WIDGET* w);
int get_textfield_enable(WIDGET* w);
int get_textfield_visible(WIDGET* w);
int get_textfield_editable(WIDGET* w);

#endif
