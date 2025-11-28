#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char *L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const char *R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

int ean8_calculate_checksum(const char *ean)
{
    int calc = 0;
    for (int i = 0; i < 7; i++) // percorre os 7 primeiros números
    {
        int temp = ean[i] - '0'; // transforma em int
        if ((i + 1) % 2 == 0)
            calc += temp;
        else
            calc += temp * 3;
    }
    int miss = (calc % 10) > 0 ? 10 - (calc % 10) : 0; // calcula quanto falta pro proximo multi de 10
    return miss;
}

int ean8_is_valid(const char *ean)
{
    if (strlen(ean) == 8) // verifica o tamanho
    {
        for (int i = 0; i < 8; i++)
        {
            if (!isdigit(ean[i])) // verifica se todos os caracteres são digitos
                return -1;
        }
        int miss = ean8_calculate_checksum(ean);
        if ((ean[7] - '0') == miss) // verifica se o digito bateu com o calculo
            return 1;
    }

    return 0;
}

char *ean8_encode(const char *ean)
{
    char *ean_code = calloc(68, sizeof(char)); // cria o vetor do codigo
    if (ean_code != NULL)
    {

        strcpy(ean_code, "101"); // adiciona o inicio
        for (int i = 0; i < 8; i++)
        {
            if (i < 4)
                strcat(ean_code, L_CODE[ean[i] - '0']); // os 4 primeiros digitos busca no L_CODE
            else
            {
                if (i == 4)
                    strcat(ean_code, "01010");          // adiciona o digito central
                strcat(ean_code, R_CODE[ean[i] - '0']); // os ultimos 4 digitos
            }
        }
        strcat(ean_code, "101"); // o digito final
        return ean_code;
    }

    return NULL; // se tiver algum erro na alocação de memoria
}

void create_image(const char *bits, const char *filename, int espacamento,
                  int pixels_por_area,
                  int altura)
{
    // variaveis de tamanho e largura
    int heigth = 2 * espacamento + altura, width = 2 * espacamento + pixels_por_area * strlen(bits);

    FILE *image = fopen(filename, "w"); // abre arquivo modo escrita
    // adiciona o cabeçalho
    fprintf(image, "P1\n");
    fprintf(image, "%d %d\n", width, heigth);

    // Margem superior
    for (int i = 0; i < espacamento; i++)
    {
        for (int k = 0; k < width; k++)
        {
            fprintf(image, "0");
        }
        fprintf(image, "\n");
    }

    // Code
    for (int i = 0; i < altura; i++)
    {
        // Margem a esquerda
        for (int k = 0; k < espacamento; k++)
        {
            fprintf(image, "0");
        }

        // Code central
        for (int k = 0; k < strlen(bits); k++)
        {
            for (int j = 0; j < pixels_por_area; j++)
            {
                fprintf(image, "%c", bits[k]);
            }
        }

        // Margem a direita
        for (int k = 0; k < espacamento; k++)
        {
            fprintf(image, "0");
        }
        fprintf(image, "\n");
    }
    // Margem inferior
    for (int i = 0; i < espacamento; i++)
    {
        for (int k = 0; k < width; k++)
        {
            fprintf(image, "0");
        }
        fprintf(image, "\n");
    }
    fclose(image);
}

void ean8_to_pbm(const char *bits, const char *filename, int espacamento,
                 int pixels_por_area,
                 int altura)
{
    FILE *image = fopen(filename, "r");
    if (image != NULL) // O arquivo existe
    {
        fclose(image);
        char sub;
        // Caso exista perguntar se que sobreescrever
        printf("Um arquivo existente foi encontrado, quer substituir?(s/n)");
        scanf(" %c", &sub);
        if (sub == 's' || sub == 'S')
        {
            create_image(bits, filename, espacamento, pixels_por_area, altura);
            printf("Outro arquivo criado!\n");
        }
        else
            printf("Arquivo mantindo!\n");
    }
    else // Não existe
        create_image(bits, filename, espacamento, pixels_por_area, altura);
}