#ifndef GRAPHICS_PICTURE_H_
#define GRAPHICS_PICTURE_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_picture(GUI* g,char* filename,int x,int y);
void destroy_picture(GUI* g,WIDGET* w);

void set_picture_visible(WIDGET* w,int visible);

int get_picture_visible(WIDGET* w);

void set_picture_click_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data);
void set_picture_paint_click(WIDGET* w,void(*uclick)(GUI* g,Window win, WIDGET* w));
void set_picture_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* w),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key));

void remove_picture_click_callback(WIDGET* w);
void remove_picture_paint_click(WIDGET* w);
void remove_picture_paint_select(WIDGET* w);

#endif
