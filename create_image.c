#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ean8.h"

// Será usada se não for executado conforme esperado
void usage()
{
    printf("\n=== Gerador de Código de Barras EAN-8 (PBM) ===\n\n");
    printf("Uso:\n");
    printf("  programa <EAN8> [espacamento] [pixels_por_area] [altura] [arquivo.pbm]\n\n");

    printf("Onde:\n");
    printf("  <EAN8>              Código numérico de 8 dígitos (obrigatório)\n");
    printf("  espacamento         Espaço lateral e superior/inferior em pixels (opcional)\n");
    printf("                      Padrão: 4\n");
    printf("  pixels_por_area     Número de pixels usados para desenhar cada barra (opcional)\n");
    printf("                      Padrão: 3\n");
    printf("  altura              Altura do código de barras em pixels (opcional)\n");
    printf("                      Padrão: 50\n");
    printf("  arquivo.pbm         Nome do arquivo de saída (opcional)\n");
    printf("                      Padrão: saida.pbm\n\n");

    printf("Exemplos:\n");
    printf("  programa 40170525\n");
    printf("      → Gera saida.pbm com os valores padrões.\n\n");

    printf("  programa 40170525 6 4 60\n");
    printf("      → Gera saida.pbm com espaçamento=6, área=4, altura=60.\n\n");

    printf("  programa 40170525 6 4 60 imagem.pbm\n");
    printf("      → Gera imagem.pbm com as configurações acima.\n\n");

    printf("Observação importante:\n");
    printf("  Todos os argumentos opcionais devem ser fornecidos na ordem correta.\n");
    printf("  Ou seja, NÃO use:\n");
    printf("     programa 40170525 imagem.pbm   (ordem inválida!)\n");
    printf("  Se quiser mudar apenas o nome da imagem, informe os valores anteriores:\n");
    printf("     programa 40170525 4 3 50 imagem.pbm\n\n");
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
    if (argc >= 3)
        espaco = atoi(argv[2]);
    if (argc >= 4)
        area = atoi(argv[3]);
    if (argc >= 5)
        altura = atoi(argv[4]);
    if (argc >= 6)
        strcpy(arquivo, argv[5]);
    if (strlen(argv[1]) != 8)
    {
        fprintf(stderr, "Erro: o código EAN-8 deve ter exatamente 8 dígitos.\n");
        return 1;
    }
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

    return 0;
}