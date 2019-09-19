
#define WIN // Se não for no windows comente essa linha e compile no terminal: gcc -o ts ts.c
#include "locale.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#ifdef WIN
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#define N 30
#define TAM_MENSAGEM 255     /* mensagem de maior tamanho */

#define PORTA_SERVIDOR_TCP 9999

#define MAXPENDING 5    /* Número máximo de requisições para conexão pendentes */

int players = 0;

typedef struct lorem{
    char nome[20];
    char IP[16];
    char porta[6];
    int classe, nivel, vida;

}LOREM;

LOREM *l[N];
void setlorem(void)
{
    for(int i =0; i<N; i++)
        l[i]=NULL;
}

int criar_socket(int porta)
{
    int sock;
    struct sockaddr_in endereco; /* Endereço Local */

    /* Criação do socket TCP para recepção e envio de pacotes */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErro na criação do socket!\n");fflush(stdout);
        return(-1);
    }

    if (porta > 0)
    {
        /* Construção da estrutura de endereço local */
        memset(&endereco, 0, sizeof(endereco));       /* Zerar a estrutura */
        endereco.sin_family      = AF_INET;           /* Família de endereçamento da Internet */
        endereco.sin_addr.s_addr = htonl(INADDR_ANY); /* Qualquer interface de entrada */
        endereco.sin_port        = htons(porta);      /* Porta local */

        /* Instanciar o endereco local */
        if (bind(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
        {
           printf("\nErro no bind()!\n");fflush(stdout);
           return(-1);
        }

        /* Indica que o socket escutara as conexões */
        if (listen(sock, MAXPENDING) < 0)
        {
           printf("\nErro no listen()!\n");fflush(stdout);
           return(-1);
        }

    }

    return(sock);
}

int aceitar_conexao(int sock)
{
    int                socket_cliente;
    struct sockaddr_in endereco; /* Endereço Local */
    int                tamanho_endereco;

    /* Define o tamanho do endereço de recepção e envio */
    tamanho_endereco = sizeof(endereco);

    /* Aguarda pela conexão de um cliente */
    if ((socket_cliente = accept(sock, (struct sockaddr *) &endereco, &tamanho_endereco)) < 0)
    {
        printf("\nErro no accept()!\n");fflush(stdout);
        return(0);
    }
    return(socket_cliente);
}

int receber_mensagem(char *mensagem,int sock, int tamanho)
{
    /* Limpar o buffer da mensagem */
    memset((void *) mensagem,(int) NULL,tamanho);

    /* Espera pela recepção de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, tamanho, 0) < 0)
    {
        printf("\nErro na recepção da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Servidor: Recebi (%s)\n",mensagem);fflush(stdout);

    return(0);
}

int enviar_mensagem(char *mensagem,int sock)
{
    /* Devolve o conteúdo da mensagem para o cliente */
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem))
    {
        printf("\nErro no envio da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Servidor: Enviei (%s)\n",mensagem);fflush(stdout);

    return(0);
}




char * auxiliar_servidor(char *mensagem, char *mensagem2){
    int i=0;
    for(i=0;i<TAM_MENSAGEM;i++)
    {
        if(mensagem[i]==';')
            return &mensagem[i+1];
        mensagem2[i] = mensagem[i];
    }
}

int inserir_cliente(char *mensagem2){

    printf("MENSAGEM NA FUNCAO INSERIR %s\n\nMensagem: ", mensagem2);
    char *separador = strchr(mensagem2,';');
    char nome[11], ip[16], porta[6], classe[2];
    int i;
    memset(nome, 0, sizeof(nome));
    memset(ip, 0, sizeof(ip));
    memset(porta, 0, sizeof(porta));
    mensagem2 = auxiliar_servidor(mensagem2, nome);
    mensagem2 = auxiliar_servidor(mensagem2, ip);
    mensagem2 = auxiliar_servidor(mensagem2, porta);
    mensagem2 = auxiliar_servidor(mensagem2, classe);

    printf("Nome: %s, ip: %s, porta: %s, Classe: %s",nome,ip,porta, classe);

   for(i=0;i<N&&l[i]!=NULL;i++);

    printf("Valor i = %d", i);
    l[i]=(LOREM *) malloc(sizeof(LOREM));

    strcpy(l[i]->nome,nome);
    strcpy(l[i]->porta,porta);
    strcpy(l[i]->IP,ip);
    l[i]->classe = atoi(classe);
    l[i]->nivel=1;
    l[i]->vida=100;

    return 1;
}

int remover_cliente(char *mensagem2){
    char nome[11];
    int i=0;
    memset(nome, 0, sizeof(nome));
    auxiliar_servidor(mensagem2, nome);

    for(i=0;i<N;i++){
        if(strcmp(nome,l[i]->nome) == 0)
        {
            free(l[i]);
            l[i]=NULL;
            return 1;
        }

    }
    return 0;

}


