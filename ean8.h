#ifndef EAN8_H
#define EAN8_H

int ean8_is_valid(const char *ean);
char *ean8_encode(const char *ean);

#endif