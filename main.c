#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "string.h"
#include <time.h>
#include <unistd.h>

int main() {
    LSTCLIENT list = {NULL, 0};
    srand(time(NULL));
    DATE date1 = {1, 1, 2022};
    DATE date2 = {30, 8, 2022};
    DATE date3 = {30, 12, 2030};
    struct ag algoritmo={10,0.01,2,10,5};
    COORDENADAS coordenadasMadrid = {40.416500, -3.702560};
    COORDENADAS coordenadasBarca = {41.388790, 2.158990};
    COORDENADAS coordenadasValencia = {39.46975, -0.37739};
    COORDENADAS coordenadasIbiza = {39.9088300, -1.4329600};
    COORDENADAS coordenadasGirona = {41.9831100, 2.8249300};


    struct client *cliente1 = createClient(123, "Jose");
    insertClient(&list, cliente1);
    inserirViagem(cliente1,"espanha",date1,date2);
    inserirCidade_Viagem(cliente1, "Madrid", 0, coordenadasMadrid);
    inserirCidade_Viagem(cliente1, "Barcelona", 0, coordenadasBarca);
    inserirCidade_Viagem(cliente1, "Valencia", 0, coordenadasValencia);
    inserirCidade_Viagem(cliente1, "Ibiza", 0, coordenadasIbiza);
    inserirCidade_Viagem(cliente1, "Girona", 0, coordenadasGirona);


    //read_client_from_file(&list,"client.txt");
    //read_viagens_from_file(&list,"viagens.txt");
    //printData(&list);
    //read_cidades_from_file(&list,"cidades.txt");
    //save_client_to_file(&list);
    //read_POI_from_file(&list,"poi.txt");
    //print_data_cidades(&list);
    //printPOI(12345,0,&list);


    struct populacao *populacao =generatePopulation(&cliente1->pviagens[0],algoritmo); //melhoria
    gerarAptidaoIndividual(populacao,algoritmo);
    print_data_population(populacao,algoritmo);

    return 0;
}

/**
* Funcao que cria o cliente para o ficheiro
* @param nif ;
* @param nome ;
* @return newClient;
*/

struct client *createClient(int nif, char *nome) {
    struct client *newClient = (CLIENT *) malloc(sizeof(CLIENT));
    newClient->nome = (char *) malloc(30 * sizeof(char));
    newClient->nif = nif;
    strcpy(newClient->nome, nome);
    newClient->nrViagens = 0;
    newClient->pviagens = (VIAGENS *) malloc(sizeof(VIAGENS));
    newClient->pviagens[0].flagOcupado = 0;
    newClient->pviagens[0].pais = (char *) malloc(30 * sizeof(char));
    newClient->pviagens[0].pcidade = (CIDADES *) malloc(sizeof(CIDADES));
    newClient->pviagens[0].pcidade[0].nome = (char *) malloc(30 * sizeof(char));
    newClient->pviagens[0].nrCidades = 0;
    newClient->pviagens[0].pcidade[0].flag = 0;
    newClient->pviagens[0].pcidade[0].nrPOI = 0;
    newClient->pviagens[0].pcidade[0].ppoI = (POI *) malloc(sizeof(POI));
    newClient->pviagens[0].pcidade[0].ppoI->nome = (char *) malloc(30 * sizeof(char));

    return newClient;
}

/**
* Funcao que insere clientes ordenados por NIF
* @param head - head to LSTCLIENT;
* @param newClient - client;
*/

    void insertClient(LSTCLIENT *head, struct client *newClient) {
        if (head->pclient == NULL) { //LISTA VAZIA
            head->pclient = newClient;
            head->sizeClient = 1;
            head->pclient->pnext = NULL;
            return;
        }
        struct client *temp = NULL;
        struct client *current = head->pclient;
        while (current != NULL && current->nif < newClient->nif) {
            temp = current;
            current = current->pnext;
        }
        if (current == head->pclient) {
            head->pclient->pnext = current;
            head->pclient = newClient;
            head->sizeClient++;
            return;
        }
        temp->pnext = newClient; //coloca o anterior a apontar para o novo
        newClient->pnext = current;//newClient aponta para o póximo node se for no meio ou null se estiver no fim
        head->sizeClient++;
    }

/**
* Funcao auxiliar que imprime a data das viagens
* @param head - head to LSTCLIENT;
*/
void printData(LSTCLIENT *head) {
    printf("Informacao relativa as viagens efetuadas pelos clientes:\n");
    int count = 0;
    int i = 0;
    CLIENT *node = head->pclient;
    while (node != NULL && i < head->sizeClient) {
        count++;;
        printf("Client Nr:%d, Nome: %s, NIF: %d\n", count, node->nome, node->nif);
        if (node->nrViagens > 0) {
            for (int j = 0; j < node->nrViagens; j++) {
                printf("\t|Viagem:0%d |Pais: %s |Data Partida:%d/%d/%d |Data Chegada:%d/%d/%d \n", j,
                       node->pviagens[j].pais, node->pviagens[j].dataPartida.day,
                       node->pviagens[j].dataPartida.month,
                       node->pviagens[j].dataPartida.year, node->pviagens[j].dataChegada.day,
                       node->pviagens[j].dataChegada.month, node->pviagens[j].dataChegada.year);
            }
        }
        i++;
        node = node->pnext;
    }
    printf("\n");
}

