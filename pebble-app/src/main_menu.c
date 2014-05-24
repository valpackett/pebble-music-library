#include "main_menu.h"

#define ARTISTS_ITEM 0
#define ALBUMS_ITEM 1
#define PLAYLISTS_ITEM 2

static Window *main_menu_window;
static MenuLayer *main_menu_layer;

static int16_t main_menu_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  return 44;
}

static void main_menu_draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  char* text = "XXXXXXXXXXX";
  switch(cell_index->row) {
    case ARTISTS_ITEM: text = "Artists"; break;
    case ALBUMS_ITEM: text = "Albums"; break;
    case PLAYLISTS_ITEM: text = "Playlists"; break;
  }
  menu_cell_basic_draw(ctx, cell_layer, text, NULL, NULL);
}

static uint16_t main_menu_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return 3;
}

static void main_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch(cell_index->row) {
    case ARTISTS_ITEM: entry_list_show(artists, ARTISTS, -1, -1); break;
    case ALBUMS_ITEM: entry_list_show(albums, ALBUMS, -1, -1); break;
    case PLAYLISTS_ITEM: entry_list_show(playlists, PLAYLISTS, -1, -1); break;
  }
}

void main_menu_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  main_menu_layer = menu_layer_create(window_frame);
  menu_layer_set_callbacks(main_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_cell_height = (MenuLayerGetCellHeightCallback) main_menu_get_cell_height_callback,
    .draw_row = (MenuLayerDrawRowCallback) main_menu_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) main_menu_get_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) main_menu_select_callback,
  });
  menu_layer_set_click_config_onto_window(main_menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(main_menu_layer));
}

void main_menu_window_unload(Window *window) {
  menu_layer_destroy(main_menu_layer);
}

void main_menu_init() {
  main_menu_window = window_create();
  window_set_window_handlers(main_menu_window, (WindowHandlers) {
    .load = main_menu_window_load,
    .unload = main_menu_window_unload,
  });
}

void main_menu_show() {
  const bool animated = true;
  window_stack_push(main_menu_window, animated);
}

void main_menu_deinit() {
  window_destroy(main_menu_window);
}
