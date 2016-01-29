#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_radio_button.h"

struct radiobutton_data_t{
  int checked;
  int text_color;
  int check_color;
  Pixmap map;
};

void paint_radio(GUI* g,Window win, WIDGET* w)
{
  struct radiobutton_data_t* data=w->widget_data;

  if((w->status&STATUS_REPAINT)>0){
    if(w->width==0){
      w->height=g->font->ascent*2;
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+25;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,3*w->height,24);
    }
    else{
      XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
      XFreePixmap(g->dsp,data->map);
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+25;
      w->height=g->font->ascent*2;
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*3,24);
    }
    // Paint not visible
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,0,w->width,w->height*3);

    // Paint Normal
    XSetForeground(g->dsp,g->draw, g->whiteColor);
    XFillArc(g->dsp,data->map,g->draw,0,w->height+(w->height/12),w->height-5,w->height-5,0,360*64);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawArc(g->dsp,data->map,g->draw,0,w->height+(w->height/12),w->height-5,w->height-5,0,360*64);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,data->text_color);
      XDrawString(g->dsp,data->map,g->text,25,w->height+w->height-(w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XDrawString(g->dsp,data->map,g->text,25,w->height+w->height-(w->height/4),w->string,strlen(w->string));
    }

    // Paint not Enabled
    XSetForeground(g->dsp,g->draw, to_gray(g->whiteColor));
    XFillArc(g->dsp,data->map,g->draw,0,w->height*2+(w->height/12),w->height-5,w->height-5,0,360*64);
    XSetForeground(g->dsp,g->draw,to_gray(g->blackColor));
    XDrawArc(g->dsp,data->map,g->draw,0,w->height*2+(w->height/12),w->height-5,w->height-5,0,360*64);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,to_gray(data->text_color));
      XDrawString(g->dsp,data->map,g->text,25,w->height*2+w->height-(w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text,to_gray(g->blackColor));
      XDrawString(g->dsp,data->map,g->text,25,w->height*2+w->height-(w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }

    // Debug print to file
    #ifdef DEBUG_PRINT_IMAGES
    char filename[1024];
    sprintf(filename,"pic_output/radiobutton_%p.xpm",(void *)w);
    Pixmap p=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*3,24);
    XSetForeground(g->dsp,g->draw,0xFFFFFFFF);
    XFillRectangle(g->dsp,p,g->draw,0,0,w->width,w->height*3);
    XpmWriteFileFromPixmap(g->dsp,filename,data->map,p,NULL);
    XFreePixmap(g->dsp,p);
    #endif

   w->status=w->status&(~STATUS_REPAINT);
  }


  // Copy the correct area to screen 
  if((w->status&STATUS_VISIBLE)==0){
    // Not Visible
    XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
  }
  else if((w->status&STATUS_ENABLE)==0){
    // Not enabled
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height*2,w->width,w->height,w->x,w->y);

    if(data->checked==1){
      if(data->check_color>0){
	XSetForeground(g->dsp,g->draw,to_gray(data->check_color));
      }
      else{
	XSetForeground(g->dsp,g->draw, to_gray(0x000000AA));
      }
      XFillArc(g->dsp,win,g->draw,w->x+w->height/4-1,w->y+w->height/4,10,10,0,360*64);
    }
  }
  else{
    // Normal Painting
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height,w->width,w->height,w->x,w->y);
    if(data->checked==1){
      if(data->check_color>0){
	XSetForeground(g->dsp,g->draw, data->check_color);
      }
      else{
	XSetForeground(g->dsp,g->draw,0x000000AA);
      }
      XFillArc(g->dsp,win,g->draw,w->x+w->height/4-1,w->y+w->height/4,10,10,0,360*64);
    }
    
  }
}

void paint_radio_click(GUI* g, Window win,WIDGET* w)
{
  struct radiobutton_data_t* data=w->widget_data;
  if(data->checked==0)
    data->checked=1;
  else
    data->checked=0;
}

WIDGET* create_radio_button(char* text,int x,int y)
{
  WIDGET* w=NULL;
  char* s=NULL;
  struct radiobutton_data_t* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget malloc failed!\n");
    exit(-1);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("String malloc failed!\n");
    exit(-2);
  }
  strcpy(s,text);

  d=malloc(sizeof(struct radiobutton_data_t));
  if(d==NULL){
    printf("Data malloc failed!\n");
    exit(-1);
  }
  d->checked=0;
  d->text_color=-1;
  d->check_color=-1;

  w->type=RADIO_BUTTON;
  w->flags=CLICKABLE;
  w->status=STATUS_ENABLE|STATUS_VISIBLE|STATUS_REPAINT;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_radio;
  w->click=paint_radio_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_radio_button;
  w->string=s;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_radio_button(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
  free(w);
}

void set_radio_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;
}

void remove_radio_button_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}

void set_radio_button_check(WIDGET* w,int check)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  if(check==1||check==0)
    data->checked=check;
  else
    printf("Invalid Value, RadioButton not changed\n");
}

int get_radio_button_check(WIDGET* w)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->checked;
}

void set_radio_button_text(WIDGET* w,char* text)
{
  char* s=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("New String malloc Failed!\n");
    exit(-1);
  }
  free(w->string);
  strcpy(s,text);
  w->string=s;
  w->status|=STATUS_REPAINT;
}

void set_radio_button_text_color(WIDGET* w,int color)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->text_color=color;
  w->status|=STATUS_REPAINT;
}

void set_radio_button_check_color(WIDGET* w,int color)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->check_color=color;
  w->status|=STATUS_REPAINT;
}

void set_radio_button_enable(WIDGET* w, int enable)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  if(enable==1)
    w->status=w->status|STATUS_ENABLE;
  else if(enable==0)
    w->status=w->status&~STATUS_ENABLE;
  else
    printf("Invalid enable value\nRadio Button not changed\n");
}

void set_radio_button_visible(WIDGET* w,int visible)
{  
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid visible value\nRadio Button not changed\n");
}

char* get_radio_button_text(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  return w->string;
}

int get_radio_button_text_color(WIDGET* w)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->text_color;
}

int get_radio_button_check_color(WIDGET* w)
{
  struct radiobutton_data_t* data=NULL;
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->check_color;
}

int get_radio_button_enable(WIDGET* w)
{
 if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  if((w->status&STATUS_ENABLE)>0)
    return 1;
  else 
    return 0;
}

int get_radio_button_visible(WIDGET* w)
{
 if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else 
    return 0;
}

void set_radio_button_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* l),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key))
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->flags=w->flags|SELECTABLE;
  w->select=uselect;
  w->key_press=ukey_press;
}

void remove_radio_button_paint_select(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->flags=w->flags&(!SELECTABLE);
  w->select=NULL;
  w->key_press=NULL;
}