/**
* Funcao que apaga a posicao do cliente
* @param head - head to LSTCLIENT;
* @param position-client;
*/
void deleteClientPos(LSTCLIENT *head, int position) { //elimnar pelo nif
    struct client *current = head->pclient;
    struct client *previous = head->pclient;
    if (head->pclient == NULL) {
        printf("\nA lista ligada esta vazia\n");
    } else if (position == 1) {  //se quisermos apagar a primeira posição
        head->pclient = current->pnext;
        free(current);
    } else {
        while (position != 1) {
            previous = current;
            current = current->pnext;
            position--;
        }
        previous->pnext = current->pnext;
        free(current);
    }
    head->sizeClient--;
}

/**
* Funcao que procura o nif do cliente
* @param head - head to LSTCLIENT;
* @param nif - client;
* @return 0
*/
struct client *findClientNIF(LSTCLIENT *head, int nif) {
    struct client *temp = NULL;
    int count = 1;
    temp = head->pclient;
    while (temp != NULL) {  //percorrer a lista ate ao fim
        if (temp->nif == nif) {
            //printf("\nCliente encontrado na posicao :%d, nome=%s, nif=%d\n", count, temp->nome, temp->nif);
            return temp;
        } else {
            temp = temp->pnext;
            count++;
        }
    }
    printf("Cliente nao existente");
    return 0;
}

/**
* Funcao que procura o nome do cliente
* @param head - head to LSTCLIENT;
* @param name - client;
* @return 0
*/
struct client *findClientName(LSTCLIENT *head, char *name) {
    struct client *temp = NULL;
    temp = head->pclient;
    while (temp != NULL) {  //percorrer a lista ate ao fim
        if (strcmp(temp->nome, name) == 0) {
            printf("Client \"%s\" encontrado", temp->nome);
            return temp;
        } else {
            temp = temp->pnext;
        }
    }
    printf("Cliente nao existente");
    return 0;
}

/**
* Funcao que insere viagens
* @param temp - client;
* @param pais - viagem;
* @param datePartida - DATE;
* @param dateChegada - DATE;
*/
void inserirViagem(struct client *temp, char *pais, DATE datePartida, DATE dateChegada) { //ordenar por data
    int i = 0;
    if (temp->nrViagens == 0) { //cliente sem viagens
        temp->nrViagens = 1;


    } else { //cliente com pelo menos 1 viagem
        temp->nrViagens++;
        temp->pviagens = (VIAGENS *) realloc(temp->pviagens, temp->nrViagens * sizeof(VIAGENS));
        for (int j = temp->nrViagens - 1; j < temp->nrViagens; j++) {
            temp->pviagens[j].pais = (char *) malloc(30 * sizeof(char));
            temp->pviagens[j].dataPartida = datePartida;
            temp->pviagens[j].dataChegada = dateChegada;
            temp->pviagens[j].nrCidades = 0;
            temp->pviagens[j].flagOcupado = 0;

        }

    }
    while (i < temp->nrViagens) {
        if (temp->pviagens[i].flagOcupado == 0) {
            temp->pviagens[i].dataPartida = datePartida;
            temp->pviagens[i].dataChegada = dateChegada;
            temp->pviagens[i].flagOcupado = 1;
            temp->pviagens[i].pcidade = (CIDADES *) malloc(sizeof(CIDADES));
            strcpy(temp->pviagens[i].pais, pais);
        }
        i++;
    }

}
/**
* Funcao que remove viagens
* @param client - client;
* @param idViagem ;
*/
void removerViagem(struct client *client, int idViagem) {
    struct client *temp = NULL;
    temp = client;
    for (int i = 0; i <= temp->nrViagens; i++) {
        if (i == idViagem) {
            for (int j = i; j < temp->nrViagens - 1; j++) {
                strcpy(temp->pviagens[j].pais, temp->pviagens[j + 1].pais);
                temp->pviagens[j].dataPartida = temp->pviagens[j + 1].dataPartida;
                temp->pviagens[j].dataChegada = temp->pviagens[j + 1].dataChegada;
            }
            temp->nrViagens--;
            temp->pviagens = (VIAGENS *) realloc(temp->pviagens, temp->nrViagens * sizeof(VIAGENS));
        } else if (idViagem == temp->nrViagens) { //ultima posicao
            free(temp->pviagens[i].pais);
            temp->nrViagens--;
            temp->pviagens = (VIAGENS *) realloc(temp->pviagens, temp->nrViagens * sizeof(VIAGENS));
        }
    }
}