int conectar_com_servidor(int sock,char *IP,int porta)
{
    struct sockaddr_in endereco; /* Endereço Local */

    /* Construção da estrutura de endereço do servidor */
    memset(&endereco, 0, sizeof(endereco));   /* Zerar a estrutura */
    endereco.sin_family      = AF_INET;       /* Família de endereçamento da Internet */
    endereco.sin_addr.s_addr = inet_addr(IP); /* Endereço IP do Servidor */
    endereco.sin_port        = htons(porta);  /* Porta do Servidor */

    /* Estabelecimento da conexão com o servidor de echo */
    if (connect(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
    {
        printf("\nErro no connect()!\n");
        fflush(stdout);
        return(-1);
    }
    return(0);
}

void Mensagem_Broadcast(LOREM *lo, char *mensagem){

    int sock;
    int resultado;
    char ip[16], recebimento[10];
    int porta;
    strcpy(ip,lo->IP);
    porta = atoi(lo->porta);
    memset(recebimento,0,sizeof(recebimento));

    if(strcmp("k000;",mensagem) == 0)
        printf("\n\n\nNome -> %s",lo->nome);

        if(mensagem[0]=='h')
        printf("\n\n\nNome -> %s",lo->nome);

        printf("ENVIOU!!");
    sock = criar_socket(0);
    resultado = conectar_com_servidor(sock,ip,porta);
        if (resultado < 0){
            printf("\nErro na conexão com o servidor 2\n");

        }

        printf("Conectou com o servidor!!");
    resultado = enviar_mensagem(mensagem,sock);
        if (resultado < 0){
            printf("\nErro no envio da mensagem\n");
        }


    resultado = receber_mensagem(recebimento,sock,5);

    printf("ENVIOU!!");
        if (resultado < 0)
        {
            printf("\nErro no recebimento da mensagem\n");

        }


                close(sock);



}

void cuida_do_envio_Broadcast(void)
{

    int i=0, clientes,aux,tamanho;
    char **mensagem, concatenada[255],definitiva[255];

    for(clientes=0;i<N;i++)
    {
        if(l[i]!= NULL){
            clientes=clientes+1;
        }
    }
    mensagem = (char **) malloc(sizeof(*mensagem)* clientes);

    for(i=0;i<clientes;i++)
    {
         mensagem[i] = (char *) malloc(sizeof(char)* 34);
    }

    for(i=0,aux=0;i<N;i++)
    {

        if(l[i]!= NULL)
        {
            sprintf(mensagem[aux],"%s;%s;%s;", l[i]->nome,l[i]->IP,l[i]->porta);
            aux++;
        }

    }
    printf("\nNúmero de clientes = %d\n", clientes);

    for(i=0;i<clientes;i++)
        printf("%s",mensagem[i]);

    for(i=0;i<clientes;i++)
    {
        strcat(concatenada,mensagem[i]);
    }

    sprintf(definitiva,"l%03d;%s",strlen(concatenada),concatenada);


    for(i=0;i<N;i++){
        if(l[i]!= NULL){
                printf("i = %d \n",i);
//            Mensagem_Broadcast(l[i],definitiva);
        }
    }
}


void imprime_vetor(void)
{
    int i=0;
    for(i=0;i<N;i++)
    {
        Sleep(1);
        if(l[i]!= NULL)
        {
            printf("\nImprimindo vetor\nPosição: %d\nNome: %s\nIP: %s\nPorta: %s\nNIVEL - %d",i+1,l[i]->nome,l[i]->IP,l[i]->porta, l[i]->nivel);
        }
    }
}

void numjogadores_broadcast(){

int i, definitiva[50];


memset(definitiva,0,sizeof(definitiva));
sprintf(definitiva,"q002;%d;",players);

printf("carai - %s", definitiva);

        for(i=0;i<N;i++){
        if(l[i]!= NULL){
                printf("i = %d \n",i);
                Mensagem_Broadcast(l[i],definitiva);
        }
    }
}

int enviarinimigo(int ini1, int ini2)
{
    char mensagem[100],mensagem1[100], aux[100];
    char n;

    memset(mensagem1,0,sizeof(mensagem1));
    memset(mensagem,0,sizeof(mensagem));
    memset(aux,0,sizeof(aux));



    sprintf(aux,"%s;%d;%s;%s;",l[ini2]->nome,l[ini2]->vida,l[ini2]->IP,l[ini2]->porta);
    n=strlen(aux);
    sprintf(mensagem,"i%03d;%s",n,aux);
    memset(aux,0,sizeof(aux));
    sprintf(aux,"%s;%d;%s;%s;",l[ini1]->nome,l[ini1]->vida,l[ini1]->IP,l[ini1]->porta);
    n=strlen(aux);
    sprintf(mensagem1,"i%03d;%s",n,aux);

    printf("Mensagem de inimigo -> %s", mensagem);
    printf("Mensagem 1 de inimigo -> %s", mensagem1);

    Mensagem_Broadcast(l[ini1],mensagem);
    Mensagem_Broadcast(l[ini2],mensagem1);

    memset(mensagem,0,sizeof(mensagem));

    Sleep(1000);
    sprintf(mensagem,"k000;");

    if(l[ini1]->nivel <= l[ini2]->nivel)
         Mensagem_Broadcast(l[ini1],mensagem);
    else
        Mensagem_Broadcast(l[ini2],mensagem);

}


int atualizadados(char *mensagem)
{
    int i, nivel,vidaa;
    char nome[10], level[10], vida[10];

    memset(nome,0,sizeof(nome));
    memset(nome,0,sizeof(vida));
    memset(level,0,sizeof(level));

    mensagem = auxiliar_servidor(mensagem, nome);

    mensagem = auxiliar_servidor(mensagem, level);
    mensagem = auxiliar_servidor(mensagem, vida);
    nivel = atoi(level);
    vidaa = atoi(vida);

    printf("NOMER -> %s", nome);

    for(i=0;i<N;i++)
    {
        if(strcmp(nome,l[i]->nome)==0)
        {
            l[i]->nivel = nivel;
            l[i]->vida = vidaa;
            break;
        }

    }
    printf("cultimato");
    return 1;
}

int luta()
{
    printf("\n\nENTROU NA LUTA\n\n");
    int vet[4],cu;
    srand(time(NULL));
    cu=rand()%4+1;

    cu = 1; ///PARA TESTE

    switch(cu){
    case 1:
        enviarinimigo(0, cu);
        ///enviarinimigo(2,3); TESTE
        break;

    case 2:
        enviarinimigo(0,cu);
        enviarinimigo(1,3);
        break;

    case 3:
        enviarinimigo(0,cu);
        enviarinimigo(1,2);
        break;
    }



}


void dadonimigo(char *mensagem)
{
    int i;
    char nome[20], danoc[5],aux[30], mensagem2[50];
    int dano;

    memset(nome,0,sizeof(nome));
    memset(danoc,0,sizeof(danoc));
    memset(aux,0,sizeof(aux));
    memset(mensagem2,0,sizeof(mensagem2));

    mensagem = auxiliar_servidor(mensagem,nome);
    mensagem = auxiliar_servidor(mensagem,danoc);

    dano = atoi(danoc);


    for(i=0;i<N;i++)
    {
        if(strcmp(nome,l[i]->nome)==0)
        {
            l[i]->vida -= dano;
            break;
        }
    }

    sprintf(aux,"%d;", dano);
    dano = strlen(aux);
    sprintf(mensagem2,"h%03d;%s",dano,aux);

    Mensagem_Broadcast(l[i],mensagem2);
    Sleep(1000);
    strcpy(mensagem2,"k000;");
    Mensagem_Broadcast(l[i],mensagem2);




}


void *central(void *arg)
{
    char tag[6], mensagem[255], tamen[4]; // tag vai para uma funcao que vai definir o que sera feito
    int i=0, *sock = arg, tam, verificador;
    memset(tag,0,sizeof(tag));
    memset(mensagem,0,sizeof(mensagem));
    memset(tamen,0,sizeof(tamen));
    printf("\n\n\nsocket cliente na fun: %d",sock);
    receber_mensagem(tag,sock,5);

    for(i = 0;i<3;i++)
        tamen[i] = tag[i+1];

    tam = atoi(tamen);
    receber_mensagem(mensagem,sock,tam);

    switch(tag[0])
    {
    case 'c':
        verificador = inserir_cliente(mensagem);
        players = players + 1 ;
        break;
    case 'd':
        dadonimigo(mensagem);
        verificador =2;
        break;
    case 'y':
        verificador = atualizadados(mensagem);
        verificador = 2;
        enviar_mensagem("a000;",sock);
        players = players -1;
        if(players==0)
            luta();

        break;
    }

    if(verificador==1){

        enviar_mensagem("a000;",sock);
        imprime_vetor();
        Sleep(1000);
        numjogadores_broadcast();

        if(players == 0)
            luta();

    }
    else
        enviar_mensagem("n000;",sock);

}




int main()
{
    pthread_t vetthread[30];
    setlocale(LC_ALL,"Portuguese");
    setlorem();
    int arg[30];
    int                sock;                   /* Socket */
    int                socket_cliente;         /* Socket de conexão com o cliente */
    int                resultado;              /* Resultado das funções */
    char               mensagem[TAM_MENSAGEM]; /* Buffer para a recepção da string de echo */
#ifdef WIN
    WORD wPackedValues;
    WSADATA  SocketInfo;
    int      nLastError,
	         nVersionMinor = 1,
	         nVersionMajor = 1;
    wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
    nLastError = WSAStartup(wPackedValues, &SocketInfo);
#endif

    sock = criar_socket(PORTA_SERVIDOR_TCP);
    if (sock < 0)
    {
        printf("\nErro na criação do socket!\n");
        return(1);
    }

    for (;;) /* Loop eterno */
    {
        /* Aguarda por uma conexão e a aceita criando o socket de contato com o cliente */
        socket_cliente = aceitar_conexao(sock);
        if (socket_cliente == 0)
        {
            printf("\nErro na conexao do socket!\n");
            return(1);
        }

        /* Recebe a mensagem do cliente */
        printf("socket cliente na main: %d",socket_cliente);
        arg[0] = socket_cliente;
        pthread_create(&vetthread[0],0,central,(void *)socket_cliente);

        imprime_vetor();






        //close(socket_cliente);    /* Fecha o socket do cliente */
    }
    /*não passa por aqui */
}
