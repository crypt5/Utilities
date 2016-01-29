#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <string.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_label.h"

struct label_data_t{
  int text_color;
  int border_color;
  int border_width;
  int background;
  Pixmap map;
};

void paint_label(GUI* g,Window win, WIDGET* w)
{
  struct label_data_t* data=w->widget_data;
 
  if((w->status&STATUS_REPAINT)>0){

    if(w->width==0){
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+6;
      w->height=g->font->ascent*2;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,3*w->height,24);
    }
    else{
      XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
      XFreePixmap(g->dsp,data->map);
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+6;
      w->height=g->font->ascent*2;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*3,24);
    }

    //not visible and background of whole thing
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,0,w->width,w->height*3);
    
    //Normal
    if(data->background>=0)
      XSetForeground(g->dsp,g->draw,data->background);
    else
      XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,w->height,w->width,w->height);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,data->text_color);
      XDrawString(g->dsp,data->map,g->text,3,w->height+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XDrawString(g->dsp,data->map,g->text,3,w->height+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
    }
    if(data->border_color>=0){
      XSetForeground(g->dsp,g->draw,data->border_color);
      XSetLineAttributes(g->dsp,g->draw,data->border_width,LineSolid,CapButt,JoinMiter);
      XDrawRectangle(g->dsp,data->map,g->draw,0,w->height,w->width-1,w->height-1);
      XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
  }
    //Not enabled
    if(data->background>=0)
      XSetForeground(g->dsp,g->draw,to_gray(data->background));
    else
      XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width,w->height);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text, to_gray(data->text_color));
      XDrawString(g->dsp,data->map,g->text,3,w->height*2+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text, to_gray(g->blackColor));
      XDrawString(g->dsp,data->map,g->text,3,w->height*2+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    if(data->border_color>=0){
      XSetForeground(g->dsp,g->draw, to_gray(data->border_color));
      XSetLineAttributes(g->dsp,g->draw,data->border_width,LineSolid,CapButt,JoinMiter);
      XDrawRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width-1,w->height-1);
      XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
    }
    w->status=w->status&(~STATUS_REPAINT);

    // Debug print to file
    #ifdef DEBUG_PRINT_IMAGES
    char filename[1024];
    sprintf(filename,"pic_output/label_%p.xpm",(void *)w);
    Pixmap p=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*3,24);
    XSetForeground(g->dsp,g->draw,0xFFFFFFFF);
    XFillRectangle(g->dsp,p,g->draw,0,0,w->width,w->height*3);
    XpmWriteFileFromPixmap(g->dsp,filename,data->map,p,NULL);
    XFreePixmap(g->dsp,p);
    #endif
  }

  // Copy the correct area to screen 
  if((w->status&STATUS_VISIBLE)==0)
    XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
  else if((w->status&STATUS_ENABLE)==0)
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height*2,w->width,w->height,w->x,w->y);
  else
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height,w->width,w->height,w->x,w->y);

}

WIDGET* create_label(char* message,int x, int y)
{
  WIDGET* w=NULL;
  struct label_data_t* data=NULL;
  char* s=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget Malloc Failed\n");
    exit(-1);
  }
  data=malloc(sizeof(struct label_data_t));
  if(data==NULL){
    printf("Label data malloc failed\n");
    exit(-1);
  }
  data->text_color=-1;
  data->border_color=-1;
  data->border_width=-1;
  data->background=-1;

  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String malloc failed\n");
    exit(-1);
  }
  strcpy(s,message);

  w->type=LABEL;
  w->flags=NONE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE|STATUS_REPAINT;
  w->x=x;
  w->y=y;
  w->width=0;
  w->height=0;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_label;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_label;
  w->string=s;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_label(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Label is NULL!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
  free(w);
}

void set_label_background(WIDGET* l,int ARGB)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  data->background=ARGB;
  l->status=l->status|STATUS_REPAINT;
}

void set_label_text_color(WIDGET* l,int ARGB)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  data->text_color=ARGB;
  l->status=l->status|STATUS_REPAINT;
}

void set_label_text(WIDGET* l,char* message)
{
  char* s=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  free(l->string);

  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String malloc failed\n");
    exit(-2);
  }
  strcpy(s,message);
  l->string=s;
  l->status=l->status|STATUS_REPAINT;
}

void set_label_border(WIDGET* l, int ARGB, int thickness)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  data->border_color=ARGB;
  data->border_width=thickness;
  l->status=l->status|STATUS_REPAINT;
}

void set_label_enable(WIDGET* l, int enable)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if(enable==1)
    l->status=l->status|STATUS_ENABLE;
  else if(enable==0)
    l->status=l->status&~STATUS_ENABLE;
  else
    printf("Invalid Enable Flag\nNo Action Taken\n");
  
}

void set_label_visible(WIDGET* l, int visible)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if(visible==1){
    l->status=l->status|STATUS_VISIBLE;
  }
  else if(visible==0){
    l->status=l->status&~STATUS_VISIBLE;
  }
  else{
    printf("Invalid visible Flag\nNo Action Taken\n");
  }
}

int get_label_background(WIDGET* l)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  return data->background;
}

int get_label_text_color(WIDGET* l)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  return data->text_color;
}

char* get_label_text(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  return l->string;
}

void get_label_border(WIDGET* l,int* re_ARGB, int* re_thickness)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  data=l->widget_data;
  *re_ARGB=data->border_color;
  *re_thickness=data->border_width;
}

int get_label_enable(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if((l->status&STATUS_ENABLE)>0)
    return 1;
  else
    return 0;
}

int get_label_visible(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if((l->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}

void set_label_click_callback(WIDGET* l,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->call=ucallback;
  l->data=data;
  l->flags=l->flags|CLICKABLE;
}

void set_label_paint_click(WIDGET* l,void(*uclick)(GUI* g,Window win, WIDGET* l))
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->click=uclick;
  l->flags=l->flags|CLICKABLE;
}

void set_label_paint_select(WIDGET* l,void(*uselect)(GUI* g,Window win, WIDGET* l),void(*ukey_press)(GUI* g,Window win,WIDGET* w,char key))
{  
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->select=uselect;
  l->key_press=ukey_press;
  l->flags=l->flags|SELECTABLE;
}

void remove_label_click_callback(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->call=NULL;
  l->data=NULL;
  l->flags=l->flags&!CLICKABLE;
}

void remove_label_paint_click(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->click=NULL;
  l->flags=l->flags&!CLICKABLE;
}

void remove_label_paint_select(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->select=NULL;
  l->key_press=NULL;
  l->flags=l->flags&!SELECTABLE;
}
