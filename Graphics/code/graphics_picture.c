#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_picture.h"

struct picture_data_t{
  Pixmap img;
};

void paint_picture(GUI* g, Window win, WIDGET *w)
{
  struct picture_data_t* data=w->widget_data;
  if((w->status&STATUS_VISIBLE)>0){
    XCopyArea(g->dsp,data->img,win,g->draw,0,0,w->width,w->height,w->x,w->y);
  }
  else{
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,win,g->draw,w->x,w->y,w->width,w->height);
  }
}

WIDGET* create_picture(GUI* g,char* filename,int x,int y)
{
  WIDGET* w=NULL;
  struct picture_data_t* data=NULL;
  unsigned int height,width,bod,dep;
  int i,j;
  Window win;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Initial Widget Malloc Failed!!\n");
    exit(-1);
  }
  data=malloc(sizeof(struct picture_data_t));
  if(data==NULL){
    printf("Data malloc Failed!!!\n");
    exit(-1);
  }

  if(strstr(filename, ".xpm") != NULL){
    if(access(filename,R_OK)!=-1){
      XpmReadFileToPixmap(g->dsp,g->mainWindow,filename,&data->img,NULL,NULL);
    }
    else{
      printf("Read access to picture denied or it doens't exist!\n");
      exit(-2);
    }
  }
  else{
    printf("Invalid Format!\nHas to be X11 *.xpm format");
    printf("GIMP can convert images to this format.\n");
    exit(-2);
  }

  XGetGeometry(g->dsp,data->img,&win,&i,&j,&width,&height,&bod,&dep);

  w->type=PICTURE;
  w->flags=NONE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE;
  w->x=x;
  w->y=y;
  w->height=(int)height;
  w->width=(int)width;
  w->z_order=0;
  w->call=NULL;
  w->paint=paint_picture;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_picture;
  w->string=NULL;
  w->data=g;
  w->widget_data=data;

  return w;
}

void destroy_picture(GUI* g,WIDGET* w)
{
  struct picture_data_t* data=NULL;

  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  data=w->widget_data;
  free(data);
  free(w);
}

void set_picture_visible(WIDGET* w,int visible)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid Visible Flag\nNo Action Taken\n");
}

int get_picture_visible(WIDGET* w)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}

void set_picture_click_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  w->flags=w->flags|CLICKABLE;
  w->call=ucallback;
  w->data=data;
}

void set_picture_paint_click(WIDGET* w,void(*uclick)(GUI* g,Window win, WIDGET* w))
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  w->flags=w->flags|CLICKABLE;
  w->click=uclick;
}

void set_picture_paint_select(WIDGET* w,void(*uselect)(GUI* g,Window win, WIDGET* w),void(*ukey_press)(GUI* g,Window win,WIDGET* w, char key))
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  w->flags=w->flags|SELECTABLE;
  w->select=uselect;
  w->key_press=ukey_press;
}

void remove_picture_click_callback(WIDGET* w)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  if(w->click==NULL)
    w->flags=w->flags&(!CLICKABLE);
  w->call=NULL;
  w->data=NULL;
}

void remove_picture_paint_click(WIDGET* w)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  if(w->call==NULL)
    w->flags=w->flags&(!CLICKABLE);
  w->click=NULL;
}

void remove_picture_paint_select(WIDGET* w)
{
  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  w->flags=w->flags&(!SELECTABLE);
  w->select=NULL;
  w->key_press=NULL;
}
