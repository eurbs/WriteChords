#include "WriteChords.h"
  
static Window *window;
static Layer *layer;
static GBitmap *image;

static uint16_t ct_num; // chord type index number
static uint16_t c_num; // chord index number

//static chord c;

static uint8_t col_posns[] = {22, 42, 62, 82, 102, 122};
static uint8_t row_posns[] = {36, 52, 78, 104, 130};

static char* chord_base[NUM_CHORD_BASES] = {"A","A#/Bb","B","C","C#/Dd","D","D#/Eb","E","F","F#/Gb","G","G#/Ab"};
static char* chord_types[NUM_CHORD_TYPES] = {"Major", "Minor"};

ResHandle read_chord(void)
{
  switch(c_num){
    case 0: return resource_get_handle(RESOURCE_ID_CHORD_A);
    case 1: return resource_get_handle(RESOURCE_ID_CHORD_AB);
    case 2: return resource_get_handle(RESOURCE_ID_REF_B);
    case 3: return resource_get_handle(RESOURCE_ID_REF_C);
    case 4: return resource_get_handle(RESOURCE_ID_REF_CD);
    case 5: return resource_get_handle(RESOURCE_ID_REF_D);
    case 6: return resource_get_handle(RESOURCE_ID_REF_DE);
    case 7: return resource_get_handle(RESOURCE_ID_REF_E);
    case 8: return resource_get_handle(RESOURCE_ID_REF_F);
    case 9: return resource_get_handle(RESOURCE_ID_REF_FG);
    case 10: return resource_get_handle(RESOURCE_ID_REF_G);
    case 11: return resource_get_handle(RESOURCE_ID_REF_GA);
    default: return resource_get_handle(RESOURCE_ID_CHORD_A); // to silence compiler
  }
}

// DRAW THE CHORDS
void draw_chords(chord c)
{
  // draw 'dem chords
}

// CHAR TO NUM
uint8_t char_to_num(char c){
  switch(c){
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    default: return 0;
  }
}

void getChordName(char *chord_name){
  int baselen = strlen(chord_base[c_num]);
  int typelen = strlen(chord_types[ct_num]);
  int i;
  for(i = 0; i < baselen; i++)
    chord_name[i] = chord_base[c_num][i];
  chord_name[i++] = ' ';
  for(; i < typelen + baselen + 1; i++)
    chord_name[i] = chord_types[ct_num][i - 1 - baselen];
  chord_name[i] = '\0';
  return;
}

static void layer_update_callback(Layer *me, GContext *ctx) {
  int i;
  // get the name of the chord]
  char chord_name[16];
  getChordName(&chord_name[0]);
  
 //DEBUG 
 // APP_LOG(APP_LOG_LEVEL_DEBUG, "ct_num: %u c_num: %u name %s", ct_num, c_num, chord_name);
  
  // get the info for the chord according to reference
  ResHandle chord_info = read_chord();
  uint8_t buff[16]; // SELF : really gotta get rid of 'byte' and just use uint8_t
  uint8_t max = 0;
  resource_load_byte_range(chord_info, 16 * ct_num, buff, 16);
  fingering f[6];
  for (i = 0; i < 6; i++){
    f[i].fret = char_to_num(buff[i]); // CHAR TO NUM --> maybe i should have the binary refs with the numbers :O
    if(buff[i] > max)
      max = buff[i];
    f[i].fing= buff[i+7]; //char to num with x included :O
  }
  
  // get the position of the chord
  uint8_t pos = max < 5 ? 0 : max -  2; // SELF come up with actual equation here
  
  chord c;
  // assign all to c
  c.name = chord_name;
  // ResHandle - thanks for the struggle
  c.fing = f;
  // last but not least
  c.pos = pos;
  
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.
  GRect bounds = image->bounds;

  // draw background
  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 0, 0 }, .size = bounds.size });
 

  // draw text
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, c.name, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                     (GRect) { .origin = { 0, 0}, .size = bounds.size }, 
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  
// draw dots!! and numbers in dots
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorWhite);
//  uint8_t x, y;
  int yi;
  GPoint dot_center, dot_center2;
  char finger_num[2];
  finger_num[1] = '\0';
//  bounds.size.h = bounds.size.h - 22;
  for (i = 0; i < 6; i++){
    yi = c.fing[i].fret;
    dot_center = (GPoint) {col_posns[i], row_posns[yi]};
    dot_center2 = (GPoint) {col_posns[i] - 4, row_posns[yi] - 12};
    
    if (yi) {
      finger_num[0] = c.fing[i].fing;
      graphics_fill_circle(ctx, dot_center, 8);
      graphics_draw_text(ctx, finger_num, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                         (GRect) { .origin = dot_center2, .size = bounds.size }, 
                         GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
//      APP_LOG(APP_LOG_LEVEL_DEBUG, "fingering: %s", finger_num);
    }
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "i: %d String: %d String Pos: %d Fret: %d Fret Pos: %d", 
//           i, i, col_posns[i], c.fing[i].fret, row_posns[c.fing[i].fret]);
  }
}

void cleanup_chord_display()
{
  gbitmap_destroy(image);

  window_destroy(window);
  layer_destroy(layer);
}

void setup_chord_display(uint16_t ct_numm, uint16_t c_numm)
{
  ct_num = ct_numm;
  c_num = c_numm;
  
  window = window_create();
  window_stack_push(window, true /* Animated */);

  // Init the layer for display the image
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, layer_update_callback);
  layer_add_child(window_layer, layer);
  
  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRET_BOARD);
  
  window_set_window_handlers(window, (WindowHandlers) {
    .unload = cleanup_chord_display,
 });

  
}

