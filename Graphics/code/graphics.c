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
#include "link.h"
#include "queue.h"


void destroy_gui(GUI* g);
WIDGET* get_at_coords(SORTED_LIST* widgets,int x, int y);
char process_keystroke(GUI* g, XKeyEvent* e);

int to_gray(int color)
{
  int alpha,red,green,blue,avg,re;
  if((color&0x00FFFFFF)==0)
    return 0x00808080;
  if((color&0x00FFFFFF)==0x00FFFFFF)
    return 0x00AAAAAA;
  alpha=(color&0xFF000000)>>24;
  red=(color&0x00FF0000)>>16;
  green=(color&0x0000FF00)>>8;
  blue=color&0x000000FF;

  if(red==blue&&red==green){
    avg=(red+green+blue)/3;

    red=avg+0.1*red;
    green=avg+0.1*green;
    blue=avg+0.1*blue;
  }
  else {
    avg=red*0.21;
    avg=avg+green*0.71;
    avg=avg+blue*0.08;

    red=avg;
    green=avg;
    blue=avg;
  }

  re=(alpha<<24)|(red<<16)|(green<<8)|(blue);
  return re;
}
int window_comp(void* one, void* two)
{
  Window win=*(Window*)one;
  WINDOW* w=two;
  if(w->w==win){
    return 0;
    printf("EQUAL\n");
  }
  return 1;
}

void fake_free(void* data){}

int widget_compare(void* one, void* two)
{
	WIDGET* o=(WIDGET*)one;
	WIDGET* t=(WIDGET*)two;
	return (o->z_order)-(t->z_order);
}

