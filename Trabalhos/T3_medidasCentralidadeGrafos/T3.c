#include <stdio.h>
#include <stdlib.h>
#define INF 999999999

typedef struct tipoItem{
    int id;
    int qtd;
    struct tipoItem *prox;
}tipoItem;

typedef struct tipoLista{
    int tam;
    tipoItem *lista;
}tipoLista;

typedef struct tipoPilha{
    tipoItem *topo;
}tipoPilha;

typedef struct tipoVertice{
    int vertID;
    int antecInd;
    int e;

    float grau;

    float closeness;
    float somaCamMin;
    float vOndeChega;

    float betweenness;

    float clustering;
    float iTriangulos;

    struct tipoVertice *prox;
}tipoVertice;

typedef struct tipoGrafo{
    tipoVertice *adj;
    int tam;
    int qtdVertices;
    float totalTriangulos;
    int qtdArestas;
    float *betweenness;
}tipoGrafo;

typedef struct tipoFila{
    tipoVertice *inicio, *fim;
}tipoFila;

void inicializagrafo(tipoGrafo *grafo, int N){ //ta ok
    int i;
    grafo->betweenness = malloc(N*sizeof(int));
    grafo->adj = malloc(N*sizeof(tipoVertice));
    for(i=0; i<N; i++){
        grafo->adj[i].vertID = i;
        grafo->adj[i].prox = NULL;
        grafo->adj[i].grau = 0;
        grafo->adj[i].e =-1;
        grafo->adj[i].betweenness = 0;
        grafo->betweenness[i] = 0;
    }
    grafo->tam = N;
    grafo->qtdVertices = 0;
    grafo->totalTriangulos = 0;
    grafo->qtdArestas = 0;
}