/**
* Funcao que edita viagens dependendo dos parametros
* @param client - client;
* @param idViagem - viagem;
* @param newDataPartida - DATE;
* @param newDataChegada - DATE;
* @param newPais ;
*/
void editarViagem(struct client *client, int idViagem, DATE newDataPartida, DATE newDataChegada, char *newPais) {
    if (newPais == NULL) { //apenas alterar data, pais mantem-se
        client->pviagens[idViagem].dataChegada = newDataChegada;
        client->pviagens[idViagem].dataPartida = newDataPartida;
        return;
    }
    strcpy(client->pviagens[idViagem].pais, newPais);
    client->pviagens[idViagem].dataChegada = newDataChegada;
    client->pviagens[idViagem].dataPartida = newDataPartida;
}
/**
* Funcao que insere cidade
* @param client - client;
* @param cidadeName - cidade;
* @param coordenadas - COORDENADAS;
*/
void inserirCidade_Viagem(struct client *client, char *cidadeName, int idViagem, COORDENADAS coordenadas) {
    struct client *temp = client;

    if (temp->nrViagens == 0) { //teste cliente sem viagens
        printf("Nao foi possivel adicionar cidades a vigaem do cliente com nif:%d, pois este nao tem viagens.\n",
               temp->nif);
        return;
    }
    if (temp->pviagens[idViagem].pais == NULL) { //teste id de viagem existe
        printf("Viagem com id:%d nao existe.\n", idViagem);
        return;
    }
    if (temp->pviagens[idViagem].nrCidades == 0) {
        temp->pviagens[idViagem].nrCidades = 1;//se nrcidades==0 entao nrcidades=1;

        for (int i = temp->pviagens[idViagem].nrCidades - 1; i < temp->pviagens[idViagem].nrCidades; i++) {
            temp->pviagens[idViagem].pcidade[i].nome = (char *) malloc(30 * sizeof(char));
            temp->pviagens[idViagem].pcidade[i].flag = 0;
            temp->pviagens[idViagem].pcidade[i].nrPOI = 0;
        }
    } else {
        temp->pviagens[idViagem].nrCidades++;
        temp->pviagens[idViagem].pcidade = (CIDADES *) realloc(temp->pviagens[idViagem].pcidade,
                                                               temp->pviagens[idViagem].nrCidades *
                                                               sizeof(CIDADES));

        for (int i = temp->pviagens[idViagem].nrCidades - 1; i < temp->pviagens[idViagem].nrCidades; i++) {
            temp->pviagens[idViagem].pcidade[i].nome = (char *) malloc(30 * sizeof(char));
            temp->pviagens[idViagem].pcidade[i].flag = 0;
            temp->pviagens[idViagem].pcidade[i].nrPOI = 0;
        }
    }

    int z = 0;
    while (z < temp->pviagens->nrCidades) {
        if (temp->pviagens[idViagem].pcidade[z].flag == 0) { //testar se a posição já esta ocupada com a flag
            temp->pviagens[idViagem].pcidade[z].flag = 1;
            strcpy(temp->pviagens[idViagem].pcidade[z].nome, cidadeName);
            temp->pviagens[idViagem].pcidade[z].coordenadas = coordenadas;
        }
        z++;
    }
}

/**
* Funcao auxiliar  que imprime informacao relativa as cidades
* @param head - head to LSTCLIENT;

*/
void print_data_cidades(LSTCLIENT *head) {
    printf("Informacao relativa as viagens efetuadas por todos os clientes:\n");
    int i = 0;
    CLIENT *node = head->pclient;
    while (node != NULL && i < head->sizeClient) {
        if (node->nrViagens > 0) {
            for (int j = 0; j < node->nrViagens; j++) {
                if (node->pviagens[j].nrCidades > 0) {
                    printf("Client:%s | Nif:%d | viagemId:%d\n", node->nome, node->nif, j);
                    for (int k = 0; k < node->pviagens[j].nrCidades; k++) {
                        printf("\tCidade0%d:%s, coordenadas[%lf|%lf]\n", k, node->pviagens[j].pcidade[k].nome,
                               node->pviagens[j].pcidade[k].coordenadas.latitude,
                               node->pviagens[j].pcidade[k].coordenadas.longitude);
                    }
                }
            }
        }
        printf("\n");
        i++;
        node = node->pnext;
    }
}
/**
* Funcao que edita cidades
* @param client - client;
* @param nif ;
* @param newName ;
* @param idViagem ;
* @param idCidade ;
*/
void editarCidade(struct client *client, int nif, char *newName, int idViagem, int idCidade) {
    struct client *temp = NULL;
    temp = client;
    strcpy(temp->pviagens[idViagem].pcidade[idCidade].nome, newName);
}

/**
* Funcao que remove Cidades
* @param client - client;
* @param nif;
* @param idViagem;
* @param idCidade;
*/
void removerCidade(struct client *client, int nif, int idViagem, int idCidade) {
    struct client *temp = NULL;
    temp = client;
    for (int i = 0; i < temp->pviagens[idViagem].nrCidades; i++) {
        if (i == idCidade) {
            for (int j = i; j < temp->pviagens[idViagem].nrCidades - 1; j++) {
                strcpy(temp->pviagens[idViagem].pcidade[j].nome, temp->pviagens[idViagem].pcidade[j + 1].nome);
            }
            temp->pviagens[idViagem].nrCidades--;
            temp->pviagens[idViagem].pcidade = (CIDADES *) realloc(temp->pviagens[idViagem].pcidade,
                                                                   temp->pviagens[idViagem].nrCidades *
                                                                   sizeof(CIDADES));
        } else if (idCidade == temp->pviagens->nrCidades) {
            free(temp->pviagens[idViagem].pcidade->nome);
            temp->pviagens[idViagem].nrCidades--;
            temp->pviagens[idViagem].pcidade = (CIDADES *) realloc(temp->pviagens[idViagem].pcidade,
                                                                   temp->pviagens[idViagem].nrCidades *
                                                                   sizeof(CIDADES));
        }
    }
}