void* event_loop(void* data)
{
  GUI* g=NULL;
  Window win;
  WINDOW* win_data=NULL;
  XEvent e;
  int keep_running,i;
  WIDGET* active=NULL;
  WIDGET* clicked=NULL;
  WIDGET* selected=NULL;
  WIDGET* temp=NULL;
  char key;

  g=(GUI*)data;
  pthread_mutex_lock(&g->lock);
  keep_running=g->run;
  pthread_mutex_unlock(&g->lock);

  while(keep_running==1){

    if(XPending(g->dsp)>0){
      XNextEvent(g->dsp,&e);
      win=e.xclient.window;
      switch (e.type) {
      case ClientMessage:
        if (e.xclient.message_type == g->wm_protocols &&
            e.xclient.data.l[0] == g->wm_delete_window){
	  if(win==g->mainWindow)  {
	    pthread_mutex_lock(&g->lock);
	    g->run=0;
	    pthread_mutex_unlock(&g->lock);
	  }
	  else{ //Not the main window so un map it
	    XUnmapWindow(g->dsp, win);
	  }
	}
	break;
      case ButtonPress:
	if(e.xbutton.button==1){//Left mouse Button
	  if(win==g->mainWindow)
	    clicked=get_at_coords(g->widgets,e.xbutton.x, e.xbutton.y);
	  else{
	    win_data=list_get(g->windows,&win); 
	    if(win_data!=NULL)
	      clicked=get_at_coords(win_data->widgets,e.xbutton.x, e.xbutton.y);
	  }
	  if(clicked!=NULL){
	    if(clicked!=selected&&selected!=NULL){
	      selected->paint(g,win,selected);
	      selected=NULL;
	    }
	    if((clicked->flags&(CLICKABLE|SELECTABLE))>0){
		active=clicked;
		if(active->click!=NULL)
		  active->click(g,win,active);
	    }
	    else{
	      active=NULL;
	    }
	  }
	  else{
	    active=NULL;
	    if(selected!=NULL)
	      selected->paint(g,win,selected);
	    selected=NULL;
	  }
	}
	break;
      case ButtonRelease:
	if(e.xbutton.button==1){//Left mouse Button
	  if(win==g->mainWindow)
	    clicked=get_at_coords(g->widgets,e.xbutton.x, e.xbutton.y);
	  else{
	    win_data=list_get(g->windows,&win); 
	    if(win_data!=NULL)
	      clicked=get_at_coords(win_data->widgets,e.xbutton.x, e.xbutton.y);
	  }
	  if(clicked!=NULL){
	    if(clicked==active){
	      if((clicked->flags&CLICKABLE)>0){
		if(active->call!=NULL)
		  active->call(g,active,active->data);
		active->paint(g,win,active);
	      }
	      if((clicked->flags&SELECTABLE)>0){
		selected=clicked;
		selected->click(g,win,selected);
	      }
	    }
	    else{
	      if(active!=NULL){
		active->paint(g,win,active);
	      }
	      active=NULL;
	    }
	  }
	  else{
	    if(active!=NULL)
	      active->paint(g,win,active);
	    active=NULL;
	  }
	}
	break;
      case KeyRelease:
	if(selected!=NULL){
	  key=process_keystroke(g,&e.xkey);
	  if(selected->key_press!=NULL)
	    selected->key_press(g,win,selected,key);
	}
	break;
      case Expose:
	//TODO Smart Repainting Code
	//break;
      case MapNotify: 
	if(win==g->mainWindow){
	  sorted_list_walk_reset(g->widgets);
	  while((temp=(WIDGET*)sorted_list_get_next(g->widgets))!=NULL){
	    temp->paint(g,win,temp);
	  }
	}
	else{
	  win_data=list_get(g->windows,&win); 
	  if(win_data!=NULL){
	    sorted_list_walk_reset(win_data->widgets);
	    while((temp=(WIDGET*)sorted_list_get_next(win_data->widgets))!=NULL){
		  temp->paint(g,win,temp);
	    }
	  }
	}
	break;
      case UnmapNotify:
      case ConfigureNotify:
      case ReparentNotify:
	// Ignore these events
	break;
      default:
	printf("Unhandled Event, Code: %d\n",e.type);
	break;
      }
    }
    else{
      pthread_mutex_lock(&g->lock);
      if(is_queue_empty(g->updates)!=1){
	temp=dequeue(g->updates);
	temp->paint(g,win,temp);
      }
      else{
	for(i=0;i<list_length(g->windows);i++){
	  win_data=list_get_pos(g->windows,i); 
	  if(is_queue_empty(win_data->updates)!=1){
	    temp=dequeue(win_data->updates);
	    temp->paint(g,win,temp);
	  }
	}
      }
      pthread_mutex_unlock(&g->lock);
    }
    pthread_mutex_lock(&g->lock);
    keep_running=g->run;
    pthread_mutex_unlock(&g->lock);
  }
  return NULL;
}

GUI* init_gui(char* address)
{
  GUI* g=NULL;
  Display* d=NULL;
  int re;

  g=malloc(sizeof(GUI));
  if(g==NULL){
    printf("Main GUI struct failed!\n");
    exit(-1);
  }

  d=XOpenDisplay(address);
  if(d==NULL){
    printf("Could not open display\n");
    exit(-1);
  }

  g->dsp=d;
  g->blackColor = BlackPixel(g->dsp, DefaultScreen(g->dsp));
  g->whiteColor = WhitePixel(g->dsp, DefaultScreen(g->dsp));
  g->bgColor=0x00AD855C;
  g->run=1;
  re=pthread_mutex_init(&g->lock,NULL);
  if(re!=0){
    printf("GUI Mutex Creation Failed\n");
    exit(-1);
  }
  g->wm_protocols = XInternAtom(g->dsp, "WM_PROTOCOLS", False);
  g->wm_delete_window = XInternAtom(g->dsp, "WM_DELETE_WINDOW", False);

  g->widgets=sorted_list_init(fake_free,widget_compare);
  g->updates=init_queue(fake_free);
  g->windows=list_init(fake_free,window_comp);

  g->font=XLoadQueryFont(g->dsp,"*9x15*");
  return g;
}

