eng. de software 
Pong game em C
W-S 1p
Executavel esta no arquivo rar
#include <stdio.h>

long long comparacoes = 0, trocas = 0;

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    trocas++;
    *b = temp;
}

int particionar_primeiro(int arr[], int baixo, int alto) {
    int pivo = arr[baixo], i = baixo + 1, j = alto;
    while (i <= j) {
        while (i <= alto && arr[i] <= pivo) { i++; comparacoes++; }
        while (j >= baixo && arr[j] > pivo) { j--; comparacoes++; }
        if (i < j) trocar(&arr[i], &arr[j]);
    }
    trocar(&arr[baixo], &arr[j]);
    return j;
}

int particionar_ultimo(int arr[], int baixo, int alto) {
    int pivo = arr[alto], i = baixo - 1, j;
    for (j = baixo; j < alto; j++) {
        comparacoes++;
        if (arr[j] < pivo) {
            i++;
            trocar(&arr[i], &arr[j]);
        }
    }
    trocar(&arr[i + 1], &arr[alto]);
    return i + 1;
}

void quicksort(int arr[], int baixo, int alto, int tipo) {
    if (baixo < alto) {
        int p = (tipo == 1) ? particionar_primeiro(arr, baixo, alto)
                            : particionar_ultimo(arr, baixo, alto);
        quicksort(arr, baixo, p - 1, tipo);
        quicksort(arr, p + 1, alto, tipo);
    }
}

void testar(int tipo, int tamanho) {
    int arr[10000], i;  
    for (i = 0; i < tamanho; i++) arr[i] = i; // Entrada ordenada

    comparacoes = trocas = 0;
    quicksort(arr, 0, tamanho - 1, tipo);

    printf("Pivô %s -> Comparações: %lld | Trocas: %lld\n",
           (tipo == 1 ? "Primeiro" : "Último"), comparacoes, trocas);
}

int main() {
    int tamanho = 10000;

    testar(1, tamanho);
    testar(2, tamanho);

    return 0;
}