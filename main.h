//
// Created by rpssr on 11/11/2021.
//

#ifndef PONTOAPONTO_MAIN_H
#define PONTOAPONTO_MAIN_H

typedef struct date{
    int day;
    int month;
    int year;
}DATE;

typedef struct poI{ //points of interest
    char *nome;
    int poiId;
    struct poI *pnext;
}POI;

typedef struct {
    double latitude;
    double longitude;
}COORDENADAS;

typedef struct cidades{
    char* nome;
    int flag;
    POI *ppoI;
    int nrPOI;
    int cidadeID;
    COORDENADAS coordenadas;
}CIDADES;

typedef struct viagens{

    char* pais;
    int flagOcupado;
    CIDADES *pcidade;
    int nrCidades;
    DATE dataPartida;
    DATE dataChegada;
}VIAGENS;

typedef struct client{
    char * nome;
    int nif;
    struct client *pnext;
    int nrViagens;
    VIAGENS *pviagens;
}CLIENT;

typedef struct lstClient{
    CLIENT *pclient;
    int sizeClient;
}LSTCLIENT;

typedef struct populacao {
    CIDADES *ptrajeto;
    double aptidao;
}POPULACAO;


typedef struct info_algoritmo{
    int id;
    double individuoMaisApto;
    struct info_algoritmo *pnext;
}INFO_ALGORITMO;

typedef struct lstAlgoritmo{
    INFO_ALGORITMO *palgoritmo;
    int sizeAlgortimo;
}LSTALGORITMO;

typedef struct ag{
    int sizePop;
    double probs;
    int nrElitismo;
    int nrGeracoes;
    int nrCidades;
}AG;

int main();

//funcoes clientes
void addClientAtBeg(LSTCLIENT *head, int nif, char* nome);
void addClientAtEnd(LSTCLIENT *head, int nif, char *nome);
void deleteClientPos(LSTCLIENT *head, int position);
void printData(struct lstClient *head);
struct client* findClientNIF(LSTCLIENT *head, int nif);
struct client *findClientName(LSTCLIENT *head, char* name);
void insertClient(LSTCLIENT *head, struct client *newClient);
struct client * createClient (int nif,  char *nome);


//funcoes Viagens
void inserirViagem(struct client *temp, char *pais, DATE datePartida,DATE dateChegada);
void removerViagem(struct client *client ,int idViagem);
void editarViagem(struct client *client, int idViagem, DATE newDataPartida, DATE newDataChegada, char *newPais);
void historicoViagens(struct client *temp, char* cidadeName, char *poiName);
void relatorioViagens(struct client *temp, struct date d1, struct date d2);


//funcoes cidades
void print_data_cidades(LSTCLIENT *head);
void inserirCidade_Viagem(struct client *client, char *cidadeName, int idViagem, COORDENADAS coordenadas);
void editarCidade(struct client *client, int nif, char *newName, int idViagem, int idCidade);
void removerCidade(struct client *client, int nif, int idViagem, int idCidade);

//funcoes POI
void createPoiList(struct client *client, int idViagem, int idCidade);
void insertPOI(struct client *client , int idViagem, int idCidade, char *poiName);
void printPOI(struct client *temp,int idViagem, int idCidade);
void editarPOI(struct client *temp,int idViagem,int idCidade, char* newName, int poiID);
void removerPOI(struct client *temp, int idViagem,int idCidade,int poiID);
struct poI *createPoi(char* poiName);

//funcoes ficheiros
void save_client_full_info(LSTCLIENT *head);
void save_client_to_file(LSTCLIENT *head);
void read_client_from_file(LSTCLIENT *head, char* filename);
void save_viagens_to_file(LSTCLIENT *head);
void read_viagens_from_file(LSTCLIENT *head,char* filename);
void save_cidades_to_file(LSTCLIENT *head);
void read_cidades_from_file(LSTCLIENT *head, char *filename);
void save_POI_to_file(LSTCLIENT *head);
void read_POI_from_file(LSTCLIENT *head, char* filename);


//funcoes aux
void quickSort(struct client *aux,int first,int last);
int compare_dates (struct date d1, struct date d2);
int partition(struct client *aux, int left, int right);

struct cidades *randomize(struct viagens *viagem, int size);
void swap(CIDADES a, CIDADES b);

//funcoes AG
struct populacao *generatePopulation(struct viagens *viagem1, struct ag algoritmo);
double aptidao(struct cidades *trajeto, int size);
double dist(CIDADES cidade1, CIDADES cidade2);
void gerarAptidaoIndividual(struct populacao *populacao, struct ag ag);
void print_data_population(struct populacao *populacao, struct ag algoritmo) ;


#endif //PONTOAPONTO_MAIN_H

