#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_textfield.h"

struct textfield_data_t{
  int max_length;
  int current_pos;
  int text_color;
  int background_color;
  int editable;
  Pixmap map;
};
void paint_textfield(GUI* g,Window win,WIDGET* w)
{
  int width;
  int taken=0;
  struct textfield_data_t* data=w->widget_data;

  if((w->status&STATUS_REPAINT)>0){

    if(w->width==0){
      while(w->string[taken]!='\0')
	taken++;
      for(width=taken;width<data->max_length;width++)
	w->string[width]='A';
      w->width=XTextWidth(g->font,w->string,strlen(w->string))+6;
      w->height=g->font->ascent*2;
      for(width=taken;width<data->max_length;width++)
	w->string[width]='\0';
      data->map=XCreatePixmap(g->dsp,g->mainWindow,w->width,3*w->height,24);
    }
   
    //not visible and background of whole thing
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,data->map,g->draw,0,0,w->width,w->height*3);

    // Normal
    if(data->background_color>-1)
      XSetForeground(g->dsp,g->draw,data->background_color);
    else
      XSetForeground(g->dsp,g->draw, g->whiteColor);

    XFillRectangle(g->dsp,data->map,g->draw,0,w->height,w->width,w->height);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawRectangle(g->dsp,data->map,g->draw,0,w->height,w->width-1,w->height-1);

    // Not enabled
     if(data->background_color>-1)
       XSetForeground(g->dsp,g->draw,to_gray(data->background_color));
    else
      XSetForeground(g->dsp,g->draw, to_gray(g->whiteColor));

    XFillRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width,w->height);
    XSetForeground(g->dsp,g->draw,to_gray(g->blackColor));
    XDrawRectangle(g->dsp,data->map,g->draw,0,w->height*2,w->width-1,w->height-1);

    // Debug print to file
    #ifdef DEBUG_PRINT_IMAGES
    char filename[1024];
    sprintf(filename,"pic_output/textfield_%p.xpm",(void *)w);
    Pixmap p=XCreatePixmap(g->dsp,g->mainWindow,w->width,w->height*3,24);
    XSetForeground(g->dsp,g->draw,0xFFFFFFFF);
    XFillRectangle(g->dsp,p,g->draw,0,0,w->width,w->height*3);
    XpmWriteFileFromPixmap(g->dsp,filename,data->map,p,NULL);
    XFreePixmap(g->dsp,p);
    #endif

    w->status=w->status&(~STATUS_REPAINT);
  }

  if((w->status&STATUS_VISIBLE)==0){
    XCopyArea(g->dsp,data->map,win,g->draw,0,0,w->width,w->height,w->x,w->y);
  }
  else if((w->status&STATUS_ENABLE)==0){
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height*2,w->width,w->height,w->x,w->y);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text, to_gray(data->text_color));
      XDrawString(g->dsp,win,g->text,w->x+4,w->y+w->height/2+w->height/4,w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text, to_gray(g->blackColor));
      XDrawString(g->dsp,win,g->text,w->x+4,w->y+w->height/2+w->height/4,w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
  }
  else{
    XCopyArea(g->dsp,data->map,win,g->draw,0,w->height,w->width,w->height,w->x,w->y);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,(w->status&STATUS_ENABLE)>0 ? data->text_color : to_gray(data->text_color));
      XDrawString(g->dsp,win,g->text,w->x+4,w->y+w->height/2+w->height/4,w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XSetForeground(g->dsp,g->text,(w->status&STATUS_ENABLE)>0 ? g->blackColor : to_gray(g->blackColor));
      XDrawString(g->dsp,win,g->text,w->x+4,w->y+w->height/2+w->height/4,w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
  }
}