void criaHeapBetweenness(tipoGrafo *grafo, int i, int f){
    tipoVertice aux = grafo->adj[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(grafo->adj[j].betweenness > grafo->adj[j+1].betweenness) j = j+1;
        }
        if(aux.betweenness > grafo->adj[j].betweenness){
            grafo->adj[i] = grafo->adj[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    grafo->adj[i] = aux;
}
void heapSortBetweenness(tipoGrafo *grafo, int N){
    int i;
    tipoVertice aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapBetweenness(grafo, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = grafo->adj[0];
        grafo->adj[0] = grafo->adj[i];
        grafo->adj[i] = aux;
        criaHeapBetweenness(grafo, 0, i-1); // reconstroi heap
    }// pega maior elemento e coloca na posição certa do vetor
}


void criaHeapCloseness(tipoGrafo *grafo, int i, int f){
    tipoVertice aux = grafo->adj[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(grafo->adj[j].closeness > grafo->adj[j+1].closeness) j = j+1;
        }
        if(aux.closeness > grafo->adj[j].closeness){
            grafo->adj[i] = grafo->adj[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    grafo->adj[i] = aux;
}
void heapSortCloseness(tipoGrafo *grafo, int N){
    int i;
    tipoVertice aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapCloseness(grafo, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = grafo->adj[0];
        grafo->adj[0] = grafo->adj[i];
        grafo->adj[i] = aux;
        criaHeapCloseness(grafo, 0, i-1); // reconstroi heap
    }// pega maior elemento e coloca na posição certa do vetor
}

void criaHeapClustering(tipoGrafo *grafo, int i, int f){
    tipoVertice aux = grafo->adj[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(grafo->adj[j].clustering > grafo->adj[j+1].clustering) j = j+1;
        }
        if(aux.clustering > grafo->adj[j].clustering){
            grafo->adj[i] = grafo->adj[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    grafo->adj[i] = aux;
}
void heapSortClustering(tipoGrafo *grafo, int N){
    int i;
    tipoVertice aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapClustering(grafo, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = grafo->adj[0];
        grafo->adj[0] = grafo->adj[i];
        grafo->adj[i] = aux;
        criaHeapClustering(grafo, 0, i-1);
    }// pega maior elemento e coloca na posição certa do vetor
}

void criaHeapGrau(tipoGrafo *grafo, int i, int f){
    tipoVertice aux = grafo->adj[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(grafo->adj[j].grau > grafo->adj[j+1].grau) j = j+1;
        }
        if(aux.grau > grafo->adj[j].grau){
            grafo->adj[i] = grafo->adj[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    grafo->adj[i] = aux;
}
void heapSortGrau(tipoGrafo *grafo, int N){
    int i;
    tipoVertice aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapGrau(grafo, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = grafo->adj[0];
        grafo->adj[0] = grafo->adj[i];
        grafo->adj[i] = aux;
        criaHeapGrau(grafo, 0, i-1); // reconstroi heap
    }// pega maior elemento e coloca na posição certa do vetor
}

void insereAresta(tipoGrafo *grafo, int v1, int v2){
    tipoVertice *aux = malloc(sizeof(tipoVertice));
    aux->vertID = v2;
    aux->antecInd = v1;
    aux->prox = NULL;

    if(grafo->adj[v1].e = -1) grafo->adj[v1].e = 1;
    if(grafo->adj[v2].e = -1) grafo->adj[v2].e = 1;

    if(grafo->adj[v1].prox == NULL)
        grafo->adj[v1].prox = aux;
    else{
        if(aux->vertID < grafo->adj[v1].prox->vertID){
            aux->prox = grafo->adj[v1].prox;
            grafo->adj[v1].prox = aux;
        }
        else{
            tipoVertice *percorre = grafo->adj[v1].prox;
            while (percorre->prox != NULL && percorre->prox->vertID < aux->vertID){
            percorre = percorre->prox;
            }
            aux->prox = percorre->prox;
            percorre->prox = aux;
        }

    }
    grafo->adj[v1].grau++;
    grafo->qtdArestas++;
}

void imprimeGrafo(tipoGrafo *grafo){
    int i;
    FILE *arqGrafo = fopen("Grafo.txt", "w");

    fprintf(arqGrafo, "Grafo referente à votação de administradores da Wikipedia até Janeiro de 2018\n\n");

    tipoVertice *percorre;
    for(i=0; i<grafo->tam; i++){
        if(grafo->adj[i].e){
            fprintf(arqGrafo, "\nUsuário %d votou em %.f pessoa(s): ", grafo->adj[i].vertID, grafo->adj[i].grau);
            percorre = grafo->adj[i].prox;
            while(percorre != NULL){
                fprintf(arqGrafo, "%d ", percorre->vertID);
                percorre = percorre->prox;
            }
            fprintf(arqGrafo, "\n");
        }
    }
}

void leArquivoGrafo(tipoGrafo *grafo, char *arquivo){
    FILE *arq = fopen(arquivo, "r");

    int aux, auxID1, auxID2;
    char c;

    do{ fscanf(arq, "%c", &c);
    }while (c != '\n'); // Pula a primeira linha
    do{ fscanf(arq, "%c", &c);
    }while (c != '\n'); // Pula a segunda linha
    do{ fscanf(arq, "%c", &c);
    }while (c != '\n'); // Pula a terceira linha
    do{ fscanf(arq, "%c", &c);
    }while (c != '\n'); // Pula a quarta linha
    do{ fscanf(arq, "%c", &c);
    }while (c != '\n'); // Pula a quinta linha
    //printf("pulou\n");*/

    while(fscanf(arq, "%d", &aux) != EOF){
        auxID1 = aux;
        fscanf(arq, "%d", &aux);
        auxID2 = aux;
        insereAresta(grafo, auxID1, auxID2);
    }

    printf("Lendo arquivo e inserindo no grafo...\n");

    //corrigeIndices(grafo);
}

void inicializaFila(tipoFila *fila){
    fila->inicio = NULL;
    fila->fim = NULL;
}

void insereNaFila(tipoFila *fila, int vertID){
    tipoVertice *aux = malloc(sizeof(tipoVertice));
    aux->vertID = vertID;
    aux->prox = NULL;

    if(fila->inicio == NULL) fila->inicio = aux;
    else fila->fim->prox = aux;
    fila->fim = aux;
}

tipoVertice removeDaFila(tipoFila *fila){
    tipoVertice retorno = (*fila->inicio);
    tipoVertice *r = fila->inicio;
    fila->inicio = fila->inicio->prox;
    free(r);
    return retorno;
}

void BuscaCaminhoMinimo(tipoGrafo *grafo, int inicio){
    if(grafo->adj[inicio].prox == NULL) return;

	int *antecessor = malloc(grafo->tam*sizeof(int));
	float *dist = malloc(grafo->tam*sizeof(float));

  	int i;
 	for(i=0; i<grafo->tam; i++){
		antecessor[i] = -1;
		dist[i] = INF;
	}

	tipoFila fila;
	inicializaFila(&fila);

	dist[inicio] = 0;
	antecessor[inicio] = inicio;

	insereNaFila(&fila, inicio);
	tipoVertice removido;

	while(fila.inicio != NULL){
		removido = removeDaFila(&fila);
		tipoVertice *percorre = grafo->adj[removido.vertID].prox;
		while(percorre != NULL){
			if(antecessor[percorre->vertID] == -1){
				antecessor[percorre->vertID] = removido.vertID;
				dist[percorre->vertID] = dist[removido.vertID]+1.0;
				insereNaFila(&fila, percorre->vertID);
			}
			percorre = percorre->prox;
		}

	}
	float total=0;
	float qtdV = 0; //vertices nos quais ele consegue chegar
	for(i=0; i<grafo->tam; i++){
		if(dist[i] != INF){
			total += dist[i];
			qtdV++;
        }
	}
	grafo->adj[inicio].somaCamMin = total;
	grafo->adj[inicio].vOndeChega = qtdV;

	float closeness = total/qtdV;
    grafo->adj[inicio].closeness = closeness;

}

void inicializaPilha(tipoPilha *pilha){
    pilha->topo = NULL;
}

void insereNaPilha(tipoPilha *pilha, int dado){
    tipoItem *aux = malloc(sizeof(tipoPilha));
    aux->id = dado;
    aux->prox = pilha->topo;
    pilha->topo = aux;
}

int desempilha(tipoPilha *pilha){
    int retorna = pilha->topo->id;
    tipoItem *aux = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(aux);
    return retorna;
}

void inicializaLista(tipoLista *parents, tipoGrafo *grafo){
    parents->lista = malloc(grafo->tam*sizeof(tipoItem));
    int i;
    for(i=0; i<grafo->tam; i++){
        parents->lista[i].prox = NULL;
        parents->lista[i].id = i;
    }
    parents->tam = grafo->tam;
}

void insereNaLista(tipoLista *parents, int filho, int pai){
    tipoItem *aux = malloc(sizeof(tipoItem));
    aux->id = pai;
    aux->prox = NULL;

    if(parents->lista[filho].prox == NULL){
        parents->lista[filho].prox = aux;
        parents->lista[filho].qtd = 1;
    }

    else if(parents->lista[filho].prox != NULL){
        aux->prox = parents->lista[filho].prox;
        parents->lista[filho].prox = aux;
        parents->lista[filho].qtd++;
    }

    parents->tam++;
}

int retiraLista(tipoLista *parent, int filho){
    if(parent->lista[filho].prox != NULL){
        int pai = parent->lista[filho].prox->id;
        parent->lista[filho].prox = parent->lista[filho].prox->prox;
        parent->tam--;
        return pai;
    }
    else return -1;
}

void betweenness(tipoGrafo *grafo, int inicio){
    if(grafo->adj[inicio].prox == NULL) return;
	int i, j;

	for(i=0; i<grafo->tam; i++){
        tipoLista parents;
        inicializaLista(&parents, grafo);

		tipoPilha pilha;
		inicializaPilha(&pilha);

		float *qtdCamMin = malloc(grafo->tam*sizeof(float));
		int *dist = malloc(grafo->tam*sizeof(int));
		for(j=0; j<grafo->tam; j++){
			qtdCamMin[j] = 0.0;
			dist[j] = -1;
		}
		qtdCamMin[inicio] = 1;
		dist[inicio] = 0;

        tipoFila fila;
		inicializaFila(&fila);

		tipoVertice removido;

		insereNaFila(&fila, inicio);
		while(fila.inicio != NULL){
			removido = removeDaFila(&fila);
			insereNaPilha(&pilha, removido.vertID);
			tipoVertice *percorre = grafo->adj[removido.vertID].prox;
			while(percorre != NULL){
				if(dist[percorre->vertID] == -1){
					insereNaFila(&fila, percorre->vertID);
					dist[percorre->vertID] = dist[removido.vertID]+1;
				}
				if(dist[percorre->vertID] == dist[removido.vertID]+1){
					qtdCamMin[percorre->vertID] += qtdCamMin[removido.vertID];
					insereNaLista(&parents, percorre->vertID, removido.vertID);
				}
				percorre = percorre->prox;
			}
		}

        float total=0;
        float qtdV = 0; //vertices nos quais ele consegue chegar
        for(i=0; i<grafo->tam; i++){
            if(dist[i] != INF){
                total += dist[i];
                qtdV++;
            }
        }
        grafo->adj[inicio].somaCamMin = total;
        grafo->adj[inicio].vOndeChega = qtdV;

        float closeness = total/qtdV;
        grafo->adj[inicio].closeness = closeness;


		float *sigma = malloc(grafo->tam*sizeof(float));
		for(j=0; j<grafo->tam; j++) sigma[j] = 0.0;

		while(pilha.topo != NULL){
            int w = desempilha(&pilha);
            for(j=0; j<parents.lista[w].qtd; j++){
                int pai = retiraLista(&parents, w);
                sigma[pai] = sigma[pai] + (qtdCamMin[pai]/qtdCamMin[w])*(1.0+sigma[w]);
            }
            if(w != inicio){
                grafo->adj[w].betweenness += sigma[w];
            }
		}
		//for(j=0; j<grafo->tam; j++) printf("%.2f ", sigma[j]);
	}
}

void clustering(tipoGrafo *grafo){
    printf("Calculando Clustering...\n");

    int i;
    tipoVertice *p1, *p2, *p3;

    float itotalClust;
    for(i=0; i<grafo->tam; i++){
        itotalClust = 0.0;
        p1 = grafo->adj[i].prox;
        while(p1 != NULL){
            p2 = grafo->adj[p1->vertID].prox;
            while(p2!=NULL){
                /*if(p2->vertID == i) p2 = p2->prox;
                if(p2 == NULL) break;*/
                p3 = grafo->adj[p2->vertID].prox;
                while(p3 != NULL /*&& p3->vertInd <= i*/){
                    if(p3->vertID == i) itotalClust++;
                    p3 = p3->prox;
                }
                p2 = p2->prox;
            }
            p1 = p1->prox;
        }
        grafo->totalTriangulos += itotalClust;
        //printf("%d (i) %.f\n", i, itotalClust);
        if (grafo->adj[i].grau > 1){ // se vertice i esta conectado a outro
            grafo->adj[i].clustering = (2*itotalClust)/(grafo->adj[i].grau*(grafo->adj[i].grau-1));
            grafo->adj[i].iTriangulos = itotalClust;
        }

        else
            grafo->adj[i].clustering = 0.0;
    }
}

int main(){

    printf(":::TRABALHO DE AEDII\n:::ICT UNIFESP\n:::Renata Sendreti Broder\n\n\nCálculo de medidas de centralidade em grafos\nWiki-Vote: quem votou em quem\n\n");
    tipoGrafo grafo;
    inicializagrafo(&grafo, 5001);
    leArquivoGrafo(&grafo, "Wiki-Vote.txt");
    int i;

    for(i=0; i<grafo.tam; i++){
        if(grafo.adj[i].e = 1) grafo.qtdVertices++;
    }

    imprimeGrafo(&grafo);

    for(i=0; i<grafo.tam; i++) betweenness(&grafo, i); //funciona para arquivos pequenos, mas com este ta travando

    heapSortBetweenness(&grafo, grafo.tam);
    FILE *arqBetweenness = fopen("Betweenness.txt", "w");
    fprintf(arqBetweenness, "Cálculo de medidas: Betweenness\nQuantidade de vértices: %d\nQuantidade de arestas: %d\n\nVérticeID\tBetweenness\n", grafo.qtdVertices, grafo.qtdArestas);
    for(i=0; i<grafo.tam; i++){
        if(grafo.adj[i].e) fprintf(arqBetweenness, " %d\t\t%.2f\n", grafo.adj[i].vertID, grafo.adj[i].betweenness);
    }
    close(arqBetweenness);

    /*printf("Calculando Closeness...\n");
    for(i=0; i<grafo.tam; i++) BuscaCaminhoMinimo(&grafo, i); //calcula closeness*/

    clustering(&grafo); //calcula clustering
    printf("Gravando dados...\n");

    heapSortClustering(&grafo, grafo.tam);
    FILE *arqClustering = fopen("Clustering.txt", "w"); //Gera arquivo medidas de clustering
    fprintf(arqClustering, "Cálculo de medidas: Clustering\nQuantidade de vértices: %d\nQuantidade de arestas: %d\nQuantidade total de triângulos: %.f\n\nVérticeID\tClustering\tTriangulos-Vertice\n", grafo.qtdVertices, grafo.qtdArestas, grafo.totalTriangulos);
    for(i=0; i<grafo.tam; i++){
        if(grafo.adj[i].e)
            fprintf(arqClustering, " %d\t\t%.2f\t\t%.f\n", grafo.adj[i].vertID, grafo.adj[i].clustering, grafo.adj[i].iTriangulos);
    }
    fclose(arqClustering);

    heapSortGrau(&grafo, grafo.tam);
    FILE *arqGrau = fopen("Grau.txt", "w"); //Gera arquivo medidas de clustering
    fprintf(arqGrau, "Cálculo de medidas: Grau\nQuantidade de vértices: %d\nQuantidade de arestas: %d\n\nVérticeID\tGrau\n", grafo.tam, grafo.qtdArestas);
    for(i=0; i<grafo.tam; i++){
        if(grafo.adj[i].e)
            fprintf(arqGrau, " %d\t\t%.f\n", grafo.adj[i].vertID, grafo.adj[i].grau);
    }
    fclose(arqGrau);

    heapSortCloseness(&grafo, grafo.tam);
    FILE *arqCloseness = fopen("Closeness.txt", "w"); //Gera arquivo medidas de clustering
    fprintf(arqCloseness, "Cálculo de medidas: Closeness\nQuantidade de vértices: %d\nQuantidade de arestas: %d\n\nVérticeID\tCloseness\tSoma CamMin\t\tVertices onde chega\n", grafo.qtdVertices, grafo.qtdArestas);
    for(i=0; i<grafo.tam; i++){
        if(grafo.adj[i].e)
            fprintf(arqCloseness, " %d\t\t%.2f\t\t%.2f\t\t%.f\n", grafo.adj[i].vertID, grafo.adj[i].closeness, grafo.adj[i].somaCamMin, grafo.adj[i].vOndeChega);
    }
    fclose(arqCloseness);

    return 0;
}
