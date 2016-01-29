#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_button.h"

struct button_data_t{
  int text_color;
  Pixmap map;
};

void paint_button(GUI* g,Window win, WIDGET* w)
{
  struct button_data_t* data=w->widget_data;

  if((w->status&STATUS_REPAINT)>0){
    if(w->width==0){
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
      w->height=g->font->ascent*2;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,4*w->height,24);
    }
    else{
      XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
      XFreePixmap(g->dsp,data->map);
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
      w->height=g->font->ascent*2;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*4,24);
    }
    // Paint not visible
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,0,w->width,w->height*4);


    // Paint not clicked
    XSetForeground(g->dsp,g->draw, 0x00AAAAAA);
    XFillRectangle(g->dsp,data->map,g->draw,0,w->height,w->width,w->height);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawRectangle(g->dsp,data->map,g->draw,0,w->height,w->width-1,w->height-1);
    XDrawLine(g->dsp,data->map,g->draw,1,w->height*2-2,w->width-2,w->height*2-2);
    XDrawLine(g->dsp,data->map,g->draw,w->width-2,w->height+1,w->width-2,w->height*2-2);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,data->text_color);
      XDrawString(g->dsp,data->map,g->text,10,w->height+(w->height/2+w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text,g->blackColor);
      XDrawString(g->dsp,data->map,g->text,10,w->height+(w->height/2+w->height/4),w->string,strlen(w->string));
    }

    // Paint not enabled
    XSetForeground(g->dsp,g->draw,0x00CCCCCC);
    XFillRectangle(g->dsp,data->map,g->draw,0,w->height*3,w->width,w->height);
    XSetForeground(g->dsp,g->draw,to_gray(g->blackColor));
    XDrawRectangle(g->dsp,data->map,g->draw,0,w->height*3,w->width-1,w->height-1);
    XDrawLine(g->dsp,data->map,g->draw,1,w->height*4-2,w->width-2,w->height*4-2);
    XDrawLine(g->dsp,data->map,g->draw,w->width-2,w->height*3+1,w->width-2,w->height*4-2);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,to_gray(data->text_color));
      XDrawString(g->dsp,data->map,g->text,10,w->height*3+(w->height/2+w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text,0x00EEEEEE);
      XDrawString(g->dsp,data->map,g->text,10,w->height*3+(w->height/2+w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }

    // Paint clicked
    XSetForeground(g->dsp,g->draw,0x00808080);
    XFillRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width,w->height);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width-1,w->height-1);
    
    XDrawLine(g->dsp,data->map,g->draw,1,w->height*2+1,w->width-2,w->height*2+1);
    XDrawLine(g->dsp,data->map,g->draw,1,w->height*2+1,1,w->height*3-2);
    

    if(data->text_color<0){
      XDrawString(g->dsp,data->map,g->text,11,w->height*2+(w->height/2+w->height/4)+1,w->string,strlen(w->string));
    }
    else {
      XSetForeground(g->dsp,g->text,data->text_color);
      XDrawString(g->dsp,data->map,g->text,10,w->height*2+(w->height/2+w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }

    // Debug print to file
    #ifdef DEBUG_PRINT_IMAGES
    char filename[1024];
    sprintf(filename,"pic_output/button_%p.xpm",(void *)w);
    Pixmap p=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*4,24);
    XSetForeground(g->dsp,g->draw,0xFFFFFFFF);
    XFillRectangle(g->dsp,p,g->draw,0,0,w->width,w->height*4);
    XpmWriteFileFromPixmap(g->dsp,filename,data->map,p,NULL);
    XFreePixmap(g->dsp,p);
    #endif

    w->status=w->status&(~STATUS_REPAINT);
  }

  // Copy the correct area to screen 
  if((w->status&STATUS_VISIBLE)==0)
    XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
  else if((w->status&STATUS_ENABLE)==0)
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height*3,w->width,w->height,w->x,w->y);
  else
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height,w->width,w->height,w->x,w->y);

}

void paint_click(GUI* g, Window win,WIDGET* w)
{
  struct button_data_t* data=w->widget_data;
  XCopyArea(g->dsp,data->map,win,g->draw,0,w->height*2,w->width,w->height,w->x,w->y);
}

WIDGET* create_button(char* text, int x, int y)
{
  WIDGET* w=NULL;
  char* d=NULL;
  struct button_data_t* data=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Button Malloc failed!\n");
    exit(-1);
  }
  d=malloc(strlen(text)+1);
  if(d==NULL){
    printf("String malloc failed\n");
    exit(-1);
  }
  data=malloc(sizeof(struct button_data_t));
  if(data==NULL){
    printf("Button data malloc failed\n");
    exit(-1);
  }
  data->text_color=-1;

  strcpy(d,text);
  w->type=BUTTON;
  w->flags=CLICKABLE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE|STATUS_REPAINT;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_button;
  w->click=paint_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_button;
  w->string=d;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_button(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  free(w->widget_data);
  free(w->string);
  free(w);
}

void set_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;

}

void set_button_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* w),void(*ukey_press)(GUI* g,Window win,WIDGET* w,char key))
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=uselect;
  w->flags=w->flags|SELECTABLE;
  w->key_press=ukey_press;
}

void remove_button_click_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}

void remove_button_paint_select(WIDGET* w)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=NULL;
  w->key_press=NULL;
  w->flags=w->flags&(~SELECTABLE);
}

void set_button_text_color(WIDGET* w,int ARGB)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  struct button_data_t* data=w->widget_data;
  data->text_color=ARGB;
  w->status=w->status|STATUS_REPAINT;
}

void set_button_text(WIDGET* w, char* message)
{
  char* s=NULL;
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  free(w->string);
  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String re malloc failed!\n");
    exit(-1);
  }
  strcpy(s,message);
  w->string=s;
  w->status=w->status|STATUS_REPAINT;
}

void set_button_enable(WIDGET* w,int enable)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if(enable==1)
    w->status=w->status|STATUS_ENABLE;
  else if(enable==0)
    w->status=w->status&(~STATUS_ENABLE);
  else
    printf("Invalid value for enable\nNo action taken\n");
}

void set_button_visible(WIDGET* w,int visible)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&(~STATUS_VISIBLE);
  else
    printf("Invalid value for visible\nNo action taken\n");
}

int get_button_text_color(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  struct button_data_t* data=w->widget_data;
  return data->text_color;
}

char* get_button_text(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  return w->string;
}

int get_button_enable(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if((w->status&STATUS_ENABLE)>0)
    return 1;
  else
    return 0;
}

int get_button_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}
