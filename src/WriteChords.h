#ifndef __WRITECHORDS_H__
#define __WRITECHORDS_H__

#include "pebble.h"

#define NUM_CHORD_BASES 12
#define NUM_CHORD_TYPES 2

/* Data Structs */
typedef unsigned char byte;

typedef struct fingering {
  byte fret;
  byte fing; // this is the number of the finger
} fingering;
/* the struct will go if the fret is 0 or x, never even look at fing */

typedef struct chord {
  char* name;
  fingering* fing; //len = 6
  byte pos; // this is the initial position of first shown fret
} chord;  
  
/**********************
 *   chord_menu.c   *
 **********************/
//void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
void chord_window_load(Window *window);
//void chord_window_unload(Window *window); 
void setup_chord_menu(uint16_t c_num);
void cleanup_chord_menu(Window *window);

/*************************
 *   chord_type_menu.c   *
 *************************/
void chord_type_window_load(Window *window);
void setup_chord_type_menu();
void cleanup_chord_type_menu(Window *window);
/********************
 *   read_files.c   * 
 ********************/


/***********************
 *   chord_display.c   *
 ***********************/
void setup_chord_display(uint16_t ct_num, uint16_t c_num);
void cleanup_chord_display();
/*********************
 *   chord_write.c   *
 **********************/
// Has window with 7 - 8 chords
// Select to write to file
// handles the multiple key thing

/*******************
 *   song_view.c   *
 *******************/
//see all the chords in your song
//should be able to scroll
//BPM and scale included

/*********************
 *   WriteChords.c   *
 *********************/

#endif

