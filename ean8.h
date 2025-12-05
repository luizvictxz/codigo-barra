#ifndef EAN8_H
#define EAN8_H

int ean8_is_valid(const char *ean);
char *ean8_encode(const char *ean);
void ean8_to_pbm(const char *bits, const char *filename, int espacamento, int pixels_por_area, int altura);
int ean8_extract_id(const char *filename, char *buffer_out);

#endif