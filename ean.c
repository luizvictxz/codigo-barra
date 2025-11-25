#include <stdio.h>
#include <string.h>

int missed_to_multi(char *value)
{
    int calc = 0;
    if (strlen(value) == 8)
    {

        for (int i = 0; i < 7; i++)
        {
            int temp = value[i] - '0';
            if ((i + 1) % 2 == 0)
                calc += temp;
            else
                calc += temp * 3;
        }
        int miss = (calc % 10) > 0 ? 10 - (calc % 10) : 0;
        if ((value[7] - '0') == miss)
            return miss;
    }

    return 0;
}

int main()
{
    char ean[9]; // armazena o codigo completo
    scanf("%s", ean);

    return 0;
}