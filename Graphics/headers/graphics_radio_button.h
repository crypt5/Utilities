#ifndef GRAPHICS_RADIO_BUTTON_H_
#define GRAPHICS_RADIO_BUTTON_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_radio_button(char* text,int x,int y);
void destroy_radio_button(GUI* g,WIDGET* w);

void set_radio_button_text(WIDGET* w,char* text);
void set_radio_button_text_color(WIDGET* w,int color);
void set_radio_button_check_color(WIDGET* w,int color);
void set_radio_button_check(WIDGET* w,int check);
void set_radio_button_enable(WIDGET* w, int enable);
void set_radio_button_visible(WIDGET* w,int visible);

char* get_radio_button_text(WIDGET* w);
int get_radio_button_text_color(WIDGET* w);
int get_radio_button_check_color(WIDGET* w);
int get_radio_button_check(WIDGET* w);
int get_radio_button_enable(WIDGET* w);
int get_radio_button_visible(WIDGET* w);

void set_radio_button_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* l),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key));
void set_radio_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data);

void remove_radio_button_paint_select(WIDGET* w);
void remove_radio_button_callback(WIDGET* w);

#endif
