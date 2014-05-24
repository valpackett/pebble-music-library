#include <pebble.h>
#include "main_menu.h"
#include "entry_list.h"
#include "comm.h"

static void in_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message");
  Tuple *msg_type = dict_find(iter, MSG_TYPE);
  Tuple *msg_ctx = dict_find(iter, MSG_CTX);
  Tuple *count = dict_find(iter, COUNT);
  Tuple *index = dict_find(iter, INDEX);
  Tuple *id = dict_find(iter, ID);
  Tuple *name = dict_find(iter, NAME);

  if (msg_type && msg_ctx) {
    EntryList *elist = NULL;
    switch(msg_ctx->value->uint8) {
      case ARTISTS: elist = artists; break;
      case ALBUMS: elist = albums; break;
      case SONGS: elist = songs; break;
      case PLAYLISTS: elist = playlists; break;
    }
    if (elist && entry_list_is_active(elist)) {
      switch(msg_type->value->uint8) {
        case RSP_START:
          if (count)
            entry_list_content_start(elist, count->value->uint16);
          break;
        case RSP_DATA:
          if (index && id && name)
            entry_list_content_add(elist, index->value->uint16, id->value->uint32, name->value->cstring);
          break;
        default:
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid message: unsupported type");
          break;
      }
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid message: nonexistent or inactive context");
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid message: no type or context");
  }
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Dropped Message: %d", reason);
}

static void out_sent_handler(DictionaryIterator *sent, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Sent Message");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Not Sent Message: %d", reason);
}

static void init(void) {
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  app_message_open(app_message_inbox_size_maximum(), APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
  artists = entry_list_init(artist_select_callback);
  albums = entry_list_init(album_select_callback);
  songs = entry_list_init(song_select_callback);
  playlists = entry_list_init(playlist_select_callback);
  main_menu_init();
  main_menu_show();
}

static void deinit(void) {
  app_message_deregister_callbacks();
  entry_list_deinit(playlists);
  entry_list_deinit(songs);
  entry_list_deinit(albums);
  entry_list_deinit(artists);
  main_menu_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