/**
* Funcao que cria pontos de interesse
* @param client - client;
* @param idViagem ;
* @param idCidade;
*/
void createPoiList(struct client *client, int idViagem, int idCidade) {
    struct client *temp = NULL;
    temp = client;
    temp->pviagens[idViagem].pcidade[idCidade].ppoI = (POI *) malloc(sizeof(POI));
    temp->pviagens[idViagem].pcidade[idCidade].ppoI->pnext = NULL;
    temp->pviagens[idViagem].pcidade[idCidade].ppoI->nome = (char *) malloc(30 * sizeof(char));
    temp->pviagens[idViagem].pcidade[idCidade].nrPOI = 0;
}
/**
* Funcao que insere pontos de interesse
* @param client - client;
* @param idViagem ;
* @param idCidade ;
* @param poiName ;
*/
void insertPOI(struct client *client, int idViagem, int idCidade, char *poiName) {
    struct client *temp = client;
    struct poI *newPoi = createPoi(poiName);

    if (temp->pviagens[idViagem].pcidade[idCidade].nrPOI == 0) { //ainda nao existem POI's
        temp->pviagens[idViagem].pcidade[idCidade].nrPOI = 1;
        temp->pviagens[idViagem].pcidade[idCidade].ppoI = newPoi;
        return;
    } else {
        struct poI *aux = client->pviagens[idViagem].pcidade[idCidade].ppoI;
        while (aux->pnext != NULL) {
            aux = aux->pnext;
        }
        newPoi->poiId = aux->poiId + 1;
        aux->pnext = newPoi;
        temp->pviagens[idViagem].pcidade[idCidade].nrPOI++;
        return;
    }

}
/**
* Funcao que imprime pontos de interesse
* @param temp- client;
* @param idViagem ;
* @param idCidade ;
*/
void printPOI(int nif, int idViagem, LSTCLIENT *head) {
    CLIENT* temp=findClientNIF(head,nif);
    printf("\nInformacao relativa ao pontos de interesse visitados pelo cliente:%s\n",temp->nome);
    int i = 0;
    for(int l=0;l<temp->pviagens[idViagem].nrCidades;l++){
        struct poI *aux = temp->pviagens[idViagem].pcidade[l].ppoI;
        while ((aux != NULL)&& i<temp->pviagens[idViagem].nrCidades-1) {
            printf("Cidade:%d=> ID:%d | Nome:%s\n",l, aux->poiId, aux->nome);
            aux = aux->pnext;
            i++;
        }
    }
}
/**
* Funcao que edita pontos de interesse
* @param temp- client;
* @param idViagem ;
* @param idCidade ;
* @param newName ;
* @param poiID ;
*/
void editarPOI(struct client *temp, int idViagem, int idCidade, char *newName, int poiID) {
    struct poI *aux = temp->pviagens[idViagem].pcidade[idCidade].ppoI; //aux=head dos POIS do cliente passado a funcao
    while (aux != NULL) {
        if (aux->poiId == poiID) {
            strcpy(aux->nome, newName);
        }
        aux = aux->pnext;
    }
}
/**
* Funcao que remove pontos de interesse
* @param temp- client;
* @param idViagem ;
* @param idCidade ;
* @param poiID ;
*/
void removerPOI(struct client *temp, int idViagem, int idCidade, int poiID) {
    struct poI *current = temp->pviagens[idViagem].pcidade[idCidade].ppoI; //aux=head dos POIS do cliente passado a funcao
    struct poI *prev = NULL;

    while (current != NULL) {
        if (current->poiId == poiID && poiID == 1) { //1=ultima posicao (lista ordenada maiorID->menorID)
            free(current);
            temp->pviagens[idViagem].pcidade[idCidade].nrPOI--;
            temp->pviagens[idViagem].pcidade[idCidade].ppoI = (POI *) realloc(
                    temp->pviagens[idViagem].pcidade[idCidade].ppoI,
                    temp->pviagens[idViagem].pcidade[idCidade].nrPOI *
                    sizeof(POI));
            return;
        } else if (current->poiId == poiID - 1) { //qualquer posicao do meio
            prev = current;
            prev->pnext = current->pnext;
            free(current);
            temp->pviagens[idViagem].pcidade[idCidade].nrPOI--;
            temp->pviagens[idViagem].pcidade[idCidade].ppoI = (POI *) realloc(
                    temp->pviagens[idViagem].pcidade[idCidade].ppoI,
                    temp->pviagens[idViagem].pcidade[idCidade].nrPOI *
                    sizeof(POI));
            return;

        } else if (current->poiId == poiID && poiID ==
                                              temp->pviagens[idCidade].pcidade[idCidade].nrPOI) {//se nrPOI=idPOI entao estamos a na primera posicao
            temp->pviagens[idViagem].pcidade[idCidade].ppoI = current->pnext;
            free(current);
            temp->pviagens[idViagem].pcidade[idCidade].nrPOI--;
            temp->pviagens[idViagem].pcidade[idCidade].ppoI = (POI *) realloc(
                    temp->pviagens[idViagem].pcidade[idCidade].ppoI,
                    temp->pviagens[idViagem].pcidade[idCidade].nrPOI *
                    sizeof(POI));
            return;
        }
        current = current->pnext;
    }
}

