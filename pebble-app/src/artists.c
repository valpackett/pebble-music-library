#include <pebble.h>
#include "data.c"
#include "artists.h"
#include "albums.h"

static Window *artists_window;
static MenuLayer *artists_layer;
static Entry *artists_entries = NULL;
static uint16_t artists_entries_count = 0;
static uint16_t artists_entries_max_count = 0;

static int16_t artists_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  return 28;
}

static void artists_draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  Entry artist = artists_entries[cell_index->row];
  if (artist.name) menu_cell_basic_draw(ctx, cell_layer, artist.name, NULL, NULL);
}

static uint16_t artists_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return artists_entries_count;
}

static void artists_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  Entry artist = artists_entries[cell_index->row];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Clicked artist #%d -- %s", cell_index->row, artist.name);
  if (artist.id) albums_show(artist.id);
}

static void artists_free_entries() {
  free(artists_entries);
  artists_entries = NULL;
}

void artists_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  artists_layer = menu_layer_create(window_frame);
  menu_layer_set_callbacks(artists_layer, NULL, (MenuLayerCallbacks) {
    .get_cell_height = (MenuLayerGetCellHeightCallback) artists_get_cell_height_callback,
    .draw_row = (MenuLayerDrawRowCallback) artists_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) artists_get_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) artists_select_callback,
  });
  menu_layer_set_click_config_onto_window(artists_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(artists_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Artist window loaded");
}

void artists_window_unload(Window *window) {
  if (artists_entries) artists_free_entries();
  artists_entries_count = 0;
  menu_layer_destroy(artists_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Artist window unloaded");
}

void artists_init() {
  artists_window = window_create();
  window_set_window_handlers(artists_window, (WindowHandlers) {
    .load = artists_window_load,
    .unload = artists_window_unload,
  });
}

void artists_show() {
  const bool animated = true;
  window_stack_push(artists_window, animated);
  request_data(GET_ARTISTS, -1);
}

void artists_deinit() {
  window_destroy(artists_window);
  artists_free_entries();
}

bool artists_is_active() {
  return window_stack_get_top_window() == artists_window;
}

void artists_content_start(uint16_t count) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Artist count received: %d", count);
  if (artists_entries) artists_free_entries();
  artists_entries = malloc(count * sizeof(Entry));
  artists_entries_max_count = count;
  artists_entries_count = 0;
}

void artists_content_add(uint16_t index, uint32_t id, char* name) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Artist #%d added: %s [%lu]", index, name, (unsigned long) id);
  artists_entries[index].id = id;
  fucking_copy_string(artists_entries[index].name, name, ENTRY_NAME_LEN);
  artists_entries_count++;
  if (artists_layer) menu_layer_reload_data(artists_layer);
}
