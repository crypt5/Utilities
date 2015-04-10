#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graphics.h"
#include "logger.h"
#include "config.h"
#include "BBBiolib.h"

void click(GUI* g, WIDGET* self, void* data)
{
  WIDGET* text=data;
  int visible=get_textfield_enable(text);
  if(visible==1)
    set_textfield_enable(text,0);
  else
    set_textfield_enable(text,1);

  refresh_main_window(g);
}


int main()
{
  GUI* g=NULL;
  LOGGER* log=NULL;
  WIDGET* text=NULL;
  WIDGET* but=NULL;
  WIDGET* sep=NULL;

  log=logger_init("test.log");
  int i;
  for(i=0;i<100;i++)
    logger_log(log,"Test Message");
  logger_shutdown(log);

  g=init_gui();
  create_main_window(g,"Textbox Testing");
  set_main_size(g,500,500);

  text=create_textfield(10,10,20);

  but=create_button("Click me",10,50);
  set_button_callback(but,click,text);

  sep=create_seperator(10,100,300);
  set_seperator_color(sep,0x00FF00FF);
  set_seperator_thickness(sep,5);

  add_to_main(g,text);
  add_to_main(g,but);
  add_to_main(g,sep);

  show_main(g);
  while(gui_running(g)){
    usleep(250000);
  }

  destroy_gui(g);
  return 0;
}
