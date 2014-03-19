#ifndef _ENTRY_LIST
#define _ENTRY_LIST

#include <pebble.h>
#include "entry.h"
#include "util.h"
#include "comm.h"

typedef struct {
  Window *window;
  MenuLayer *menu_layer;
  Entry *entries;
  MenuLayerSelectCallback onclick;
  uint16_t entries_count;
  uint16_t entries_count_max;
  uint8_t parent_context;
  int32_t parent_id;
} EntryList;

EntryList *artists;
EntryList *albums;
EntryList *songs;

EntryList *entry_list_init();
void artist_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
void album_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
void song_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
void entry_list_show(EntryList *self, int8_t context, int8_t parent_context, int32_t parent_id);
void entry_list_deinit(EntryList *self);
bool entry_list_is_active(EntryList *self);
void entry_list_content_start(EntryList *self, uint16_t count);
void entry_list_content_add(EntryList *self, uint16_t index, uint32_t id, char* name);

#endif
