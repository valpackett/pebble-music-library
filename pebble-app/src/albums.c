#include <pebble.h>
#include "data.c"
#include "albums.h"
#include "songs.h"

static Window *albums_window;
static MenuLayer *albums_layer;
static Entry *albums_entries = NULL;
static uint16_t albums_entries_count = 0;
static uint16_t albums_entries_max_count = 0;

static int16_t albums_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  return 28;
}

static void albums_draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  Entry album = albums_entries[cell_index->row];
  if (album.name) menu_cell_basic_draw(ctx, cell_layer, album.name, NULL, NULL);
}

static uint16_t albums_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return albums_entries_count;
}

static void albums_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  Entry album = albums_entries[cell_index->row];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Clicked album #%d -- %s", cell_index->row, album.name);
  if (album.id) songs_show(GET_ALBUM_SONGS, PLAY_ALBUM_SONG, album.id);
}

static void albums_free_entries() {
  free(albums_entries);
  albums_entries = NULL;
}

static void albums_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  albums_layer = menu_layer_create(window_frame);
  menu_layer_set_callbacks(albums_layer, NULL, (MenuLayerCallbacks) {
    .get_cell_height = (MenuLayerGetCellHeightCallback) albums_get_cell_height_callback,
    .draw_row = (MenuLayerDrawRowCallback) albums_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) albums_get_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) albums_select_callback,
  });
  menu_layer_set_click_config_onto_window(albums_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(albums_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Album window loaded");
}

static void albums_window_unload(Window *window) {
  if (albums_entries) albums_free_entries();
  albums_entries_count = 0;
  menu_layer_destroy(albums_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Album window unloaded");
}

void albums_init() {
  albums_window = window_create();
  window_set_window_handlers(albums_window, (WindowHandlers) {
    .load = albums_window_load,
    .unload = albums_window_unload,
  });
}

void albums_show(int8_t artist_id) {
  const bool animated = true;
  window_stack_push(albums_window, animated);
  request_data(GET_ALBUMS, artist_id);
}

void albums_deinit() {
  window_destroy(albums_window);
  albums_free_entries();
}

bool albums_is_active() {
  return window_stack_get_top_window() == albums_window;
}

void albums_content_start(uint16_t count) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Album count received: %d", count);
  if (albums_entries) albums_free_entries();
  albums_entries = malloc(count * sizeof(Entry));
  albums_entries_max_count = count;
  albums_entries_count = 0;
}

void albums_content_add(uint16_t index, uint32_t id, char* name) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Album #%d added: %s [%lu]", index, name, (unsigned long) id);
  albums_entries[index].id = id;
  fucking_copy_string(albums_entries[index].name, name, ENTRY_NAME_LEN);
  albums_entries_count++;
  if (albums_layer) menu_layer_reload_data(albums_layer);
}
