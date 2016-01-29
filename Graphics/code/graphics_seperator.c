#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_seperator.h"

struct seperator_data_t{
  int color;
  int thickness;
};

void paint_seperator(GUI* g, Window win,WIDGET* w)
{
  struct seperator_data_t* data=w->widget_data;
  if((w->status&STATUS_VISIBLE)==0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
    XDrawLine(g->dsp,win,g->draw,w->x,w->y,w->x+w->width,w->y);
    XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
  }
  else{
    if(data->color>-1){
      XSetForeground(g->dsp,g->draw,data->color);
      XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
      XDrawLine(g->dsp,win,g->draw,w->x,w->y,w->x+w->width,w->y);
      XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
    }
    else {
      XSetForeground(g->dsp,g->draw,g->blackColor);
      XSetLineAttributes(g->dsp,g->draw,data->thickness,LineSolid,CapButt,JoinMiter);
      XDrawLine(g->dsp,win,g->draw,w->x,w->y,w->x+w->width,w->y);
      XSetLineAttributes(g->dsp,g->draw,1,LineSolid,CapButt,JoinMiter);
    }
  }
}

WIDGET* create_seperator(int x,int y,int width)
{
  WIDGET* w=NULL;
  struct seperator_data_t* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Seperator Malloc failed!\n");
    exit(-1);
  }
  d=malloc(sizeof(struct seperator_data_t));
  if(d==NULL){
    printf("Data Malloc Failed!\n");
    exit(-1);
  }
  d->color=-1;
  d->thickness=1;

  w->type=SEPERATOR;
  w->flags=NONE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=width;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_seperator;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_seperator;
  w->string=NULL;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_seperator(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  free(w->widget_data);
  free(w);
}

void set_seperator_color(WIDGET* w, int ARGB)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  struct seperator_data_t* data=w->widget_data;
  data->color=ARGB;

}

void set_seperator_thickness(WIDGET* w, int thickness)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  struct seperator_data_t* data=w->widget_data;
  data->thickness=thickness;

}

void set_seperator_visible(WIDGET* w,int visible)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&(~STATUS_VISIBLE);
  else
    printf("Invalid seperator flag!\n");
}

int get_seperator_color(WIDGET* w)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  struct seperator_data_t* data=w->widget_data;
  return data->color;
}

int get_seperator_thickness(WIDGET* w)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  struct seperator_data_t* data=w->widget_data;
  return data->thickness;
}

int get_seperator_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Seperator is NULL!\n");
    exit(-2);
  }
  if(w->type!=SEPERATOR){
    printf("Not a Seperator!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  return 0;
}
