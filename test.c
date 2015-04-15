#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  WIDGET* lab=NULL;
  WINDOW* w=NULL;

  g=init_gui();
  create_main_window(g,"Main Window");
  set_main_size(g,500,500);

  w=create_sub_window(g,"Sub Window");
  destroy_window(g,w);
  
  lab=create_label("Main Window",100,100);
  add_to_main(g,lab);

  show_main(g);
  while(gui_running(g)){
    usleep(250000);
  }

  destroy_gui(g);
  return 0;
}