/**
* Funcao que escreve  para o ficheiro "history.txt" o relatorio de viagens
* @param temp- client;
* @param d1 - date ;
* @param d2 - date ;
*/

void relatorioViagens(struct client *temp, struct date d1, struct date d2) {
    FILE *fp;
    fp = fopen("history.txt", "w");
    for (int i = 0; i < temp->nrViagens; i++) {
        if (compare_dates(temp->pviagens[i].dataPartida, d1) >= 0 &&
            compare_dates(temp->pviagens[i].dataPartida, d2) < 0) {
            fprintf(fp, "viagem:0%d| %s| Data Partida:%d-%d-%d\n", i, temp->pviagens[i].pais,
                    temp->pviagens[i].dataPartida.day,
                    temp->pviagens[i].dataPartida.month, temp->pviagens[i].dataPartida.year);
        }
    }
}

/**
* Funcao que compara as datas
* @param head - head to LSTCLIENT;
* @param newClient - client;
* @return 0 ou 1
*/
int compare_dates(struct date d1, struct date d2) {
    if (d1.year < d2.year)
        return -1;

    else if (d1.year > d2.year)
        return 1;

    if (d1.year == d2.year) {
        if (d1.month < d2.month)
            return -1;
        else if (d1.month > d2.month)
            return 1;
        else if (d1.day < d2.day)
            return -1;
        else if (d1.day > d2.day)
            return 1;
        else
            return 0;
    }
}


void quickSort(struct client *aux, int left, int right) {
    int mid;
    if (left < right) {
        mid = partition(aux, left, right);
        quickSort(aux, left, mid - 1);
        quickSort(aux, mid + 1, right);
    }
}

int partition(struct client *aux, int left, int right) {
    struct date p = aux->pviagens[right].dataPartida;
    int i, j;
    struct viagens *temp = (VIAGENS *) malloc(sizeof(VIAGENS));

    for (i = left, j = i - 1; i < right; i++) {
        if (compare_dates(aux->pviagens[i].dataPartida, p) == -1) {
            j++;
            *temp = aux->pviagens[j];
            aux->pviagens[i] = aux->pviagens[j];
            aux->pviagens[j] = *temp;
        }
    }//place pivot
    *temp = aux->pviagens[j + 1];
    aux->pviagens[j + 1] = aux->pviagens[right];
    aux->pviagens[right] = *temp;
    return j + 1;
}

/**
* Funcao que grava o cliente para o ficheiro
* @param head - head to LSTCLIENT;
*/
void save_client_full_info(LSTCLIENT *head) {
    FILE *fp;
    fp = fopen("client_file.txt", "w");
    fprintf(fp, "Numero de clients: %d\n", head->sizeClient);
    CLIENT *temp = head->pclient;
    int aux = 0;
    while (temp->pnext != NULL) {
        fprintf(fp, "Nome do cliente: %s | NIF:%d\n", temp->nome, temp->nif);
        if (temp->nrViagens == 0) {
            break;
        }
        for (int j = 0; j < temp->nrViagens; j++) {
            fprintf(fp, "Viagem: %s\n", temp->pviagens[j].pais); //escrever o pais e tempo de estadia
            for (int i = 0; i < temp->pviagens[j].nrCidades; i++) {
                fprintf(fp, "Cidade0%d: %s, POIS: \t", i,
                        temp->pviagens[j].pcidade[i].nome); //escrever as cidades no ficheiro
                while (temp->pviagens[j].pcidade[i].ppoI->pnext != NULL) {
                    if (temp->pviagens[j].pcidade[i].ppoI == NULL) {
                        break;
                    }
                    aux++;
                    fprintf(fp, "|%s| ", temp->pviagens[j].pcidade[i].ppoI->nome);
                    fprintf(fp, "\t");
                    temp->pviagens[j].pcidade[i].ppoI = temp->pviagens[j].pcidade[i].ppoI->pnext;
                }
            }
            fprintf(fp, "\n\n");
        }
        temp = temp->pnext;
    }
    fclose(fp);
}
/**
* Funcao que grava o cliente para o ficheiro
* @param head - head to LSTCLIENT;

*/
void save_client_to_file(LSTCLIENT *head) {
    FILE *file = fopen("client.txt", "w");
    struct client *temp = head->pclient;
    int i = 0;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%d\n", i, temp->nome, temp->nif);
        temp = temp->pnext;
        i++;
    }
    fclose(file);
}
/**
* Funcao que le o cliente para o ficheiro
* @param head - head to LSTCLIENT;
* @param filename - client;
*/
void read_client_from_file(LSTCLIENT *head, char *filename) {
    FILE *file = fopen(filename, "r");
    char line[256];
    char *string;
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        size_t last_idx = strlen(line) - 1;
        if (line[last_idx] == '\n') { //colocar todas as linhas a acabar \0
            line[last_idx] = '\0';
        }
        char *nome = malloc(30 * sizeof(char)); //alocar memoria para o nome;
        int nif;
        int nrViagens;
        string = strtok(line, ",");
        while (string != NULL) {
            if (count == 0) {
            }
            if (count == 1) {
                strcpy(nome, string);
            }
            if (count == 2) {
                nif = atoi(string);
            }
            count++;
            string = strtok(NULL, ",");
        }
        struct client *newClient = createClient(nif, nome);
        insertClient(head, newClient);
        count = 0;
    }
    fclose(file);
}

