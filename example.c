#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graphics.h"

void build_labs(GUI* g)
{
  WIDGET* lab1=NULL;
  WIDGET* lab2=NULL;
  WIDGET* lab3=NULL;

  lab1=create_label("Label",10,10);
  lab2=create_label("Label not Enabled",80,10);
  set_label_enable(lab2,0);
  lab3=create_label("Fancy Label",250,10);
  set_label_text_color(lab3,0x00FFFFFF);
  set_label_background(lab3,0x00000000);
  set_label_border(lab3,0x00FF0000,1);

  add_to_main(g,lab1);
  add_to_main(g,lab2);
  add_to_main(g,lab3);
}

void build_buts(GUI* g)
{
  WIDGET* but1=NULL;
  WIDGET* but2=NULL;
  WIDGET* but3=NULL;

  but1=create_button("Button",10,50);
  but2=create_button("Not Enabled Button",105,50);
  set_button_enable(but2,0);
  but3=create_button("Fancy Button",300,50);
  set_button_text_color(but3,0x0000FF00);

  add_to_main(g,but1);
  add_to_main(g,but2);
  add_to_main(g,but3);
}

void build_check(GUI* g)
{
  WIDGET* ch1=NULL;
  WIDGET* ch2=NULL;
  WIDGET* ch3=NULL;

  ch1=create_checkbox("Checkbox",10,80);
  ch2=create_checkbox("Disabled Checkbox",120,80);
  set_checkbox_enable(ch2,0);
  ch3=create_checkbox("Fancy Checkbox",320,80);
  set_checkbox_text_color(ch3,0x000000FF);
  set_checkbox_check_color(ch3,0x00FF0000);

  add_to_main(g,ch1);
  add_to_main(g,ch2);
  add_to_main(g,ch3);
}

void build_radio(GUI* g)
{
  WIDGET* ch1=NULL;
  WIDGET* ch2=NULL;
  WIDGET* ch3=NULL;

  ch1=create_radio_button("Radio",10,120);
  ch2=create_radio_button("Disabled Radio",120,120);
  set_radio_button_enable(ch2,0);
  ch3=create_radio_button("Fancy Radio",320,120);
  set_radio_button_text_color(ch3,0x0000FF00);
  set_radio_button_check_color(ch3,0x00000001);

  add_to_main(g,ch1);
  add_to_main(g,ch2);
  add_to_main(g,ch3);

}

void build_text(GUI* g)
{
  WIDGET* ch1=NULL;
  WIDGET* ch2=NULL;
  WIDGET* ch3=NULL;

  ch1=create_textfield(10,160,15);
  set_textfield_text(ch1,"Textfield");
  ch2=create_textfield(160,160,15);
  set_textfield_text(ch2,"Disabled");
  set_textfield_enable(ch2,0);
  ch3=create_textfield(320,160,15);
  set_textfield_text(ch3,"Fancy");
  set_textfield_text_color(ch3,0x00FF00FF);
  set_textfield_background_color(ch3,0x00000000);

  add_to_main(g,ch1);
  add_to_main(g,ch2);
  add_to_main(g,ch3);
}

void build_misc(GUI* g)
{
  WIDGET* border1=NULL;
  WIDGET* border2=NULL;
  WIDGET* title1=NULL;
  WIDGET* title2=NULL;
  WIDGET* sep1=NULL;
  WIDGET* sep2=NULL;

  border1=create_simple_border(10,190,20,80);
  border2=create_simple_border(200,190,20,80);
  set_simple_border_color(border2,0x00FF0000);
  set_simple_border_thickness(border2,3);

  title1=create_titled_border("Titled Border",10,230,20,150);
  title2=create_titled_border("Fancy Border",220,230,20,150);
  set_titled_border_color(title2,0x00008800);
  set_titled_border_thickness(title2,3);
  set_titled_border_text_color(title2,0x00990000);

  sep1=create_seperator(20,280,200);
  sep2=create_seperator(260,280,200);
  set_seperator_color(sep2,0x00000099);
  set_seperator_thickness(sep2,4);

  add_to_main(g,border1);
  add_to_main(g,border2);
  add_to_main(g,title1);
  add_to_main(g,title2);
  add_to_main(g,sep1);
  add_to_main(g,sep2);
}

int main()
{
  GUI* g=NULL;
  WIDGET* pic=NULL;

  g=init_gui();
  create_main_window(g,"Example GUI");
  set_main_size(g,500,500);
  
  build_labs(g);
  build_buts(g);
  build_check(g);
  build_radio(g);
  build_text(g);
  build_misc(g);

  
  pic=create_picture(g,"pic.xpm",10,300);
  add_to_main(g,pic);
  
  show_main(g);
  while(gui_running(g)){
    usleep(250000);
  }

  destroy_gui(g);
  return 0;
}


