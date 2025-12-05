#include <stdio.h>
#include <stdlib.h>
#include "ean8.h"

int main(int argc, char *argv[])
{
    // Validação de argumento obrigatório
    if (argc < 2)
    {
        printf("Arquivo inválido (Argumento ausente).\n");
        exit(1);
    }

    char identificador[9]; // 8 dígitos + null
    int resultado = ean8_extract_id(argv[1], identificador);

    // Validar arquivo inexistente ou inválido
    if (resultado == -1)
    {
        printf("Arquivo inválido!\n");
        return 1;
    }
    // Validar se código foi encontrado
    if (resultado == -2)
    {
        printf("Código não encontrado.\n");
        return 0; // O enunciado não especifica return code para erro lógico, mas 0 é safe
    }
    // Sucesso -> Saída padrão

    printf("%s\n", identificador);

    return 0;
}