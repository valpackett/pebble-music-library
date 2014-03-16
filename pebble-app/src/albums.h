#ifndef ALBUMS
#define ALBUMS

void albums_init();
void albums_show(int8_t artist_id);
void albums_deinit();
bool albums_is_active();
void albums_content_start(uint16_t count);
void albums_content_add(uint16_t index, uint32_t id, char* name);

#endif
