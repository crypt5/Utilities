#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_border.h"

struct border_data_t{
  int color;
  int thickness;
};

void paint_border(GUI* g,Window win, WIDGET* w)
{
  struct border_data_t* data=w->widget_data;
  if((w->status&STATUS_VISIBLE)==0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
    XDrawRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
    XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
  }
  else{
    if(data->color>-1){
      XSetForeground(g->dsp,g->draw,data->color);
      XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
      XDrawRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
      XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
    }
    else {
      XSetForeground(g->dsp,g->draw,g->blackColor);
      XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
      XDrawRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
      XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
    }
  }
}

WIDGET* create_simple_border(int x,int y,int height,int width)
{
  WIDGET* w=NULL;
  struct border_data_t* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Border Malloc failed!\n");
    exit(-1);
  }
  d=malloc(sizeof(struct border_data_t));
  if(d==NULL){
    printf("Data Malloc Failed!\n");
    exit(-1);
  }
  d->color=-1;
  d->thickness=1;

  w->type=BORDER;
  w->flags=NONE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_border;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_simple_border;
  w->string=NULL;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_simple_border(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  free(w->widget_data);
  free(w);
}

void set_simple_border_color(WIDGET* w, int ARGB)
{
  struct border_data_t* data=NULL;
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->color=ARGB;
}

void set_simple_border_thickness(WIDGET* w,int thickness)
{
  struct border_data_t* data=NULL;
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->thickness=thickness;
}

void set_simple_border_visible(WIDGET* w,int visible)
{
 if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid Visible input for border\nNo action taken\n");
}


int get_simple_border_color(WIDGET* w)
{
  struct border_data_t* data=NULL;
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  return data->color;
}

int get_simple_border_thickness(WIDGET* w)
{
  struct border_data_t* data=NULL;
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  return data->thickness;
}

int get_simple_border_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}
