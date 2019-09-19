// No codeblocks inclua no menu em: Project -> Build Options... -> Linker settings -> Other link options -l wsock32
#define WIN // Se não for no windows comente essa linha e compile no terminal: gcc -o tc tc.c
#include <pthread.h>
#include "locale.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
#ifdef WIN
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#define TAM_MENSAGEM 255     /* mensagem de maior tamanho */

#define PORTA_SERVIDOR_TCP 9999
#define PORTA 8889
#define N 30
#define MAXPENDING 5

#include "funcoes.h"

int  sock, foi=0, portainimigo;
int conectados;
char IP[TAM_MENSAGEM];
char IPini[TAM_MENSAGEM];

Personagem *personagem_aux;
Classe_personagem *classe_aux;

typedef struct lorem{
    char nome[20];
    int classe, nivel, vida;

}LOREM;

LOREM *jogador;
int criar_socket(int porta)
{
    int sock;
    struct sockaddr_in endereco; /* Endereço Local */

    /* Criação do socket datagrama/UDP para recepção e envio de pacotes */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {

        fflush(stdout);
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

            fflush(stdout);
            return(-1);
        }

        /* Indica que o socket escutara as conexões */
        if (listen(sock, MAXPENDING) < 0)
        {

            fflush(stdout);
            return(-1);
        }

    }

    return(sock);
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

        fflush(stdout);
        return(-1);
    }
    return(0);
}

int enviar_mensagem(char *mensagem,int sock)
{
    /* Envia o conteúdo da mensagem para o cliente */
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem))
    {

        fflush(stdout);
        return(-1);
    }


    fflush(stdout);

    return(0);
}

