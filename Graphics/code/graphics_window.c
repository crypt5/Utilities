#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/xpm.h>
#include <pthread.h>
#include <string.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_window.h"
#include "link.h"
#include "queue.h"

void fake_free2(void* data){}

WINDOW* create_window(GUI* g,char* title, int bgColor)
{
  WINDOW* w=NULL;
  Window win;
  int bg;
  if(g==NULL){
    printf("Main GUI is not created\n");
    exit(-1);
  }
  w=malloc(sizeof(WINDOW));
  if(w==NULL){
    printf("Window Malloc failed!\n");
    exit(-1);
  }
  if(bgColor<0)
    bg=g->bgColor;
  else
    bg=bgColor;

  win=XCreateSimpleWindow(g->dsp,DefaultRootWindow(g->dsp),0,0,100,100,0,0,bg);
  XSelectInput(g->dsp, win, StructureNotifyMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ExposureMask);

  if(title!=NULL)
    XStoreName(g->dsp,win,title);
  else
    XStoreName(g->dsp,win,"No Title");
  w->w=win;
  w->widgets=list_init(fake_free2,NULL);
  w->updates=init_queue(fake_free2);
  XSetWMProtocols(g->dsp, w->w, &g->wm_delete_window, 1);
  return w;
}

void destroy_window(GUI* g,WINDOW* win)
{
  WIDGET* w=NULL;
  int i;
  if(g==NULL){
    printf("GUI doesn't exist! Can't delete window\n");
    exit(-1);
  }
  if(win==NULL){
    printf("Window doesn't exist!\n");
    exit(-1);
  }
  
  for(i=0;i<list_length(win->widgets);i++){
    w=list_get_pos(win->widgets,i);
    w->ufree(g,w);
  }
  
  list_destroy(win->widgets);
  destroy_queue(win->updates);
  free(win);
  win=NULL;
}

void set_window_visible(GUI* g,WINDOW* win,int visible)
{
  if(g==NULL){
    printf("GUI doesn't exist! Can't Set an Icon window\n");
    exit(-1);
  }
  if(win==NULL){
    printf("Window doesn't exist!\n");
    exit(-1);
  }
  if(visible==1)
    XMapWindow(g->dsp,win->w);
  else if(visible==0)
    XUnmapWindow(g->dsp,win->w);
  else
    printf("Invalid visible identifier: %d\nNo action taken\n",visible);
}

void set_window_size(GUI* g,WINDOW* win, int height, int width)
{
  if(g==NULL){
    printf("GUI doesn't exist! Can't show window\n");
    exit(-1);
  }
  if(win==NULL){
    printf("Window doesn't exist!\n");
    exit(-1);
  }
  XResizeWindow(g->dsp,win->w,width,height);
}

void set_window_icon(GUI* g, WINDOW* win,char* filename)
{
  XWMHints* hint=NULL;
  Pixmap p;
  if(g==NULL){
    printf("Can't set Icon, GUI is NULL\n");
    exit(-1);
  }
  if(win==NULL){
    printf("Window doesn't exist!\n");
    exit(-1);
  }
  if(strstr(filename, ".xpm") != NULL){
    if(access(filename,R_OK)!=-1){
      XpmReadFileToPixmap(g->dsp,win->w,filename,&p,NULL,NULL);
      hint=XAllocWMHints();
      hint->flags=IconPixmapHint;
      hint->icon_pixmap=p;
      hint->icon_x=0;
      hint->icon_y=0;
      XSetWMHints(g->dsp,win->w,hint);
      XFree(hint);
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
}

void add_widget_to_window(WINDOW* win, WIDGET* w)
{
  if(win==NULL){
    printf("Window is NULL, can't Add\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Widget is NULL, Can't add\n");
    exit(-1);
  }
  list_add_tail(win->widgets,w);
}

void update_window_widget(WINDOW* win, WIDGET* w)
{
  if(win==NULL){
    printf("Window is NULL, can't Add\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Widget is NULL, Can't add\n");
    exit(-1);
  }
  enqueue(win->updates,w);
}
void ok_popup_callback(GUI* g, WIDGET* w,void* data){*(int*)data=1;}
int ok_popup(GUI* g,char* message, char* title,int popup_type)
{
  WINDOW* win=NULL;
  int clicked=0;
  WIDGET* lab=NULL;
  WIDGET* but=NULL;

  win=create_window(g,title,-1);
  set_window_size(g,win,150,500);
  lab=create_label(message,75,30);
  but=create_button("OK",230,120);
  set_button_callback(but,ok_popup_callback,&clicked);

  add_widget_to_window(win,lab);
  add_widget_to_window(win,but);

  register_window(g,win);
  set_window_visible(g,win,1);
  while(clicked==0)
    usleep(5000);  
  
  set_window_visible(g,win,0);
  
  unregister_window(g,win);
  destroy_window(g,win);
  return clicked;
}
