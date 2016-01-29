#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

/* Uncomment to print out the debug images
 * Create a folder in the main program dir
 * called pic_output to hold the pictures
 */
//#define DEBUG_PRINT_IMAGES

//Widget Types (decimal)
#define LABEL 1
#define BUTTON 2
#define RADIO_BUTTON 3
#define CHECKBOX 4
#define BORDER 5
#define TITLE_BORDER 6
#define TEXTFIELD 7
#define PICTURE 8
#define COMBOBOX 9
#define SEPERATOR 10

//Widget Action FLags (binary)
#define NONE 0x00 
#define CLICKABLE 0x01
#define SELECTABLE 0x02

//Widget Status Flags (binary)
#define STATUS_NONE 0
#define STATUS_VISIBLE 0x01
#define STATUS_ENABLE 0x02
#define STATUS_REPAINT 0x04


typedef struct graphics_widget_t WIDGET;
#include "graphics.h"


typedef void(*callback)(GUI* g,WIDGET* self,void* data);
typedef void(*painter)(GUI* g,Window win, WIDGET* w);
typedef void(*on_click)(GUI* g,Window win, WIDGET* w);
typedef void(*on_select)(GUI* g,Window win, WIDGET* w);
typedef void(*on_keypress)(GUI* g,Window win,WIDGET* w, char key);
typedef void(*free_widget)(GUI* g,WIDGET* widget);

struct graphics_widget_t{
  char type;
  char flags;
  char status;
  int x;
  int y;
  int height;
  int width;
  int z_order;
  callback call;     //Button Press callback
  painter paint;     //Function to paint widget
  on_click click;    //Function to paint when widget is clicked
  on_select select;  //Function to print widget when selected
  on_keypress key_press; //Function to pass key press to
  free_widget ufree; //Function to free widget's memory
  char* string;
  void* data;        //callback data
  void* widget_data; //Special widget data
};

#include "graphics_label.h"
#include "graphics_button.h"
#include "graphics_radio_button.h"
#include "graphics_checkbox.h"
#include "graphics_border.h"
#include "graphics_titled_border.h"
#include "graphics_textfield.h"
#include "graphics_picture.h"
#include "graphics_combobox.h"
#include "graphics_seperator.h"

#endif
