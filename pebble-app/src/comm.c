#include "comm.h"

void request_data(int8_t context, int8_t parent_context, int8_t id) {
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

void request_play(int8_t parent_context, int32_t parent_id, int8_t index) {
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