/**
* Funcao que grava as viagens para o ficheiro
* @param head - head to LSTCLIENT;
*/
void save_viagens_to_file(LSTCLIENT *head) {
    quickSort(head->pclient, 0, head->pclient->nrViagens);
    FILE *file = fopen("viagens.txt", "w");
    struct client *temp = head->pclient;
    int count = 0;
    while (temp != NULL) {
        for (int i = 0; i < temp->nrViagens; i++) {
            fprintf(file, "%d,%d,%s,", temp->nif, count, temp->pviagens[i].pais);
            DATE dataChegada = temp->pviagens[i].dataChegada;
            DATE dataPartida = temp->pviagens[i].dataPartida;
            fprintf(file, "%d-%d-%d,", dataPartida.day, dataPartida.month, dataPartida.year);
            fprintf(file, "%d-%d-%d\n", dataChegada.day, dataChegada.month, dataChegada.year);
        }
        count++;
        temp = temp->pnext;
    }
    fclose(file);
}

/**
* Funcao que le as viagens do ficheiro
* @param head - head to LSTCLIENT;
* @param filename - viagens;
*/
void read_viagens_from_file(LSTCLIENT *head, char *filename) {
    FILE *file = fopen(filename, "r");
    char line[256];
    char *string;
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        size_t last_idx = strlen(line) - 1;
        if (line[last_idx] == '\n') { //colocar todas as linhas a acabar \0
            line[last_idx] = '\0';
        }
        int nif = 0, idViagem = 0;
        int day1 = 0, month1 = 0, year1 = 0;
        int day2 = 0, month2 = 0, year2 = 0;
        char *pais = malloc(30 * sizeof(char));
        string = strtok(line, ",");
        while (string != NULL) {
            if (count == 0)
                nif = atoi(string);
            if (count == 1)
                idViagem = atoi(string);
            if (count == 2)
                strcpy(pais, string);
            if (count == 3)
                sscanf(string, "%d%*[^0123456789]%d%*[^0123456789]%d", &day1, &month1, &year1);
            if (count == 4)
                sscanf(string, "%d%*[^0123456789]%d%*[^0123456789]%d", &day2, &month2, &year2);
            count++;
            string = strtok(NULL, ",");
        }
        DATE dataChegada = {day1, month1, year1};
        DATE dataPartida = {day2, month2, year2};
        struct client *temp = findClientNIF(head, nif);
        inserirViagem(temp, pais, dataChegada, dataPartida);
        count = 0;
    }
    fclose(file);

}
/**
* Funcao que grava cidades para ficheiro
* @param head - head to LSTCLIENT;
*/
void save_cidades_to_file(LSTCLIENT *head) {
    FILE *file = fopen("cidades.txt", "w");
    struct client *temp = head->pclient;
    while (temp != NULL) {
        for (int i = 0; i < temp->nrViagens; i++) {
            for (int j = 0; j < temp->pviagens[i].nrCidades; j++) {
                fprintf(file, "%d,%d,", temp->nif, i);
                fprintf(file, "%s,", temp->pviagens[i].pcidade[j].nome);
                fprintf(file, "%lf|%lf\n", temp->pviagens[i].pcidade[j].coordenadas.latitude,
                        temp->pviagens[i].pcidade[j].coordenadas.longitude);
            }
        }
        temp = temp->pnext;
    }
    fclose(file);
}
/**
* Funcao que le ficheiros para o ficheiro
* @param head - head to LSTCLIENT;
* @param filename - cidades;
*/
void read_cidades_from_file(LSTCLIENT *head, char *filename) {
    FILE *file = fopen(filename, "r");
    char line[256];
    char *string;
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        size_t last_idx = strlen(line) - 1;
        if (line[last_idx] == '\n') { //colocar todas as linhas a acabar \0
            line[last_idx] = '\0';
        }
        int nif, idViagem = 0;
        double longitude, latitude = 0;
        char *cidadeName = malloc(30 * sizeof(char));
        string = strtok(line, ",");
        while (string != NULL) {
            if (count == 0) {
                nif = atoi(string);
            }
            if (count == 1) {
                idViagem = atoi(string);
            }
            if (count == 2) {
                strcpy(cidadeName, string);
            }
            if (count == 3) {
                sscanf(string, "%lf%*[^0123456789-]%lf", &latitude, &longitude);

            }
            count++;
            string = strtok(NULL, ",");
        }
        COORDENADAS coordenadas = {latitude, longitude};
        struct client *temp = findClientNIF(head, nif);
        inserirCidade_Viagem(temp, cidadeName, idViagem, coordenadas);
        count = 0;
    }
    fclose(file);
}

