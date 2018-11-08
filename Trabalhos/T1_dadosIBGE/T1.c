#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tipoFrota{ //Armazena dados após leitura
    char tipoVeiculo[20];
    char UF[3];
    int qtdV12;
    int qtdV13;
    int qtdV14;
    int qtdV15;
}tipoFrota;

typedef struct tipoCategoria{ //Armazenar dados após agrupar por categoria de veiculos
    char categoria[20];
    char UF[3];
    int totalCateg;
}tipoCategoria;

typedef struct tipoEstado{ //Armazena dados após agrupar por UF e por categoria
    char UF[3];
    tipoCategoria *categorias;
    int totVeiculosUF;
}tipoEstado;

typedef struct tipoMaiorEstado{ //Armazena dados referentes à categoria de veiculos com maior quantidade por UF
    char UF[3];
    char Categoria[20];
    int maior;
}tipoMaiorEstado;

typedef struct tipoAno{ //Armazena dados relativos às quantidades de veiculos por ano
    int ano;
    int totalCategAno;
}tipoAno;

tipoFrota *coletarDadosPlanilha(FILE * arquivo, int *qtdLinhas){
    int i;
    char c;
    char aux[50];

    tipoFrota *dados = (tipoFrota*) malloc(sizeof(tipoFrota)); //Aloca memória para armazenar dados lidos
    *qtdLinhas = 0;

    do{ fscanf(arquivo, "%c", &c);
    }while (c != '\n'); // Pula a primeira linha

    while(fscanf(arquivo, "%c", &c) != EOF){
        i = 0;
        while(c != ','){//Armazena dados da coluna Categoria
            aux[i] = c;
            i++;
            fscanf(arquivo, "%c", &c);
        }
        aux[i] = '\0';
        strcpy(dados[*qtdLinhas].tipoVeiculo, aux);

        i = 0;
        fscanf(arquivo, "%c", &c);
        while(c != ','){//Armazena dados da coluna UF
            aux[i] = c;
            i++;
            fscanf(arquivo, "%c", &c);
        }
        aux[i] = '\0';
        strcpy(dados[*qtdLinhas].UF, aux);

        i = 0;
        fscanf(arquivo, "%c", &c);
        while(c != '\n'){
            aux[i] = c;
            i++;
            fscanf(arquivo, "%c", &c);
        }
        sscanf(aux, "%d,%d,%d,%d,%*d", &dados[*qtdLinhas].qtdV12, &dados[*qtdLinhas].qtdV13, &dados[*qtdLinhas].qtdV14,&dados[*qtdLinhas].qtdV15);

        *qtdLinhas += 1;
        dados = (tipoFrota*) realloc(dados, (*qtdLinhas+1) * sizeof(tipoFrota));
    }

    return dados;
}

