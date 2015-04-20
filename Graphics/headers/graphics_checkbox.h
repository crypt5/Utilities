#ifndef GRAPHICS_CHECKBOX_H_
#define GRAPHICS_CHECKBOX_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_checkbox(char* text,int x,int y);
void destroy_checkbox(GUI* g,WIDGET* w);

void set_checkbox_text(WIDGET* w,char* text);
void set_checkbox_text_color(WIDGET* w,int color);
void set_checkbox_check_color(WIDGET* w, int color);
void set_checkbox_checked(WIDGET* w,int check);
void set_checkbox_enable(WIDGET* w, int enable);
void set_checkbox_visible(WIDGET* w, int visible);

char* get_checkbox_text(WIDGET* w);
int get_checkbox_text_color(WIDGET* w);
int get_checkbox_check_color(WIDGET* w);
int get_checkbox_checked(WIDGET* w);
int get_checkbox_enable(WIDGET* w);
int get_checkbox_visible(WIDGET* w);

void set_checkbox_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* l),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key));
void set_checkbox_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data);

void remove_checkbox_paint_select(WIDGET* w);
void remove_checkbox_callback(WIDGET* w);

#endif
