#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char *L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const char *R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

int ean8_calculate_checksum(const char *ean)
{
    int calc = 0;
    for (int i = 0; i < 7; i++)
    {
        int temp = ean[i] - '0';
        if ((i + 1) % 2 == 0)
            calc += temp;
        else
            calc += temp * 3;
    }
    int miss = (calc % 10) > 0 ? 10 - (calc % 10) : 0;
    return miss;
}

int ean8_is_valid(const char *ean)
{
    if (strlen(ean) == 8)
    {
        for (int i = 0; i < 8; i++)
        {
            if (!isdigit(ean[i]))
                return -1;
        }
        int miss = ean8_calculate_checksum(ean);
        if ((ean[7] - '0') == miss)
            return 1;
    }

    return 0;
}

char *ean8_encode(const char *ean)
{
    char *ean_code;
    ean_code = calloc(68, sizeof(char));
    if (ean_code != NULL)
    {

        strcpy(ean_code, "101");
        for (int i = 0; i < 8; i++)
        {
            if (i < 4)
                strcat(ean_code, L_CODE[ean[i] - '0']);
            else
            {
                if (i == 4)
                    strcat(ean_code, "01010");
                strcat(ean_code, R_CODE[ean[i] - '0']);
            }
        }
        strcat(ean_code, "101");
        return ean_code;
    }

    return NULL;
}