#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  WIDGET* lab=NULL;
  WINDOW* win=NULL;

  g=init_gui();
  create_main_window(g,"Main Window");
  set_main_size(g,500,500);

  lab=create_label("Main Window",100,100);
  add_to_main(g,lab);

  win=create_window(g,"test",-1);
  set_window_size(g,win,300,300);
  register_window(g,win);

  show_main(g);
  int i=0;
  while(gui_running(g)){
    set_window_visible(g,win,i);
    usleep(1000000);
    if(i==0)
      i++;
    else
      i--;
  }
  destroy_gui(g);
  return 0;
}
