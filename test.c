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
  destroy_window(g,win);

  show_main(g);
  while(gui_running(g)){
    usleep(250000);
  }
  destroy_gui(g);
  return 0;
}