void create_main_window(GUI* g,char* title)
{
  Window w;

  if(g==NULL){
    printf("Can't create window, GUI is NULL\n");
    exit(-1);
  }
  w = XCreateSimpleWindow(g->dsp,DefaultRootWindow(g->dsp),0,0,100,100,0,0,g->bgColor);
  XSelectInput(g->dsp, w, StructureNotifyMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ExposureMask);

  g->mainWindow=w;
  if(title!=NULL)
    XStoreName(g->dsp,w,title);
  else
    XStoreName(g->dsp,w,"No Title");

  g->text=XCreateGC(g->dsp,g->mainWindow,0,NULL);
  g->draw=XCreateGC(g->dsp,g->mainWindow,0,NULL);
  XSetGraphicsExposures(g->dsp, g->draw, 0);
  XSetGraphicsExposures(g->dsp, g->text,0);
  XSetFont(g->dsp,g->text,g->font->fid);
  XSetForeground(g->dsp, g->text, g->blackColor);
}

void destroy_gui(GUI* g)
{
  WIDGET* w=NULL;
  int re;

  if(g==NULL){
    printf("GUI Never Opened\n");
    exit(-1);
  }
  XUnmapWindow(g->dsp,g->mainWindow);
  pthread_mutex_lock(&g->lock);
  g->run=0;
  pthread_mutex_unlock(&g->lock);

  pthread_join(g->tid,NULL);
  re=pthread_mutex_destroy(&g->lock);
  if(re!=0)
    printf("Mutex Destroy Failed\n");
  for(re=0;re<sorted_list_length(g->widgets);re++){
    w=sorted_list_get_pos(g->widgets,re);
    w->ufree(g,w);
  }
  for(re=0;re<list_length(g->windows);re++)
    destroy_window(g,list_get_pos(g->windows,re));
  list_destroy(g->windows);
  sorted_list_destroy(g->widgets);
  destroy_queue(g->updates);
  XFreeFont(g->dsp,g->font);
  XFreeGC(g->dsp,g->text);
  XFreeGC(g->dsp,g->draw);
  XCloseDisplay(g->dsp);
  free(g);
  g=NULL;

}

void shutdown_gui(GUI* g)
{
  pthread_mutex_lock(&g->lock);
  g->run=0;
  pthread_mutex_unlock(&g->lock);
}

void show_main(GUI* g)
{
  int re;
  XEvent e;
  if(g==NULL){
    printf("GUI Object is NULL, no wondow to show\n");
    exit(-1);
  }

  XMapWindow(g->dsp, g->mainWindow);
  while(XNextEvent(g->dsp,&e)){
    if(e.type==MapNotify)
      break;
  }
  re=pthread_create(&g->tid,NULL,event_loop,g);
  if(re!=0){
    printf("Thread Create for Event loop Failed\n");
    exit(-1);
  }
  XSetWMProtocols(g->dsp, g->mainWindow, &g->wm_delete_window, 1);

}

void set_main_size(GUI* g,int height, int width)
{
  if(g==NULL){
    printf("GUI Object NULL, No window\n");
    exit(-1);
  }
  XResizeWindow(g->dsp,g->mainWindow,width,height);
}

void set_main_background(GUI* g,int RGB)
{
  if(g==NULL){
    printf("GUI Is NULL, No BG color to change\n");
    exit(-1);
  }
  g->bgColor=RGB;
}

