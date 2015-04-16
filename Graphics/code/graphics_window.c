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