/**
* Funcao que grava os pontos de interesse para ficheiro
* @param head - head to LSTCLIENT;
*/
void save_POI_to_file(LSTCLIENT *head) {
    FILE *file = fopen("poi.txt", "w");
    struct client *temp = head->pclient;
    while (temp != NULL) {
        for (int i = 0; i < temp->nrViagens; i++) {
            for (int j = 0; j < temp->pviagens[i].nrCidades; j++) {
                struct poI *aux = temp->pviagens[i].pcidade[j].ppoI;
                if (temp->pviagens[i].pcidade[j].nrPOI >= 1) {
                    while (aux != NULL) {
                        fprintf(file, "%d,%d,", temp->nif, i); //i=idViagem
                        fprintf(file, "%d,", j);//j==idCidade
                        fprintf(file, "%s\n", aux->nome);
                        aux = aux->pnext;
                    }
                }
            }
        }
        temp = temp->pnext;
    }
    fclose(file);
}

/**
* Funcao que grava os clientes  para ficheiros binarios
* @param head - head to LSTCLIENT;
*/
void save_clients_bin(LSTCLIENT *head) {
    FILE *fp = NULL;
    fp = fopen("clientsBin.bin", "wb");
    CLIENT *client = head->pclient;
    fwrite(&head->sizeClient, sizeof(int), 1, fp);
    int size;
    while(client!=NULL){
        fwrite(&client->nif, sizeof(int), 1, fp);
        size = strlen(client->nome) + 1;
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(client->nome, sizeof(char), size, fp);
        client= client->pnext;
    }
    fclose(fp);
}


/**
* Funcao que le os clientes dos ficheiros binarios
* @param head - head to LSTCLIENT;
*/
void read_clients_bin(char filename[],LSTCLIENT *head) {
    FILE *fp = NULL;
    fp = fopen(filename, "rb");
    char *nome = malloc(sizeof(char) * 30);
    int sizenome = 0;
    int nif;
    int aux = 0;
    fread(&aux, sizeof(int), 1, fp);
    for (int i = 0; i < aux; i++) {
        fread(&nif, sizeof(int), 1, fp);
        fread(&sizenome, sizeof(int), 1, fp);
        fread(nome, sizeof(char), sizenome, fp);
        struct client *clientaux = createClient(nif, nome);
        insertClient(head, clientaux);
    }
}

/**
* Funcao que cria  ponto de interesse
* @param head - head to LSTCLIENT;
* @return 1 ponto de interesse
*/
struct poI *createPoi(char *poiName) {
    struct poI *Poi = malloc(sizeof(POI));
    Poi->nome = malloc(30 * sizeof(char));
    strcpy(Poi->nome, poiName);
    Poi->pnext = NULL;
    Poi->poiId = 0;
    return Poi;
}

/**
* Funcao que le pontos de interesse do ficheiro
* @param head - head to LSTCLIENT;
* @param filename - poi;
*/
void read_POI_from_file(LSTCLIENT *head, char *filename) {
    FILE *file = fopen(filename, "r");
    char line[256];
    char *string;
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        size_t last_idx = strlen(line) - 1;
        if (line[last_idx] == '\n') { //colocar todas as linhas a acabar \0
            line[last_idx] = '\0';
        }
        int nif=0;
        int idCidade=0;
        int idViagem = 0;
        char *poiName = malloc(30 * sizeof(char));
        string = strtok(line, ",");
        while (string != NULL) {
            if (count == 0) {
                nif = atoi(string);
            }
            if (count == 1) {
                idViagem = atoi(string);
            }
            if (count == 2) {
                idCidade = atoi(string);
            }
            if (count == 3) {
                strcpy(poiName, string);
            }
            count++;
            string = strtok(NULL, ",");
        }
        struct client *temp = findClientNIF(head, nif);

        insertPOI(temp, idViagem, idCidade, poiName);
        count = 0;
    }
    fclose(file);
}

/**
* Funcao que calcula a distancia entre as cidades
* @param cidade1 - CIDADES;
* @param cidade2 - CIDADES;
* @return distancia
*/
double dist(CIDADES cidade1, CIDADES cidade2) {
    double distancia = 0;
    distancia = sqrt((cidade2.coordenadas.longitude - cidade1.coordenadas.longitude) *
                     (cidade2.coordenadas.longitude - cidade1.coordenadas.longitude) +
                     (cidade2.coordenadas.latitude - cidade1.coordenadas.latitude) *
                     (cidade2.coordenadas.latitude - cidade1.coordenadas.latitude));

    return distancia;
}

