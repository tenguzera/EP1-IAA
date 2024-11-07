#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_FILENAME 256 /* tamanho máximo de um nome de arquivo*/

/*********************************************************************/
/**   ACH2002 - Introdução à Análise de Algoritmos                  **/
/**   EACH-USP - Segundo Semestre de 2023                           **/
/**   Prof.Dra. Ariane Machado Lima                                 **/
/**                                                                 **/
/**   Exercício de Programação 1: Recursão/Divisão e Conquista      **/
/**                                                                 **/
/**   <João Ricardo Paiva Camargo>            <14747274>            **/
/**                                                                 **/
/*********************************************************************/

typedef struct Node {
    bool val;
    bool isLeaf;
    struct Node* topLeft;
    struct Node* topRight;
    struct Node* bottomLeft;
    struct Node* bottomRight;
} Node;

Node* createNode(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, Node* _bottomLeft, Node* _bottomRight) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = _val;
    node->isLeaf = _isLeaf;
    node->topLeft = _topLeft;
    node->topRight = _topRight;
    node->bottomLeft = _bottomLeft;
    node->bottomRight = _bottomRight;
    return node;
}

Node* makeTree(int** grid, int posI, int posJ, int n) {
    Node* newNode = createNode(false, false, NULL, NULL, NULL, NULL);
    int startCellVal = grid[posI][posJ];
    for (int i = posI; i < posI + n; i++) {
        for (int j = posJ; j < posJ + n; j++) {
            if (grid[i][j] != startCellVal) {
                newNode->val = startCellVal;
                newNode->isLeaf = false;
                newNode->topLeft = makeTree(grid, posI, posJ, n / 2);
                newNode->topRight = makeTree(grid, posI, posJ + n / 2, n / 2);
                newNode->bottomLeft = makeTree(grid, posI + n / 2, posJ, n / 2);
                newNode->bottomRight = makeTree(grid, posI + n / 2, posJ + n / 2, n / 2);
                return newNode;
            }
        }
    }
    newNode->val = startCellVal;
    newNode->isLeaf = true;
    return newNode;
}

int** lerArquivo(char* filename, int tamMatriz){

    FILE* ler = fopen(filename, "r");
    if (ler == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
        return NULL;
    }

    int** matrizImg = (int**) malloc(sizeof(int*)*tamMatriz);
    for (int i=0; i<tamMatriz; i++){
        matrizImg[i] = (int*)malloc(sizeof(int)*tamMatriz);
    }

    char buffer;
    for (int i = 0; i < tamMatriz; i++) {
        for (int j = 0; j < tamMatriz+1; j++) {
            buffer = fgetc(ler);
            if(buffer != 10)
                matrizImg[i][j] = buffer - 48;
        }
    }

    fclose(ler);

    return matrizImg;
}

void verificarCor(Node* QT, int **matriz, int size, int nivel){

    if(QT->isLeaf){
        if(QT->val) matriz[nivel][1]++;
            else matriz[nivel][2]++;
    }
        else {
        nivel++;
        verificarCor(QT->topLeft, matriz, size / 2, nivel);
        verificarCor(QT->topRight, matriz, size / 2, nivel);
        verificarCor(QT->bottomLeft, matriz, size / 2, nivel);
        verificarCor(QT->bottomRight, matriz, size / 2, nivel);
    }

}

int saida(Node* QT, int tamMatriz){

    //Inicializar matriz para guardar as cores dos nós
    int numNos = log2(tamMatriz) + 1;
    int **resultado = (int**) malloc(sizeof(int*)*numNos);
    for(int i=0; i <= numNos; i++){
        resultado[i] = (int*) malloc(sizeof(int)*3);
    }
    
    int proxysize = tamMatriz;
    for (int i = 0; proxysize >= 1 ; i++) {
        resultado[i][0] = proxysize;
        resultado[i][1] = 0;
        resultado[i][2] = 0;
        proxysize = proxysize / 2;
    }

    //Contabilizar os nós e armazenar as cores na matriz "resultado"    
    verificarCor(QT, resultado, tamMatriz, 0);

    //Imprimir resultado
    FILE* out = fopen("saida.txt","w");
    for (int i = 0; i < numNos; i++) {
            int j = 0;
            fprintf(out, "%ix%i %i %i\n", resultado[i][j], resultado[i][j], 
                                          resultado[i][j+1], resultado[i][j+2]);
    }

    fclose(out);

}

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Este programa exige dois argumentos: tamanho da matriz e nome do arquivo.\n");
        exit(1);
    }

    FILE* fp = fopen(argv[2], "r"); /* abre arquivo em modo somente leitura */ 

    char *tamMatrizChar;
    strcpy(tamMatrizChar, argv[1]);
    int tamMatriz = atoi(tamMatrizChar);

    //Guardar o arquivo em um array
    int** matrizImg = lerArquivo(argv[2], tamMatriz);

    //Construir QuadTree a partir do array
    Node* treeInicio = makeTree(matrizImg, 0, 0, tamMatriz);
    
    //Saída do programa
    saida(treeInicio, tamMatriz);

}