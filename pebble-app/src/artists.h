#ifndef ARTISTS
#define ARTISTS

void artists_init();
void artists_show();
void artists_deinit();
bool artists_is_active();
void artists_content_start(uint16_t count);
void artists_content_add(uint16_t index, uint32_t id, char* name);

#endif
