#include "WriteChords.h"

#define NUM_MENU_ITEMS NUM_CHORD_BASES

static char* chord_base[NUM_CHORD_BASES] = {"A","A#/Bb","B","C","C#/Dd","D","D#/Eb","E","F","F#/Gb","G","G#/Ab"};
static uint16_t chord_type_num;

static Window *window;
static MenuLayer *menu_layer;


// callback used to specify the height of the section header
static int16_t chord_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t chord_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
    return NUM_MENU_ITEMS;
}

// Draw headers
static void chord_menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Draw title text in seciton header
  menu_cell_basic_header_draw(ctx, cell_layer, "Choose Chord");
}

// draw items
static void chord_menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    menu_cell_title_draw(ctx, cell_layer, chord_base[cell_index->row]);
}

// capture when user selects menu tiem
void chord_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  setup_chord_display(chord_type_num, cell_index->row);
}

// inits menu upon window load
void chord_window_load(Window *window)
{
  // prep to initialize the menu layer
  // bounds specify menu layer's viewport size
  // in this case, same as window's size
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // create menu layer
  menu_layer = menu_layer_create(bounds);

  
  // set all callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_header_height = chord_menu_get_header_height_callback,
    .get_num_rows = chord_menu_get_num_rows_callback,
    .draw_header = chord_menu_draw_header_callback,
    .draw_row = chord_menu_draw_row_callback,
    .select_click = chord_menu_select_callback,
  });


  // bind menu layer's click config provider to window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // add to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void cleanup_chord_menu(Window *window) {
  // destroy menu layer
  menu_layer_destroy(menu_layer);
  window_destroy(window);
}

void setup_chord_menu(uint16_t ct_num)
{
  window = window_create();
  chord_type_num = ct_num;
  
  window_set_window_handlers(window, (WindowHandlers) {
      .load = chord_window_load,
      .unload = cleanup_chord_menu,
});

  window_stack_push(window, true);
}

