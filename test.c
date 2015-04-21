#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

void button_call(GUI* g, WIDGET* w, void* data)
{
  static int visible=1;
  set_window_visible(g,(WINDOW*)data,visible);
  if(visible==0)
    visible=1;
  else
    visible=0;
}

int main()
{
  GUI* g=NULL;
  WIDGET* lab=NULL;
  //WIDGET* lab_sub=NULL;
  WIDGET* show=NULL;
  //WINDOW* win=NULL;

  g=init_gui();
  create_main_window(g,"Main Window");
  set_main_size(g,500,500);

  lab=create_label("Main Window",100,100);
  //lab_sub=create_label("Sub Window",100,100);
  add_to_main(g,lab);

  show=create_button("Show Window",100,140);
  add_to_main(g,show);

  //win=create_window(g,"Sub Window",-1);
  //set_window_size(g,win,300,300);
  //register_window(g,win);

  //add_widget_to_window(win,lab_sub);

  //set_button_callback(show,button_call,win);
  show_main(g);

  ok_popup(g,"This is a Warning","Warning",POPUP_TYPE_WARNING);

  while(gui_running(g)){
    usleep(5000);
  }

  destroy_gui(g);
  return 0;
}
