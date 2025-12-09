#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char *L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const char *R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

static int ean8_calculate_checksum(const char *ean)
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
    for (int i = 0; i < 8; i++)
    {
        if (!isdigit(ean[i])) // verifica se todos os caracteres são digitos
            return -1;
    }
    int miss = ean8_calculate_checksum(ean);
    if ((ean[7] - '0') == miss) // verifica se o digito bateu com o calculo
        return 1;

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

void ean8_to_pbm(const char *bits, const char *filename, int espacamento, int pixels_por_area, int altura)
{
    int heigth = 2 * espacamento + altura;
    int width = 2 * espacamento + pixels_por_area * strlen(bits);

    FILE *image = fopen(filename, "w");
    if (image == NULL)
        return;

    fprintf(image, "P1\n");
    fprintf(image, "%d %d\n", width, heigth);

    // Margem superior
    for (int i = 0; i < espacamento; i++)
    {
        for (int k = 0; k < width; k++)
            fprintf(image, "0");
        fprintf(image, "\n");
    }

    // Código de Barras
    for (int i = 0; i < altura; i++)
    {
        // Margem esquerda
        for (int k = 0; k < espacamento; k++)
            fprintf(image, "0");

        // Barras
        for (int k = 0; k < strlen(bits); k++)
        {
            for (int j = 0; j < pixels_por_area; j++)
            {
                fprintf(image, "%c", bits[k]);
            }
        }

        // Margem direita
        for (int k = 0; k < espacamento; k++)
            fprintf(image, "0");
        fprintf(image, "\n");
    }

    // Margem inferior
    for (int i = 0; i < espacamento; i++)
    {
        for (int k = 0; k < width; k++)
            fprintf(image, "0");
        fprintf(image, "\n");
    }

    fclose(image);
}

// Identifica qual número corresponde ao padrão de bits
static int match_pattern(char *pattern, int is_left)
{
    for (int i = 0; i < 10; i++)
    {
        const char *code = is_left ? L_CODE[i] : R_CODE[i];
        if (strcmp(pattern, code) == 0)
            return i;
    }
    return -1;
}

static void exact_digit(const int is_left, const int **img, int pos, int linha, int erro_decode, int width, int L, char *buffer_out)
{
    for (int i = 0; i < 4; i++)
    {
        char pat[8];
        for (int b = 0; b < 7; b++)
        {
            int p_idx = pos + (b * L) + (L / 2);
            if (p_idx >= width)
            {
                erro_decode = 1;
                break;
            }
            pat[b] = img[linha][p_idx] ? '1' : '0';
        }
        pat[7] = '\0';
        int val = match_pattern(pat, 1); // 1 = Left
        if (val == -1)
            erro_decode = 1;
        else
        {
            if (is_left)
                buffer_out[i] = val + '0';
            else
                buffer_out[i + 4] = val + '0';
        }

        pos += 7 * L;
    }
}

static void free_memory(int **img, const int number)
{
    for (int i = 0; i < number; i++)
        free(img[i]);
    free(img);
}

// Nova Função de Extração
int ean8_extract_id(const char *filename, char *buffer_out)
{
    FILE *image = fopen(filename, "r");
    if (image == NULL)
        return -1; // Erro arquivo inexistente

    char format[3];
    int width, height;

    // Validação PBM
    if (fscanf(image, "%2s", format) != 1 || strcmp(format, "P1") != 0)
    {
        fclose(image);
        return -1;
    }
    if (fscanf(image, "%d %d", &width, &height) != 2)
    {
        fclose(image);
        return -1;
    }

    // Alocação e Leitura da Imagem
    int **img = (int **)malloc(height * sizeof(int *));
    int load_error = 0;
    for (int i = 0; i < height; i++)
    {
        img[i] = (int *)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++)
        {
            if (fscanf(image, "%1d", &img[i][j]) != 1)
                load_error = 1;
        }
    }
    fclose(image);

    if (load_error)
    {
        free_memory(img, height);
        return -1;
    }

    // Lógica de Scan
    int linha = height / 2;
    int col = 0;

    // Pula Quiet Zone
    while (col < width && img[linha][col] == 0)
        col++;
    if (col >= width)
    { // Só branco
        free_memory(img, height);
        return -2; // Código não encontrado
    }

    // Calcula Largura do Módulo
    int inicio_cod = col;
    int L = 0;
    while (col < width && img[linha][col] == 1)
    {
        L++;
        col++;
    }
    if (L == 0)
    { // Erro estranho
        free_memory(img, height);
        return -2;
    }

    int pos = inicio_cod + (3 * L); // Pula marcador inicial

    // Decodifica
    int erro_decode = 0;

    // dígitos da esquerda
    exact_digit(1, img, pos, linha, erro_decode, width, L, buffer_out);
    pos += 5 * L; // Pula centro

    // digitros direita
    exact_digit(0, img, pos, linha, erro_decode, width, L, buffer_out);
    buffer_out[8] = '\0'; // Null terminator

    // Limpeza
    free_memory(img, height);
    return erro_decode ? -2 : 0;
}