void paint_textfield_click(GUI* g,Window win, WIDGET* w)
{
  int i;
  struct textfield_data_t* data=w->widget_data;
  if(data->editable==1){
    i=XTextWidth(g->font,"A",1);
    if(data->text_color>0){
      XSetForeground(g->dsp,g->text,data->text_color);
      XDrawString(g->dsp,win,g->text,w->x+(i*data->current_pos),w->y+w->height/2+w->height/4,"|",1);
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else{
      XDrawString(g->dsp,win,g->text,w->x+(i*data->current_pos),w->y+w->height/2+w->height/4,"|",1);
    }
  }
}

void update_textfield(GUI* g,Window win,WIDGET* w, char c)
{
  struct textfield_data_t* data=w->widget_data;
  int i;
  if(data->editable==1&&c>=' '&&strlen(w->string)<data->max_length){
    if(w->string[data->current_pos]=='\0'){
      w->string[data->current_pos]=c;
    }
    else{
      for(i=data->max_length;i>data->current_pos;i--)
	w->string[i]=w->string[i-1];
      w->string[data->current_pos]=c;
    }
    data->current_pos++;
  }
  else if(data->editable==1&&c<' '){
    switch(c){
    case 0:
      //No Operation return from key parsing function
      break;
    case BACKSPACE:
      if(data->current_pos>0){
	if(w->string[data->current_pos]=='\0'){
	  data->current_pos--;
	  w->string[data->current_pos]='\0';
	}
	else{
	  data->current_pos--;
	  for(i=data->current_pos;i<data->max_length;i++)
	    w->string[i]=w->string[i+1];
	}
      }
      break;
    case DELETE:
      if(data->current_pos<data->max_length&&w->string[data->current_pos]!='\0'){
	for(i=data->current_pos;i<data->max_length;i++)
	  w->string[i]=w->string[i+1];
      }
      break;
    case LEFT:
      if(data->current_pos>0)
	data->current_pos--;
      break;
    case RIGHT:
      if(data->current_pos<data->max_length&&w->string[data->current_pos]!='\0')
	data->current_pos++;
      break;
    case ENTER:
    case UP:
    case DOWN:
    case TAB:
      //We dont care in a textbox
      break;
    default:
      printf("Unknown Control Operator: %d\n",c);
    }
  }
  paint_textfield(g,win,w);
  paint_textfield_click(g,win,w);
}

WIDGET* create_textfield(int x,int y,int max_length)
{
  WIDGET* w=NULL;
  struct textfield_data_t* data=NULL;
  char* s=NULL;
  int i;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget malloc failed!\n");
    exit(-1);
  }
  data=malloc(sizeof(struct textfield_data_t));
  if(data==NULL){
    printf("Textfield data malloc failed!\n");
    exit(-1);
  }
  data->current_pos=0;
  data->max_length=max_length;
  data->text_color=-1;
  data->background_color=-1;
  data->editable=1;
  s=malloc(max_length+1);
  if(s==NULL){
    printf("Textbox string malloc failed\n");
    exit(-1);
  }
  for(i=0;i<=max_length;i++)
    s[i]='\0';

  w->type=TEXTFIELD;
  w->flags=CLICKABLE|SELECTABLE;
  w->status=STATUS_ENABLE|STATUS_VISIBLE|STATUS_REPAINT;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_textfield;
  w->click=paint_textfield_click;
  w->select=NULL;
  w->key_press=update_textfield;
  w->ufree=destroy_textfield;
  w->string=s;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_textfield(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
  free(w);
}

void set_textfield_text(WIDGET* w,char* text)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  if(strlen(text)<data->max_length){
    strcpy(w->string,text);
    data->current_pos=0;
  }
  else{
    printf("String too long! can't use!\n");
  }
}

void set_textfield_text_color(WIDGET* w,int ARGB)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  data->text_color=ARGB;
}

void set_textfield_background_color(WIDGET* w,int ARGB)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){ 
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  data->background_color=ARGB;
  w->status|=STATUS_REPAINT;
}

void set_textfield_enable(WIDGET* w, int enable)
{
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  if(enable==1)
    w->status=w->status|STATUS_ENABLE;
  else if(enable==0)
    w->status=w->status&~STATUS_ENABLE;
  else
    printf("Invalid enable flag\n");
}

void set_textfield_visible(WIDGET* w,int visible)
{  
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid visible flag\n");
}

void set_textfield_editable(WIDGET* w, int edit)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  if(edit==0||edit==1)
    data->editable=edit;
  else
    printf("Invalid Editable Flag\n");
}

char* get_textfield_text(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  return w->string;
}

int get_textfield_text_color(WIDGET* w)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->text_color;
}

int get_textfield_background_color(WIDGET* w)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->background_color;
}

int get_textfield_enable(WIDGET* w)
{
 if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  if((w->status&STATUS_ENABLE)>0)
    return 1;
  else
    return 0;
}

int get_textfield_visible(WIDGET* w)
{
 if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else 
    return 0;
}

int get_textfield_editable(WIDGET* w)
{
  struct textfield_data_t* data=NULL;
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTFIELD){
    printf("Widget not a Textfield\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->editable;
}
