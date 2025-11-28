#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ean8.h"

// Será usada se não for executado conforme esperado
void usage()
{
    printf("Instruções do programa:\n");
}

int main(int argc, char *argv[])
{
    char ean[9];
    char *codigo_pbm;
    if (argc < 2)
    {
        usage();
        exit(-1);
    }
    else
    {
        if (strlen(argv[1]) == 8)
        {
            strcpy(ean, argv[1]);
            int ok = ean8_is_valid(ean);
            if (ok == -1)
            {
                fprintf(stderr, "Erro: O código EAN contém caracteres não numéricos.\n");
                return 1;
            }
            if (ok == 0)
            {
                fprintf(stderr, "Erro: Código EAN inválido (último dígito incorreto).\n");
                return 1;
            }
            char *bits = ean8_encode(ean);
            if (bits == NULL)
            {
                fprintf(stderr, "Erro: Falha ao alocar memória para codificação.\n");
                return 1;
            }
            ean8_to_pbm(bits, "saida.pbm");
            free(bits);
        }
    }
    return 0;
}