/**
* Funcao que calcula a aptidao
* @param trajeto - cidades;
* @param size ;
* @return aptidao;
*/
double aptidao(struct cidades *trajeto, int size) {
    double aptidao = 0;

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            aptidao += 1 / (dist(trajeto[i], trajeto[0])); //quando estamos na ultima posicao do trajeto temos que calcular a distancia com a primeira
            return aptidao;
        }
        aptidao += 1 / (dist(trajeto[i], trajeto[i + 1]));
    }

    return aptidao;
}
/**
* Funcao que gera a populacao
* @param viagens - viagens;
* @param algoritmo - ag;
* @return populacao
*/
struct populacao *generatePopulation(struct viagens *viagem1, struct ag algoritmo) {
    if(viagem1==NULL){
        struct populacao *populacao1;
        return populacao1=NULL;
    }
    struct populacao *populacao1 = malloc(sizeof(POPULACAO) * algoritmo.sizePop);
    struct cidades *aux=(CIDADES *)malloc(sizeof(CIDADES)*algoritmo.nrCidades);
    for (int i = 0; i < algoritmo.sizePop; i++) {
        for (int j = 0; j < algoritmo.nrCidades; j++) {
            populacao1[i].ptrajeto = (CIDADES *) malloc(sizeof(CIDADES) * algoritmo.nrCidades);
            populacao1[i].aptidao = 0;
        }
    }

    for (int z = 0; z < algoritmo.sizePop; z++) {
        aux = randomize(viagem1, algoritmo.nrCidades);
        for(int k=0;k<algoritmo.nrCidades;k++){
            populacao1[z].ptrajeto[k] = aux[k];
        }
    }

    return populacao1;
}


/**
* Funcao que gera aleatoreamente as viagens
* @param viagens - viagens;
* @param algoritmo - ag;
* @return array de cidades aleatorio
*/
struct cidades *randomize(struct viagens *viagem, int size) {
    int i;
    struct cidades *aux=viagem->pcidade;
    for (i = size - 1; i > 0; i--) {
        int j = rand() % size + 0;
        CIDADES temp = aux[i];
        aux[i] = aux[j];
        aux[j] = temp;
    }
    return aux;
}

/**
* Funcao que gera Aptidao Individual
* @param populacao - populacao;
* @param ag - ag;
*/
void gerarAptidaoIndividual(struct populacao *populacao, struct ag ag) {
    double apt1 = 0;
    for (int i = 0; i < ag.sizePop; i++) {
        for (int j = 0; j < ag.nrCidades; j++) {
            populacao[i].aptidao = aptidao(populacao[i].ptrajeto, ag.nrCidades);
        }
    }
}

/**
* Funcao que gera Aptidao mais apta
* @param populacao1 - populacao;
* @param algoritmo - ag;
*@return  a aptidao de individuo mais apto
*/
double aptidaoMaisApto(struct populacao *populacao1,struct ag *algoritmo){
    double aux;
    for(int i=0;i<algoritmo->sizePop;i++){
        if(populacao1[i].aptidao<populacao1[i+1].aptidao){
            aux=populacao1[i+1].aptidao;
        }
    }
    printf("APITDAO DO MAIS APTO:%lf\n",aux);
    return aux;
}
/**
* Funcao que gera insere geracao
* @param head - head to LSTALGORITMO;
* @param newentry - info-algoritmo;
* @param populacao1 - populacao;
* @param algoritmo - ag ;
*/
void insertGeracao(LSTALGORITMO *head, struct info_algoritmo *newentry, struct populacao *populacao1,struct ag *algoritmo){
    newentry->individuoMaisApto=aptidaoMaisApto(populacao1,algoritmo);

    if (head->palgoritmo == NULL) { //LISTA VAZIA
        head->palgoritmo = newentry;
        head->palgoritmo->id=1;
        head->sizeAlgortimo = 1;
        head->palgoritmo->pnext = NULL;
        return;
    }
    struct info_algoritmo *temp = NULL;
    struct info_algoritmo *current = head->palgoritmo;
    while (current != NULL && current->individuoMaisApto < newentry->individuoMaisApto) {
        temp = current;
        current = current->pnext;
    }
    if (current == head->palgoritmo) {
        head->palgoritmo->pnext = current;
        head->palgoritmo->id=current->id-1;
        head->palgoritmo = newentry;
        newentry->pnext->id=newentry->id+1;
        head->sizeAlgortimo++;
        return;
    }
    temp->pnext = newentry;
    newentry->id=current->id+1;
    newentry->pnext = current;
    head->sizeAlgortimo++;

}
/**
* Funcao que imprime a data de uma populacao
* @param populacao1 - populacao;
* @param algoritmo -ag;
*/
void print_data_population(struct populacao *populacao, struct ag algoritmo) {
for (int l = 0; l < algoritmo.sizePop; l++) {
    printf("Trajeto %d\n",l);
       for (int x = 0; x < algoritmo.nrCidades; x++) {
           printf("\t%s\n", populacao[l].ptrajeto[x].nome);
       }
       printf("Aptidao:%f\n", populacao[l].aptidao);
   }
}
