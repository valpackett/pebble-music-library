#include <pebble.h>
#include "data.c"
#include "songs.h"
#include "albums.h"

static Window *songs_window;
static MenuLayer *songs_layer;
static Entry *songs_entries = NULL;
static uint16_t songs_entries_count = 0;
static uint16_t songs_entries_max_count = 0;
static int8_t songs_parent_data_type = -1;
static int32_t songs_parent_id = -1;

static int16_t songs_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  return 28;
}

static void songs_draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  Entry song = songs_entries[cell_index->row];
  if (song.name) menu_cell_basic_draw(ctx, cell_layer, song.name, NULL, NULL);
}

static uint16_t songs_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return songs_entries_count;
}

static void songs_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  Entry song = songs_entries[cell_index->row];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Clicked song #%d -- %s", cell_index->row, song.name);
  request_play(songs_parent_data_type, songs_parent_id, cell_index->row);
}

static void songs_free_entries() {
  free(songs_entries);
  songs_entries = NULL;
}

void songs_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  songs_layer = menu_layer_create(window_frame);
  menu_layer_set_callbacks(songs_layer, NULL, (MenuLayerCallbacks) {
    .get_cell_height = (MenuLayerGetCellHeightCallback) songs_get_cell_height_callback,
    .draw_row = (MenuLayerDrawRowCallback) songs_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) songs_get_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) songs_select_callback,
  });
  menu_layer_set_click_config_onto_window(songs_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(songs_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Song window loaded");
}

void songs_window_unload(Window *window) {
  if (songs_entries) songs_free_entries();
  songs_entries_count = 0;
  menu_layer_destroy(songs_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Song window unloaded");
}

void songs_init() {
  songs_window = window_create();
  window_set_window_handlers(songs_window, (WindowHandlers) {
    .load = songs_window_load,
    .unload = songs_window_unload,
  });
}

void songs_show(int8_t parent_type, int8_t parent_play_type, int8_t parent_id) {
  const bool animated = true;
  window_stack_push(songs_window, animated);
  request_data(parent_type, parent_id);
  songs_parent_data_type = parent_play_type;
  songs_parent_id = parent_id;
}

void songs_deinit() {
  window_destroy(songs_window);
  songs_free_entries();
}

bool songs_is_active() {
  return window_stack_get_top_window() == songs_window;
}

void songs_content_start(uint16_t count) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Song count received: %d", count);
  if (songs_entries) songs_free_entries();
  songs_entries = malloc(count * sizeof(Entry));
  songs_entries_max_count = count;
  songs_entries_count = 0;
}

void songs_content_add(uint16_t index, uint32_t id, char* name) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Song #%d added: %s [%lu]", index, name, (unsigned long) id);
  songs_entries[index].id = id;
  fucking_copy_string(songs_entries[index].name, name, ENTRY_NAME_LEN);
  songs_entries_count++;
  if (songs_layer) menu_layer_reload_data(songs_layer);
}
