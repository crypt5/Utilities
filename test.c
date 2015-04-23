#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;

  g=init_gui();
  create_main_window(g,"Main Window");
  set_main_icon(g,"pic.xpm");
  set_main_size(g,500,500);


  show_main(g);
  while(gui_running(g)){
    usleep(5000);
  }

  destroy_gui(g);
  return 0;
}
