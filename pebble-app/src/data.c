#ifndef MSG_TYPE

#include <pebble.h>

#define MSG_TYPE 0
#define GET_ARTISTS 10
#define START_ARTISTS 11
#define SEND_ARTISTS 12
#define GET_ALBUMS 20
#define START_ALBUMS 21
#define SEND_ALBUMS 22
// 3x -- playlists
#define GET_ALBUM_SONGS 48
#define START_SONGS 41
#define SEND_SONGS 42
#define PLAY_ALBUM_SONG 43
#define COUNT 100
#define INDEX 101
#define ID 102
#define NAME 103

// http://blog.liw.fi/posts/strncpy/
#define fucking_copy_string(dst, src, len) if (snprintf((dst), (len), "%s", (src)) >= (len)) snprintf((dst), (len), "%s", "Overflow Error")
#define ENTRY_NAME_LEN 64

typedef struct {
  uint32_t id;
  char name[ENTRY_NAME_LEN];
} Entry;

static void request_data(uint8_t data_type, int8_t id) {
  if (bluetooth_connection_service_peek()) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet mtype = TupletInteger(MSG_TYPE, data_type);
    dict_write_tuplet(iter, &mtype);
    if (id > -1) {
      Tuplet mid = TupletInteger(ID, id);
      dict_write_tuplet(iter, &mid);
    }
    app_message_outbox_send();
  }
}

static void request_play(uint8_t data_type, int32_t parent_id, int8_t index) {
  if (bluetooth_connection_service_peek()) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet mtype = TupletInteger(MSG_TYPE, data_type);
    dict_write_tuplet(iter, &mtype);
    Tuplet mid = TupletInteger(ID, parent_id);
    dict_write_tuplet(iter, &mid);
    Tuplet mindex = TupletInteger(INDEX, index);
    dict_write_tuplet(iter, &mindex);
    app_message_outbox_send();
  }
}

#endif