int receber_mensagem(char *mensagem,int sock, int tam)
{
    /* Limpar o buffer da mensagem */
    memset((void *) mensagem,(int) NULL,tam);

    /* Espera pela recepção de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, tam, 0) < 0)
    {
        printf("\nErro na recepção da mensagem\n");
        fflush(stdout);
        return(-1);
    }

    fflush(stdout);

    return(0);
}




void primeiramensagem(char *mensagem, LOREM *jogador)
{
    int classe;
    struct  hostent *hostinfo;
    char nome[20], aux[20], aux2[50], c[100], *ip;
    memset(nome,0,sizeof(nome));
    memset(aux2,0,sizeof(aux2));
    memset(aux,0,sizeof(aux));
    memset(mensagem,0,sizeof(mensagem));
    tela_escolhas();
    classe = classe_escolhida;

    strcpy(nome, nome_escolhido);
    jogador->classe = classe;
    strcpy(jogador->nome, nome);

     gethostname(c,100);
        hostinfo=gethostbyname(c);
    ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);

    sprintf(aux,"%s;%s;%d;%d;",nome,ip,PORTA,classe);

    //printf("%d", strlen(aux));
    sprintf(mensagem, "c%03d;%s", strlen(aux),aux);

}


void enviadanoatt(char *nomeini, int dano)
{
    int cu, sock;
    char mensagem[100], aux[80];
    char  mensagemaux[TAM_MENSAGEM];

    sprintf(aux,"%d;",dano);
    cu = strlen(aux);
    sprintf(mensagem,"h%03d;%s",cu,aux);
    sock = criar_socket(0);
    conectar_com_servidor(sock,IPini,portainimigo);
    enviar_mensagem(mensagem,sock);
    receber_mensagem(mensagemaux,sock,5);
    close(sock);



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

void interpretador(int sock)
{

    char tag[10], mensagem[255], porta[6], *aux, tamen[4], nome[11], conects[2], vida[4]; // tag vai para uma funcao que vai definir o que sera feito
    int i=0, tam, verificador;
    memset(tag,0,sizeof(tag));
    memset(conects,0,sizeof(conects));
    memset(porta,0,sizeof(porta));
    memset(nome,0,sizeof(nome));
    memset(mensagem,0,sizeof(mensagem));
    memset(tamen,0,sizeof(tamen));
    memset(vida,0,sizeof(vida));

    receber_mensagem(tag,sock,5);

    if(strcmp(tag,"k000;")==0){
        enviar_mensagem("a000;",sock);
        recebe_int2(1);
    }


    for(i = 0; i<3; i++)
        tamen[i] = tag[i+1];

    tam = atoi(tamen);
    if(tam != 0){
    receber_mensagem(mensagem,sock,tam);
    }


    switch(tag[0])
    {
        case 'a':
            foi=1;
            printf("cu");
            break;
        case 'q':
            printf("%s", mensagem);
            aux = auxiliar_servidor(mensagem, conects);
            conectados = atoi(conects);
            enviar_mensagem("a000;",sock);
            break;

        case 'i':
            memset(nome_inimigo,0,sizeof(nome_inimigo));
            aux = auxiliar_servidor(mensagem, nome_inimigo);
            aux = auxiliar_servidor(aux, vida);
            aux = auxiliar_servidor(aux, IPini);
            aux = auxiliar_servidor(aux, porta);
            vidaini = atoi(vida);
            portainimigo = atoi(porta);
            enviar_mensagem("a000;",sock);
            recebe_int(1);
            break;
        case 'h':
            aux = auxiliar_servidor(mensagem, conects);
            i = atoi(conects);
            jogador->vida -= i;
            enviar_mensagem("a000;",sock);
            vida_atual = vida_atual - i;
            recebe_int2(1);
            break;

        }
        close(sock);

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

        fflush(stdout);
        return(0);
    }

    return(socket_cliente);

}

void intermediaria()
{
    pthread_t vetthread[5];
    int sock,socket_servidor_cliente;
    char *mensagem[255];
    sock = criar_socket(PORTA);
    if (sock < 0)
    {

        return(1);
    }
    while(1)
    {

        socket_servidor_cliente = aceitar_conexao(sock);
        interpretador(socket_servidor_cliente);


    }
    close(sock);

}

void envianivelatt(int lv, int vida)
{
    int cu, sock;
    char mensagem[100], aux[80];
    char  mensagemaux[TAM_MENSAGEM];
    jogador->nivel = lv;
    sprintf(aux,"%s;%d;%d;",jogador->nome,jogador->nivel, vida);
    cu = strlen(aux);
    sprintf(mensagem,"y%03d;%s",cu,aux);

    sock = criar_socket(0);
    conectar_com_servidor(sock,IP,PORTA_SERVIDOR_TCP);
    enviar_mensagem(mensagem,sock);
    receber_mensagem(mensagemaux,sock,5);

    close(sock);


}
int main(int argc, char *argv[])
{
    /* Socket */
    int  resultado;              /* Resultado das funções */
    char mensagem[TAM_MENSAGEM], mensagemaux[TAM_MENSAGEM]; /* Buffer para a recepção da string de echo */
          /* Endereço IP do servidor */



    jogador = (LOREM *) malloc(sizeof(LOREM));

#ifdef WIN
    system("chcp 1252 > nil");
    WORD wPackedValues;
    WSADATA  SocketInfo;
    int      nLastError,
             nVersionMinor = 1,
             nVersionMajor = 1;
    wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
    nLastError = WSAStartup(wPackedValues, &SocketInfo);
#endif

    memset((void *) IP,(int) NULL,TAM_MENSAGEM);
    strcpy(IP,argv[1]);       /* IP Servidor */
    memset((void *) mensagem,(int) NULL,TAM_MENSAGEM);


    sock = criar_socket(0);
    if (sock < 0)
    {

        return(1);
    }

    resultado = conectar_com_servidor(sock,IP,PORTA_SERVIDOR_TCP);
    if (resultado < 0)
    {

        return(1);
    }
    pthread_create(&vetthread[1],0,intermediaria,NULL);


    while(foi==0)
    {
        primeiramensagem(mensagem, jogador);

        resultado = enviar_mensagem(mensagem,sock);
        receber_mensagem(mensagemaux,sock, 6);
        if(strcmp(mensagemaux,"a000;")==0){
           foi = 1;
           }
        if (resultado < 0)
        {

            return(1);
        }
    }
    while(1){
    if(conectados == 2){
    pthread_create(&vetthread[2],0,tempo,NULL);
    pthread_create(&vetthread[3],0,loopExecucao,NULL);
    pthread_join(vetthread[2], NULL);
    pthread_join(vetthread[3], NULL);
    conectados = 0;
    }
    }


    ///Conexão com servidor feita



    close(sock); ///fecha depois

    return(0);
}

