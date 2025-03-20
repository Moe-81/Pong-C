void quicksort(int arr[], int baixo, int alto, int tipo) {
    if (baixo < alto) {
        int p;
        if (tipo == 1)
            p = particionar_primeiro(arr, baixo, alto);
        else if (tipo == 2)
            p = particionar_ultimo(arr, baixo, alto);
        else
            p = particionar_aleatorio(arr, baixo, alto);  // Usando pivô aleatório
        quicksort(arr, baixo, p - 1, tipo);
        quicksort(arr, p + 1, alto, tipo);
    }
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

int particionar_aleatorio(int arr[], int baixo, int alto) {
    // Gerar um valor "aleatório" com base no tempo
    long tempo = clock();  // Obtém o tempo de execução do programa
    int pivo_index = baixo + (tempo % (alto - baixo + 1));  // Índice "aleatório"

    trocar(&arr[baixo], &arr[pivo_index]);  // Coloca o pivô no início

    int pivo = arr[baixo], i = baixo + 1, j = alto;
    while (i <= j) {
        while (i <= alto && arr[i] <= pivo) { i++; comparacoes++; }
        while (j >= baixo && arr[j] > pivo) { j--; comparacoes++; }
        if (i < j) trocar(&arr[i], &arr[j]);
    }
    trocar(&arr[baixo], &arr[j]);
    return j;
}

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    trocas++;
}