void criaHeapUF(tipoFrota *dados, int i, int f){
    tipoFrota aux = dados[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(strcmp(dados[j].UF, dados[j+1].UF) < 0) j = j+1;
        }
        if(strcmp(aux.UF, dados[j].UF) < 0){
            dados[i] = dados[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    dados[i] = aux;
}
void heapSortUF(tipoFrota *dados, int N){
    int i;
    tipoFrota aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapUF(dados, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = dados[0];
        dados[0] = dados[i];
        dados[i] = aux;
        criaHeapUF(dados, 0, i-1); // reconstroi heap
    }// pega maior elemento e coloca na posição certa do vetor
}
void insertionSortUF(tipoFrota *dados, int N){
    int i, j;
    tipoFrota aux;

    for(i=1; i<N; i++){
        j=(i-1);
        while(j>=0 && strcmp(dados[j+1].UF, dados[j].UF) < 0){
            aux = dados[j+1];
            dados[j+1] = dados[j];
            dados[j] = aux;
            j--;
        }
    }
}

tipoEstado *agrupaUF(tipoFrota *dados, int *qtdEstados, int qtdLinhas){
    int i = 0;
    char UF[3];
    int auxVeiculosUF;
    *qtdEstados = 0;

    tipoEstado *estados = (tipoEstado*) malloc(sizeof(tipoEstado)); //Aloca memoria para agrupar estados
    int j = 12; //Quantidade de categorias de acordo com a planilha

    while(i<qtdLinhas){
        estados[*qtdEstados].categorias = (tipoCategoria*) malloc(j*sizeof(tipoCategoria)); //Aloca memoria para agrupar tipos dentro da struct de estados
        strcpy(estados[*qtdEstados].categorias[0].categoria, "Automovel"); //Inicialização das variaveis
        strcpy(estados[*qtdEstados].categorias[1].categoria, "Caminhao");
        strcpy(estados[*qtdEstados].categorias[2].categoria, "Caminhao trator");
        strcpy(estados[*qtdEstados].categorias[3].categoria, "Caminhonete");
        strcpy(estados[*qtdEstados].categorias[4].categoria, "Camioneta");
        strcpy(estados[*qtdEstados].categorias[5].categoria, "Micro-onibus");
        strcpy(estados[*qtdEstados].categorias[6].categoria, "Motocicleta");
        strcpy(estados[*qtdEstados].categorias[7].categoria, "Motoneta");
        strcpy(estados[*qtdEstados].categorias[8].categoria, "Onibus");
        strcpy(estados[*qtdEstados].categorias[9].categoria, "Trator de rodas");
        strcpy(estados[*qtdEstados].categorias[10].categoria, "Utilitario");
        strcpy(estados[*qtdEstados].categorias[11].categoria, "Outros");
        strcpy(estados[*qtdEstados].UF, dados[i].UF);

        estados[*qtdEstados].categorias[0].totalCateg = 0;
        estados[*qtdEstados].categorias[1].totalCateg = 0;
        estados[*qtdEstados].categorias[2].totalCateg = 0;
        estados[*qtdEstados].categorias[3].totalCateg = 0;
        estados[*qtdEstados].categorias[4].totalCateg = 0;
        estados[*qtdEstados].categorias[5].totalCateg = 0;
        estados[*qtdEstados].categorias[6].totalCateg = 0;
        estados[*qtdEstados].categorias[7].totalCateg = 0;
        estados[*qtdEstados].categorias[8].totalCateg = 0;
        estados[*qtdEstados].categorias[9].totalCateg = 0;
        estados[*qtdEstados].categorias[10].totalCateg = 0;
        estados[*qtdEstados].categorias[11].totalCateg = 0;
        auxVeiculosUF = 0;

        while(strcmp(dados[i].UF, estados[*qtdEstados].UF) == 0){ //Agrupar todos com mesma UF na mesma struct estados e calcula o total por UF
            if(strcmp(dados[i].tipoVeiculo, "Automovel") == 0){
                estados[*qtdEstados].categorias[0].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[0].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Caminhao") == 0){
                estados[*qtdEstados].categorias[1].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[1].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Caminhao trator") == 0){
                estados[*qtdEstados].categorias[2].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[2].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Caminhonete") == 0){
                estados[*qtdEstados].categorias[3].totalCateg+= (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[3].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Camioneta") == 0){
                estados[*qtdEstados].categorias[4].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[4].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Micro-onibus") == 0){
                estados[*qtdEstados].categorias[5].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[5].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Motocicleta") == 0){
                estados[*qtdEstados].categorias[6].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[6].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Motoneta") == 0){
                estados[*qtdEstados].categorias[7].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[7].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Onibus") == 0){
                estados[*qtdEstados].categorias[8].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[8].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Trator de rodas") == 0){
                estados[*qtdEstados].categorias[9].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[9].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Utilitario") == 0){
                estados[*qtdEstados].categorias[10].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[10].totalCateg;
            }
            else if(strcmp(dados[i].tipoVeiculo, "Outros") == 0){
                estados[*qtdEstados].categorias[11].totalCateg += (dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15);
                auxVeiculosUF += estados[*qtdEstados].categorias[11].totalCateg;
            }
            i++;
        }

        estados[*qtdEstados].totVeiculosUF = auxVeiculosUF;

        *qtdEstados += 1;
        estados = (tipoEstado*) realloc(estados, (*qtdEstados+1)*sizeof(tipoEstado));
    }
    return estados;
}

void criaHeapCat(tipoCategoria *categorias, int i, int f){
    tipoCategoria aux = categorias[i];
    int j = i * 2 + 1;

    while(j <= f){
        if(j < f){
            if(categorias[j].totalCateg < categorias[j+1].totalCateg) j = j+1;
        }
        if(aux.totalCateg < categorias[j].totalCateg){
            categorias[i] = categorias[j];
            i=j;
            j = 2* i + 1;
        }
        else j = f + 1;
    }
    categorias[i] = aux;
}
void heapSortCat(tipoCategoria *categorias, int N){
    int i;
    tipoCategoria aux;

    for(i=(N-1)/2; i>=0; i--) criaHeapCat(categorias, i, N-1); // do meio para a esquerda

    for(i=N-1; i>= 1; i--){
        aux = categorias[0];
        categorias[0] = categorias[i];
        categorias[i] = aux;
        criaHeapCat(categorias, 0, i-1); // reconstroi heap
    }// pega maior elemento e coloca na posição certa do vetor
}
void insertionSortCat(tipoCategoria *categorias, int N){
    int i, j;
    tipoCategoria aux;

    for(i=1; i<N; i++){
        j=(i-1);
        while(j>=0 && categorias[j+1].totalCateg < categorias[j].totalCateg){
            aux = categorias[j+1];
            categorias[j+1] = categorias[j];
            categorias[j] = aux;
            j--;
        }
    }
}
tipoMaiorEstado *ordenaMaiorEstado(tipoEstado *estados, int qtdEstados){ //Ordena, dentro do estado, de acordo com a categoria
    int i, j, maior;
    char tipoMaior[20];

    tipoMaiorEstado *maiores = (tipoMaiorEstado*) malloc(qtdEstados*sizeof(tipoMaiorEstado));

    printf("Selecione HeapSort(1) ou InsertionSort(2) para ordenar os dados de acordo com categoria de maior quantidade de cada UF: ");
    int op;
    scanf("%d", &op);
    for(i=0; i<qtdEstados; i++){
        if(op == 1) heapSortCat(estados[i].categorias, 12);
        else if(op == 2) insertionSortCat(estados[i].categorias, 12);
    }
    printf("Dados ordenados!\n\n");
}

tipoCategoria *agrupaCategoria(tipoFrota *dados, int *qtdCateg, int qtdLinhas){ //Agrupa os dados por categoria, guardando a quantidade total de cada uma
    int i = 0;
    int auxTotal = 0;
    *qtdCateg = 0;

    tipoCategoria *categorias = (tipoCategoria*) malloc(sizeof(tipoCategoria));

    while(i<qtdLinhas){
        strcpy(categorias[*qtdCateg].categoria, dados[i].tipoVeiculo);
        auxTotal = 0;
        while(strcmp(dados[i].tipoVeiculo, categorias[*qtdCateg].categoria) == 0){
            auxTotal = auxTotal + dados[i].qtdV12 + dados[i].qtdV13 + dados[i].qtdV14 + dados[i].qtdV15;
            i++;
        }
        categorias[*qtdCateg].totalCateg = auxTotal;

        *qtdCateg += 1;
        categorias = (tipoCategoria*) realloc(categorias, (*qtdCateg+1)*sizeof(tipoCategoria));
    }
    return categorias;
}

tipoAno *agrupaAno(tipoFrota *dados, int qtdAnos, int qtdLinhas){ //Agrupa os dados por ano (2012-2015)
    int i;
    int auxTotal;

    tipoAno *anos = (tipoAno*) malloc(qtdAnos*sizeof(tipoAno));
    anos[0].ano = 2012;
    anos[0].totalCategAno =0;
    anos[1].ano = 2013;
    anos[1].totalCategAno = 0;
    anos[2].ano = 2014;
    anos[2].totalCategAno = 0;
    anos[3].ano = 2015;
    anos[3].totalCategAno = 0;


    for(i=0; i<qtdLinhas; i++){
        anos[0].totalCategAno += dados[i].qtdV12;
        anos[1].totalCategAno += dados[i].qtdV13;
        anos[2].totalCategAno += dados[i].qtdV14;
        anos[3].totalCategAno += dados[i].qtdV15;
    }

    return anos;
}

int main(/*int argc, char* argv[]*/){

    printf(":::TRABALHO DE AEDII\n:::ICT UNIFESP\n:::Renata Sendreti Broder\n\n\nFrotas de veiculos no Brasil (2012-2015)\n\n");
    int i;
    int qtdLinhas; //ler dados da planilha
    FILE *arquivo = fopen("Frota.csv", "r");
    printf("Coletando dados...\n");
    tipoFrota *dados = coletarDadosPlanilha(arquivo, &qtdLinhas);
    fclose(arquivo);
    printf("Coletados!\n\n");

    int qtdCateg;
    tipoCategoria *categorias = agrupaCategoria(dados, &qtdCateg, qtdLinhas);
    FILE *arquivo1 = fopen("VeiculosCategoria.txt", "w"); //Gera arquivo "Total de veiculos por categoria"
    for(i=0; i<qtdCateg; i++) fprintf(arquivo1, "%s %d\n", categorias[i].categoria, categorias[i].totalCateg);
    fclose(arquivo1);

    int op; //ordena de acordo com UF
    printf("Selecione HeapSort(1) ou InsertionSort(2) para ordenar os dados de acordo com a UF: ");
    scanf("%d", &op);
    if(op == 1) heapSortUF(dados, qtdLinhas);
    else if(op == 2) insertionSortUF(dados, qtdLinhas);
    printf("Dados ordenados!\n\n");

    int qtdEstados;
    tipoEstado *estados = agrupaUF(dados, &qtdEstados, qtdLinhas);
    FILE *arquivo2 = fopen("VeiculosUF.txt", "w"); //Gera arquivo "Quantidade de veiculos por UF"
    for(i=0; i<qtdEstados; i++) fprintf(arquivo2, "%s %d\n", estados[i].UF, estados[i].totVeiculosUF);
    fclose(arquivo2);

    tipoMaiorEstado *maior = ordenaMaiorEstado(estados, qtdEstados);
    FILE *arquivo3 = fopen("MaiorCategUF.txt", "w"); //Gera arquivo "Maior categoria por Estado"
    for(i=0; i<qtdEstados; i++) fprintf(arquivo3, "%s %s %d\n", estados[i].UF, estados[i].categorias[11].categoria, estados[i].categorias[11].totalCateg);
    fclose(arquivo3);

    int qtdAnos = 4;
    tipoAno *anos = agrupaAno(dados, qtdAnos, qtdLinhas); //Agrupa os dados por ano (2012-2015)
    FILE *arquivo4 = fopen("TotalAnos.txt", "w");
    for(i=0; i<qtdAnos; i++) fprintf(arquivo4, "%d %d\n", anos[i].ano, anos[i].totalCategAno);
    fclose(arquivo4);

    printf("Gerando arquivos...\n\n");

    return 0;
}
