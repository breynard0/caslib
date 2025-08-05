#ifndef DRAW_STUFF
#define DRAW_STUFF
enum PixelState { OFF, ON };
#endif

void clear_display(char *buffer, short length);
void set_pixel(short x, short y, short value, char *buffer, short width);
