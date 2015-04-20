#ifndef GRAPHICS_LABEL_H_
#define GRAPHICS_LABEL_H_

#include "graphics_widget.h"

WIDGET* create_label(char* message,int x, int y);
void destroy_label(GUI* g, WIDGET* l);

void set_label_background(WIDGET* l,int ARGB);
void set_label_text_color(WIDGET* l,int ARGB);
void set_label_text(WIDGET* l,char* message);
void set_label_border(WIDGET* l, int ARGB, int thickness);
void set_label_enable(WIDGET* l, int enable);
void set_label_visible(WIDGET* l,int visible);

int get_label_background(WIDGET* l);
int get_label_text_color(WIDGET* l);
char* get_label_text(WIDGET* l);
void get_label_border(WIDGET* l,int* re_ARGB, int* re_thickness);
int get_label_enable(WIDGET* l);
int get_label_visible(WIDGET* l);


void set_label_click_callback(WIDGET* l,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data);
void set_label_paint_click(WIDGET* l,void(*uclick)(GUI* g,Window win, WIDGET* l));
void set_label_paint_select(WIDGET* l,void(*uselect)(GUI* g,Window win, WIDGET* l),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key));

void remove_label_click_callback(WIDGET* l);
void remove_label_paint_click(WIDGET* l);
void remove_label_paint_select(WIDGET* l);

#endif
