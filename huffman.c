#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char data;
    unsigned frequencia;
    struct Node* left;
    struct Node* right;
};

struct Node* novoNo(char data, unsigned frequencia) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->frequencia = frequencia;
    temp->left = temp->right = NULL;
    return temp;
}

void trocarNos(struct Node** a, struct Node** b) {
    struct Node* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct Node** heap, int tamanhoHeap, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < tamanhoHeap && heap[esquerda]->frequencia < heap[menor]->frequencia)
        menor = esquerda;

    if (direita < tamanhoHeap && heap[direita]->frequencia < heap[menor]->frequencia)
        menor = direita;

    if (menor != indice) {
        trocarNos(&heap[indice], &heap[menor]);
        minHeapify(heap, tamanhoHeap, menor);
    }
}

int tamanhoHeapUm(struct Node** heap, int tamanhoHeap) {
    return (tamanhoHeap == 1);
}

struct Node* extrairMin(struct Node** heap, int* tamanhoHeap) {
    struct Node* min = heap[0];
    heap[0] = heap[(*tamanhoHeap) - 1];
    (*tamanhoHeap)--;
    minHeapify(heap, *tamanhoHeap, 0);
    return min;
}

void inserirHeap(struct Node** heap, int* tamanhoHeap, struct Node* no) {
    (*tamanhoHeap)++;
    int i = (*tamanhoHeap) - 1;
    while (i > 0 && no->frequencia < heap[(i - 1) / 2]->frequencia) {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i] = no;
}

void constroiHeap(struct Node** heap, int tamanhoHeap) {
    int indice = (tamanhoHeap - 1) / 2;
    for (int i = indice; i >= 0; --i)
        minHeapify(heap, tamanhoHeap, i);
}

void geraCodigo(struct Node* root, int code[], int top) {
    if (root->left) {
        code[top] = 0;
        geraCodigo(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = 1;
        geraCodigo(root->right, code, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i)
            printf("%d", code[i]);
        printf("\n");
    }
}

void imprimeArvore(struct Node* root) {
    int code[100];
    geraCodigo(root, code, 0);
}

void comprimirHuffman(char data[], int tamanho) {
    struct Node *left, *right, *top;

    struct Node** heap = (struct Node**)malloc(tamanho * sizeof(struct Node*));
    int tamanhoHeap = 0;

    for (int i = 0; i < tamanho; ++i) {
        int encontrado = 0;
        for (int j = 0; j < tamanhoHeap; ++j) {
            if (heap[j]->data == data[i]) {
                heap[j]->frequencia++;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            heap[tamanhoHeap] = novoNo(data[i], 1);
            tamanhoHeap++;
        }
    }

    constroiHeap(heap, tamanhoHeap);

    while (!tamanhoHeapUm(heap, tamanhoHeap)) {
        left = extrairMin(heap, &tamanhoHeap);
        right = extrairMin(heap, &tamanhoHeap);

        top = novoNo('$', left->frequencia + right->frequencia);
        top->left = left;
        top->right = right;

        inserirHeap(heap, &tamanhoHeap, top);
    }

    imprimeArvore(heap[0]);

    free(heap);
}
int main() {
    char input[100];

    printf("Digite uma palavra: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    int tamanho = strlen(input);

    comprimirHuffman(input, tamanho);

    return 0;
}