void set_main_icon(GUI* g,char* filename)
{
  XWMHints* hint=NULL;
  Pixmap p;
  if(g==NULL){
    printf("Can't set Icon, GUI is NULL\n");
    exit(-1);
  }
  if(strstr(filename, ".xpm") != NULL){
    if(access(filename,R_OK)!=-1){
      XpmReadFileToPixmap(g->dsp,g->mainWindow,filename,&p,NULL,NULL);
      hint=XAllocWMHints();
      hint->flags=IconPixmapHint|IconMaskHint;
      hint->icon_pixmap=p;
      hint->icon_mask=p;
      XSetWMHints(g->dsp,g->mainWindow,hint);
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

int gui_running(GUI* g)
{
  if(g==NULL)
    return 0;
  return g->run;
}

void add_to_main(GUI* g,WIDGET* w)
{
  if(g==NULL){
    printf("GUI is NULL, can't Add\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Widget is NULL, Can't add\n");
    exit(-1);
  }
  sorted_list_add(g->widgets,w);
}



WIDGET* get_at_coords(SORTED_LIST* widgets,int x, int y)
{
  WIDGET* temp=NULL;
  sorted_list_walk_reset(widgets);
  while((temp=sorted_list_get_next(widgets))!=NULL){
    if((temp->flags!=NONE)&&((temp->status&STATUS_VISIBLE)>0)&&((temp->status&STATUS_ENABLE)>0)){
      if(x>temp->x&&x<temp->x+temp->width){
	if(y>temp->y&&y<temp->y+temp->height){
	  return temp;
	}
      }
    }
  }
  return NULL;
}

char process_keystroke(GUI* g, XKeyEvent* e)
{
  char re=0;
  KeySym key_symbol=XkbKeycodeToKeysym(g->dsp, e->keycode, 0, e->state & ShiftMask ? 1 : 0);
  if(key_symbol>=XK_space && key_symbol<=XK_asciitilde){
    re=key_symbol - XK_space + ' ';
  }
  else{
    switch(key_symbol){
    case XK_KP_Insert:
      re='0';
      break;
    case XK_KP_End:
      re='1';
      break;
    case XK_KP_Down:
      re='2';
      break;
    case XK_KP_Page_Down:
      re='3';
      break;
    case XK_KP_Left:
      re='4';
      break;
    case XK_KP_Begin:
      re='5';
      break;
    case XK_KP_Right:
      re='6';
      break;
    case XK_KP_Home:
      re='7';
      break;
    case XK_KP_Up:
      re='8';
      break;
    case XK_KP_Page_Up:
      re='9';
      break;
   case XK_KP_Delete:
      re='.';
      break;
    case XK_KP_Add:
      re='+';
      break;
    case XK_KP_Subtract:
      re='-';
      break;
    case XK_KP_Multiply:
      re='*';
      break;
    case XK_KP_Divide:
      re='/';
      break;
    case XK_BackSpace:
      re=BACKSPACE;
      break;
    case XK_Tab:
      re=TAB;
      break;
    case XK_Return:
    case XK_KP_Enter:
      re=ENTER;
      break;
    case XK_Left:
      re=LEFT;
      break;
    case XK_Right:
      re=RIGHT;
      break;
    case XK_Up:
      re=UP;
      break;
    case XK_Down:
      re=DOWN;
      break;
    case XK_Delete:
      re=DELETE;
      break;
      default: //Upmapped Key we dont care about
	//printf("Key still Unbound\n");
	;
    }
  }
  return re;
}

void update_widget(GUI* g,WIDGET* w)
{
  if(g==NULL){
    printf("GUI object is NULL!\n");
    exit(-1);
  }
  pthread_mutex_lock(&g->lock);
  enqueue(g->updates,w);
  pthread_mutex_unlock(&g->lock);
}

void refresh_main_window(GUI* g)
{
  if(g==NULL){
    printf("GUI object is NULL!\n");
    exit(-1);
  }
  pthread_mutex_lock(&g->lock);
  XClearWindow(g->dsp,g->mainWindow);
  int i;
  for(i=0;i<sorted_list_length(g->widgets);i++)
    enqueue(g->updates,sorted_list_get_pos(g->widgets,i));
  pthread_mutex_unlock(&g->lock);
}

void register_window(GUI* g, WINDOW* w)
{
  if(g==NULL){
    printf("GUI is NULL cant add!\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Window is NULL, can't add\n");
    exit(-1);
  }
  pthread_mutex_lock(&g->lock);
  list_add_tail(g->windows,w);
  pthread_mutex_unlock(&g->lock);
}
void unregister_window(GUI* g, WINDOW* w)
{
  if(g==NULL){
    printf("GUI is NULL cant Remove!\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Window is NULL, can't Remove\n");
    exit(-1);
  }
  pthread_mutex_lock(&g->lock);
  list_delete(g->windows,&w->w);
  pthread_mutex_unlock(&g->lock);
}
