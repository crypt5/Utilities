#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_titled_border.h"

struct titled_border_data_t{
  int thickness;
  int border_color;
  int text_color;
};

void paint_titled_border(GUI* g,Window win, WIDGET* w)
{
  int width;
  struct titled_border_data_t* data=w->widget_data;
  width=XTextWidth(g->font,w->string,strlen(w->string))+2;
  XSetForeground(g->dsp,g->draw,g->bgColor);
  XFillRectangle(g->dsp,win,g->draw,w->x,w->y-g->font->ascent/2,w->width,g->font->ascent);
  if((w->status&STATUS_VISIBLE)==0){
    XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
    XDrawRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
    XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
    return;
  }
  if(data->border_color>0)
    XSetForeground(g->dsp,g->draw,data->border_color);
  else
    XSetForeground(g->dsp,g->draw,g->blackColor);
  XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
  XDrawRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
  XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
  XSetForeground(g->dsp,g->draw,g->bgColor);
  XFillRectangle(g->dsp,win,g->draw,w->x+5,w->y-data->thickness,width,data->thickness*2);
  if(data->text_color>0){
    XSetForeground(g->dsp,g->text,data->text_color);
    XDrawString(g->dsp,win,g->text,w->x+6,w->y+(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
    XSetForeground(g->dsp,g->text,g->blackColor);
  }
  else {
    XDrawString(g->dsp,win,g->text,w->x+6,w->y+(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
  }
}

WIDGET* create_titled_border(char* text,int x,int y,int height,int width)
{
  WIDGET* w=NULL;
  char* s=NULL;
  struct titled_border_data_t* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-1);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("String malloc failed!\n");
    exit(-1);
  }
  strcpy(s,text);
  d=malloc(sizeof(struct titled_border_data_t));
  if(d==NULL){
    printf("Data malloc failed\n");
    exit(-1);
  }
  d->thickness=1;
  d->border_color=-1;
  d->text_color=-1;
  
  w->type=TITLE_BORDER;
  w->flags=NONE;
  w->status=STATUS_ENABLE|STATUS_VISIBLE;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_titled_border;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_titled_border;
  w->string=s;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_titled_border(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
  free(w);
}

void set_titled_border_text(WIDGET *w,char* text)
{
  char *s=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("String Malloc failed\n");
    exit(-2);
  }
  strcpy(s,text);
  free(w->string);
  w->string=s;
}

void set_titled_border_color(WIDGET* w,int ARGB)
{
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->border_color=ARGB;
}

void set_titled_border_text_color(WIDGET* w,int ARGB)
{
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->text_color=ARGB;
}

void set_titled_border_thickness(WIDGET* w,int thick)
{
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->thickness=thick;
}

void set_titled_border_visible(WIDGET* w,int visible)
{
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid visible Value\n");
}

char* get_titled_border_text(WIDGET* w)
{
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  return w->string;
}

int get_titled_border_color(WIDGET* w)
{
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->border_color;
}

int get_titled_border_text_color(WIDGET* w)
{
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->text_color;
}

int get_titled_border_thickness(WIDGET* w)
{  
  struct titled_border_data_t* data=NULL;
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->thickness;
}

int get_titled_border_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}
