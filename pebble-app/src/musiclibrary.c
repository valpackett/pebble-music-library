#include <pebble.h>
#include "main_menu.h"
#include "artists.h"
#include "albums.h"
#include "songs.h"
#include "data.c"

static void in_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message");
  Tuple *msg_type = dict_find(iter, MSG_TYPE);
  Tuple *count = dict_find(iter, COUNT);
  Tuple *index = dict_find(iter, INDEX);
  Tuple *id = dict_find(iter, ID);
  Tuple *name = dict_find(iter, NAME);
  if (msg_type) switch(msg_type->value->uint8) {
    case START_ARTISTS:
      if (artists_is_active() && count) artists_content_start(count->value->uint16);
      break;
    case SEND_ARTISTS:
      if (artists_is_active() && index && id && name) artists_content_add(index->value->uint16, id->value->uint32, name->value->cstring);
      break;
    case START_ALBUMS:
      if (albums_is_active() && count) albums_content_start(count->value->uint16);
      break;
    case SEND_ALBUMS:
      if (albums_is_active() && index && id && name) albums_content_add(index->value->uint16, id->value->uint32, name->value->cstring);
      break;
    case START_SONGS:
      if (songs_is_active() && count) songs_content_start(count->value->uint16);
      break;
    case SEND_SONGS:
      if (songs_is_active() && index && id && name) songs_content_add(index->value->uint16, id->value->uint32, name->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Message has unknown type %d", msg_type->value->uint8);
      break;
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Message has no type");
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
  artists_init();
  albums_init();
  songs_init();
  main_menu_init();
  main_menu_show();
}

static void deinit(void) {
  app_message_deregister_callbacks();
  albums_deinit();
  artists_deinit();
  songs_deinit();
  main_menu_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
