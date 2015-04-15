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

  lab=create_label("Main Window",100,100);
  add_to_main(g,lab);

  show_main(g);

  w=create_sub_window(g,"Sub Window");
  set_sub_window_size(w,500,500);
  set_sub_window_visible(w,4);

  while(gui_running(g)){
    usleep(250000);
  }
  destroy_window(w);
  destroy_gui(g);
  return 0;
}
