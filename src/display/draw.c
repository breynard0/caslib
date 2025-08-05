void clear_display(char *buffer, short length) {
  for (int i = 0; i < length; i++) {
    buffer[i] = 0;
  }
}

void set_pixel(short x, short y, short value, char *buffer, short width) {
  buffer[y * width + x] = value;
}
