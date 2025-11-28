#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ean8.h"

// Será usada se não for executado conforme esperado
void usage()
{
    printf("Uso: ./programa <EAN-8> [espacamento] [pixels_por_area] [altura] [arquivo]\n");
    printf("  <EAN-8>              Código de 8 dígitos (obrigatório)\n");
    printf("  espacamento          Margem em pixels (padrão: 4)\n");
    printf("  pixels_por_area      Largura de cada barra (padrão: 3)\n");
    printf("  altura               Altura do código (padrão: 50)\n");
    printf("  arquivo              Nome do PBM de saída (padrão: saida.pbm)\n");
}

int main(int argc, char *argv[])
{
    char ean[9];
    char *codigo_pbm;
    int espaco = 4;
    int area = 3;
    int altura = 50;
    char arquivo[100] = "saida.pbm";
    if (argc < 2)
    {
        usage();
        exit(-1);
    }
    else
    {
        if (argc >= 3)
            espaco = atoi(argv[2]);
        if (argc >= 4)
            area = atoi(argv[3]);
        if (argc >= 5)
            altura = atoi(argv[4]);
        if (argc >= 6)
            strcpy(arquivo, argv[5]);
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
            ean8_to_pbm(bits, arquivo, espaco, area, altura);
            free(bits);
        }
    }
    return 0;
}