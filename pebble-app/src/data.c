#ifndef MSG_TYPE

#include <pebble.h>

#define MSG_TYPE 0
enum mtype {
  REQ,
  RSP_START,
  RSP_DATA,
  RSP_END, // ununsed
  PLAY
};

#define MSG_CTX 1
#define MSG_PARENT_CTX 2
enum mcontext {
  ARTISTS,
  ALBUMS,
  PLAYLISTS,
  SONGS
};

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

static void request_data(int8_t context, int8_t parent_context, int8_t id) {
  if (bluetooth_connection_service_peek()) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet mmtype = TupletInteger(MSG_TYPE, REQ);
    dict_write_tuplet(iter, &mmtype);
    Tuplet mmctx = TupletInteger(MSG_CTX, context);
    dict_write_tuplet(iter, &mmctx);
    if (parent_context > -1) {
      Tuplet mmpctx = TupletInteger(MSG_PARENT_CTX, parent_context);
      dict_write_tuplet(iter, &mmpctx);
    }
    if (id > -1) {
      Tuplet mid = TupletInteger(ID, id);
      dict_write_tuplet(iter, &mid);
    }
    app_message_outbox_send();
  }
}

static void request_play(int8_t parent_context, int32_t parent_id, int8_t index) {
  if (bluetooth_connection_service_peek()) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet mmtype = TupletInteger(MSG_TYPE, PLAY);
    dict_write_tuplet(iter, &mmtype);
    Tuplet mmpctx = TupletInteger(MSG_PARENT_CTX, parent_context);
    dict_write_tuplet(iter, &mmpctx);
    Tuplet mid = TupletInteger(ID, parent_id);
    dict_write_tuplet(iter, &mid);
    Tuplet mindex = TupletInteger(INDEX, index);
    dict_write_tuplet(iter, &mindex);
    app_message_outbox_send();
  }
}

#endif
