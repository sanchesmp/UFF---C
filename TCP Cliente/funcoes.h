#define linhas 41
#define colunas 101
#define HORA 30

char matrizmapa[linhas][colunas];
char janela[11][24];
int Xatual = 35, Yatual = 22, Xantigo = 35, Yantigo = 22, conta_cliques = 0, classe_escolhida, vida_atual, mana_atual, xp_recebido, mapaatual = 1;
int Xguarda, Yguarda;
char nome_escolhido[20];
char nome_inimigo[20];
int vidaini = 1;
int segun = 0;
int minut = 0;
int seta = 0, posso_jogar = 0;
pthread_t vetthread[10];
int nivel;

typedef struct inimigo
{
    int codigo_inimigo;
    char nome[20];
    int vida;
    int xp;
    int dano;
} Inimigo;

typedef struct personagem
{
    char nome[20];
    int vida;
    int mana;
    int classe;
    int xp;
    int nivel;
    struct personagem *prox;
} Personagem;

typedef struct classe_personagem
{
    int codigo_classe;
    char nome[50];
    char skill[4][20];
    int dano_skill[4];
    int custo_mana[4];
    int atributo_de_vida;
} Classe_personagem;
Personagem *inserirpersonagemjogador(Personagem *jogador, int nome, int nivel, int mana, int vida, int xp)
{
    Personagem *novo=(Personagem*)malloc(sizeof(Personagem));
    Personagem*p=jogador;
//   strcpy(novo->nome,nome);
    novo->nivel=nivel;
    novo->mana=mana;
    novo->vida=vida;
    novo->xp=xp;
    if(jogador==NULL)
    {
        jogador=novo;
        return jogador;
    }
    for(p=jogador; p!=NULL; p->prox);

    p->prox=novo;
    return jogador;
}


int tela_escolhas()
{
    FILE *escolhas;
    escolhas = fopen("arquivos/escolhas.txt", "r");
    int c, continua = 1, novo = 0, carregar = 0, apagar = 0;
    char setinha = '>';
    for(int i = 0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            fscanf(escolhas, "%c", &matrizmapa[i][j]);
        }
    }
    fclose(escolhas);
    printa();
    fclose(escolhas);
    char cu;

    pegaTecla();
    cudetela();
}

int cudetela()
{
    gotoxy(100,100);
    printf("cu");
    FILE *escolhas;
    char setinha = '>';
    int continua = 1;
    Xatual = 35, Yatual = 22, Xantigo = 35, Yantigo = 22;
    escolhas = fopen("arquivos/cria.txt", "r");
    system("CLS");
    for(int i = 0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            fscanf(escolhas, "%c", &matrizmapa[i][j]);
        }
    }
    fclose(escolhas);
    printa();
    char c;
    while(continua == 1)
    {


        gotoxy(Xatual, Yatual);
        printf("%c", setinha);
        c = pegaTecla();

        switch(c)
        {
        case 's':
            Yantigo = Yatual;
            Xantigo = Xatual;
            Yatual ++;
            atualizaTela();
            gotoxy(Xatual, Yatual);
            printf("%c", setinha);
            break;
        case 'w':
            Yantigo = Yatual;
            Xantigo = Xatual;
            Yatual --;
            atualizaTela();
            gotoxy(Xatual, Yatual);
            printf("%c", setinha);
            break;
        }
        if((matrizmapa[Yatual][Xatual]) != ' ')
        {
            Xatual = Xantigo;
            Yatual = Yantigo;
        }

        if(c == 'l')
        {
            if(Yatual == 22)
            {
                classe_escolhida = 0;
            }
            if(Yatual == 23)
            {
                classe_escolhida = 1;
            }
            if(Yatual == 24)
            {
                classe_escolhida = 2;
            }
            if(Yatual == 25)
            {
                classe_escolhida = 3;
            }
            gotoxy(31, 31);
            scanf("%s", &nome_escolhido);
            carregaMapa();
            printa();

            continua = 0;

        }
    }


}




int ver_se_tem_inimigo()
{
    int tem = 0, tem2;
    srand((time(NULL)*conta_cliques*Xatual)/(Yatual));

    tem = rand()%101;
    if(tem >= 97)
    {
        if(mapaatual == 1)
        {
            return  rand()%5 + 1;
        }

        if(mapaatual == 2)
        {
            return  rand()%5 + 5;
        }

        if(mapaatual == 3)
        {
            return  rand()%5 + 10;
        }


    }
    else
    {
        return 0;
    }

}

int printa_inimigo(int verdade)
{

    FILE *inimigo;
    char c, v[100];

    switch(verdade)
    {
    case 1:
        inimigo = fopen("inimigos/esquilo.txt", "r");
        system("COLOR 06");
        break;
    case 2:
        inimigo = fopen("inimigos/lobo.txt", "r");
        system("COLOR 08");
        break;
    case 3:
        inimigo = fopen("inimigos/cobra.txt", "r");
        system("COLOR 02");
        break;
    case 4:
        inimigo = fopen("inimigos/rola.txt", "r");
        system("COLOR 05");
        break;
    case 5:
        inimigo = fopen("inimigos/aranha.txt", "r");
        system("COLOR 0C");
        break;
    case 6:
        inimigo = fopen("inimigos/Cavaleiro.txt", "r");
        system("COLOR 0C");
        break;
    case 7:
        inimigo = fopen("inimigos/Ciclope.txt", "r");
        system("COLOR 0C");
        break;
    case 8:
        inimigo = fopen("inimigos/Mrdeath.txt", "r");
        system("COLOR 0C");
        break;
    case 9:
        inimigo = fopen("inimigos/orc.txt", "r");
        system("COLOR 0C");
        break;

    case 10:
        inimigo = fopen("inimigos/mamilo.txt", "r");
        system("COLOR 0C");
        break;

    case 11:
        inimigo = fopen("inimigos/demonio.txt", "r");
        system("COLOR 0C");
        break;

    case 12:
        inimigo = fopen("inimigos/demonio02.txt", "r");
        system("COLOR 0C");
        break;

    case 13:
        inimigo = fopen("inimigos/demonio03.txt", "r");
        system("COLOR 0C");
        break;

    case 14:
        inimigo = fopen("inimigos/demonio.txt", "r");
        system("COLOR 0C");
        break;

    case 15:
        inimigo = fopen("inimigos/demonio03.txt", "r");
        system("COLOR 0C");
        break;
    case 16:
        inimigo = fopen("inimigos/player.txt", "r");
        system("COLOR 0C");
        break;

    }
    system("CLS");
    do
    {
        c = getc(inimigo);
        printf("%c", c);
    }
    while(c != EOF);
    Sleep(1000);
}

void carregaSkills(Classe_personagem classe)
{
    FILE *tela_combate;
    tela_combate = fopen("janelinha.txt", "r");
    if (tela_combate == NULL)
    {
        printf ("Houve um erro ao abrir o arquivo de tela de combate.\n");
        return 1;
    }
    for(int i = 0; i< 15; i++)
    {
        for(int j = 0; j < 24; j++)
        {
            fscanf(tela_combate, "%c", &janela[i][j]);
        }
    }
    fclose(tela_combate);

    for(int i = 0; i< 15; i++)
    {
        for(int j = 0; j < 24; j++)
        {
            int l = 0;
            if(janela[i][j] == '0')
            {
                for(int k = j; k< 20; k++)
                {
                    janela[i][k] = classe.skill[0][l];
                    l++;
                }
            }

            if(janela[i][j] == '1')
            {
                for(int k = j; k< 20; k++)
                {
                    janela[i][k] = classe.skill[1][l];
                    l++;
                }
            }

            if(janela[i][j] == '2')
            {
                for(int k = j; k< 20; k++)
                {
                    janela[i][k] = classe.skill[2][l];
                    l++;
                }
            }
            if(janela[i][j] == '3')
            {
                for(int k = j; k < 20; k++)
                {
                    janela[i][k] = classe.skill[3][l];
                    l++;
                }
            }
        }
    }



}
void printa_skills()
{
    for(int i = 0; i< 15; i++)
    {
        for(int j = 0; j < 24; j++)
        {
            printf("%c", janela[i][j]);
        }
    }

}

int combate(Inimigo inimigs, Personagem persona, Classe_personagem classe)
{
    Xguarda = Xatual;
    Yguarda = Yatual;
    Xatual = 1;
    Yatual = 27;
    Xantigo = 1;
    Yantigo = 27;

    while(inimigs.vida > 0 &&  persona.vida>0)
    {
        char seta = '>';
        gotoxy(120, 3);
        printf("%s vida: %d HP", inimigs.nome, inimigs.vida);

        gotoxy(120, 5);
        printf("%s vida: %d HP Nivel %3d", persona.nome, persona.vida, persona.nivel);
        gotoxy(120, 6);
        printf("  mana: %d GAS", persona.mana);
        gotoxy(1,25);
        printa_skills();
        gotoxy(Xatual,Yatual);
        printf("%c", seta);
        switch(pegaTecla())
        {
        case 's':
            Yantigo = Yatual;
            Yatual = Yatual + 2;
            break;
        case 'w':
            Yantigo = Yatual;
            Yatual = Yatual - 2;
            break;
        }
        if((matrizmapa[Yatual][Xatual]) != ' ')
        {
            Yatual = Yantigo;
        }
        gotoxy(Xatual, Yatual);
        printf("%c", seta);
        atualizaTela();

        if(pegaTecla() == 'l')
        {
            if(Yatual == 27)
            {
                inimigs.vida = inimigs.vida - classe.dano_skill[0];
                persona.mana = persona.mana - classe.custo_mana[0] - persona.nivel;
                persona.vida = persona.vida - inimigs.dano;
            }
            if(Yatual == 29 &&  (persona.mana >= classe.custo_mana[1]))
            {
                inimigs.vida = inimigs.vida - classe.dano_skill[1] - persona.nivel;
                persona.mana = persona.mana - classe.custo_mana[1];
                persona.vida = persona.vida - inimigs.dano;
            }
            if(Yatual == 31  && (persona.mana>= classe.custo_mana[2]))
            {
                inimigs.vida = inimigs.vida - classe.dano_skill[2] - persona.nivel;
                persona.mana = persona.mana - classe.custo_mana[2];
                persona.vida = persona.vida - inimigs.dano;
            }
            if(Yatual == 33  && persona.mana >= classe.custo_mana[3] )
            {
                inimigs.vida = inimigs.vida - classe.dano_skill[3] - persona.nivel;
                persona.mana = persona.mana - classe.custo_mana[3];
                persona.vida = persona.vida - inimigs.dano;
            }

        }


    }

    vida_atual = persona.vida;

    mana_atual = persona.mana;
    xp_recebido = xp_recebido + inimigs.xp;
    Xatual = Xguarda;
    Yatual = Yguarda;

    printa();
}




void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int carregaMapa()
{
    int c;
    FILE *mapa;
    if(mapaatual == 1)
    {
        mapa = fopen("mapa.txt", "r");
        system("COLOR 0A");
    }

    if(mapaatual == 2)
    {
        mapa = fopen("mapa2.txt", "r");
        system("COLOR FC");
    }

    if(mapaatual == 3)
    {
        mapa = fopen("mapa3.txt", "r");
        system("COLOR FC");
    }

    if(mapaatual == 4)
    {
        mapa = fopen("mapa4.txt", "r");
        system("COLOR FC");
    }

    if (mapa == NULL)
    {
        printf ("Houve um erro ao abrir o arquivo de mapa.\n");
        return 1;
    }

    for(int i = 0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            fscanf(mapa, "%c", &matrizmapa[i][j]);
        }
    }
    fclose(mapa);

}

void printa()
{
    system("CLS");
    for(int i = 0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            printf("%c", matrizmapa[i][j]);
        }
    }

}
int njmera = 0;
int pegaTecla()
{
    fflush(stdin);
    unsigned char uchTecla;
    uchTecla = getch();
//) != 0x1b) escin
    return uchTecla;
}

void atualizaTela()
{
    gotoxy(Xantigo, Yantigo);
    printf("%c", matrizmapa[Yantigo][Xantigo]);
}


void tempo()
{
    segun = 0;
    while(segun<=HORA)
    {
        segun = segun +2;
        Sleep(2000);
        gotoxy(150,5);
        printf("%d", segun);
    }
}
int passa_nivel(int x)
{
    return x;
}
int divs(float a,float b)
{

int c = a/b;
return c;
}
void tocaFear(){
    printf(" Musica : Fear of the Dark - Iron Maiden ");
int n=1;
while(n <=2){
printf(" Musica : Fear of the Dark - Iron Maiden ");
Beep(293,250);
Beep(293,250);
Beep(220,250);
Beep(220,250);
Beep(293,250);
Beep(293,250);
Beep(330,250);
Beep(330,250);
Beep(349,250);
Beep(349,250);
Beep(330,250);
Beep(330,250);
Beep(293,250);
Beep(293,250);
Beep(330,250);
Beep(262,250);
Beep(262,250);
Beep(196,250);
Beep(196,250);
Beep(262,250);
Beep(262,250);
Beep(293,250);
Beep(293,250);
Beep(330,250);
Beep(330,250);
Beep(293,250);
Beep(262,250);
Beep(262,250);
Beep(330,250);
Beep(262,250);
n++;
}
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(220,1.33),250);
Beep(divs(220,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(349,1.33),250);
Beep(divs(349,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(196,1.33),250);
Beep(divs(196,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(262,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(220,1.33),250);
Beep(divs(220,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(349,1.33),250);
Beep(divs(349,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(293,1.33),250);
Beep(divs(330,1.33),250);
Beep(divs(293,1.33),250);
Beep(175,250);
Beep(175,250);
Beep(131,250);
Beep(131,250);
Beep(175,250);
Beep(175,250);
Beep(175,250);
Beep(196,250);
Beep(196,250);
Beep(220,250);
Beep(220,250);
Beep(196,250);
Beep(196,250);
Beep(175,250);
Beep(175,250);
Beep(220,500);
Beep(175,500);
}

int telaPvP(char *jogador1nome, int vida1, char *jogador2nome, int vida2, Personagem persona, Classe_personagem classe)
{
    Xguarda = Xatual;
    Yguarda = Yatual;
    Xatual = 1;
    Yatual = 27;
    Xantigo = 1;
    Yantigo = 27;
    while(1){
    if(vida1 > 0 &&  vida2>0)
    {
        char seta = '>';
        gotoxy(Xatual,Yatual);
        printf("%c", seta);
        switch(pegaTecla())
        {
        case 's':
            Yantigo = Yatual;
            Yatual = Yatual + 2;
            break;
        case 'w':
            Yantigo = Yatual;
            Yatual = Yatual - 2;
            break;
        }
        if((matrizmapa[Yatual][Xatual]) != ' ')
        {
            Yatual = Yantigo;
        }
        gotoxy(Xatual, Yatual);
        printf("%c", seta);
        atualizaTela();
        if(pegaTecla() == 'l')
        {
            if(Yatual == 27)
            {
                persona.mana = persona.mana - classe.custo_mana[0] - persona.nivel;
                vidaini = vidaini - classe.dano_skill[0];
                posso_jogar = 0;
                enviadanoatt(nome_inimigo, classe.dano_skill[0]);
                return 0;
            }
            if(Yatual == 29 &&  (persona.mana >= classe.custo_mana[1]))
            {
                persona.mana = persona.mana - classe.custo_mana[1];
                vidaini = vidaini - classe.dano_skill[1];
                posso_jogar = 0;
                enviadanoatt(nome_inimigo, classe.dano_skill[1]);
                return 0;
            }
            if(Yatual == 31  && (persona.mana>= classe.custo_mana[2]))
            {
                persona.mana = persona.mana - classe.custo_mana[2];
                vidaini = vidaini - classe.dano_skill[2];
                posso_jogar = 0;
                enviadanoatt(nome_inimigo, classe.dano_skill[2]);
                return 0;
            }
            if(Yatual == 33  && persona.mana >= classe.custo_mana[3] )
            {
                persona.mana = persona.mana - classe.custo_mana[3];
                vidaini = vidaini - classe.dano_skill[3];
                posso_jogar = 0;
                enviadanoatt(nome_inimigo, classe.dano_skill[3]);
                return 0;
            }

        }
    }
    else
    {
        return -1;

    }

}}
int volta_vida(int m){

}
int recebe_int(int m)
{
    seta = m;
}
int recebe_int2(int m)
{
    posso_jogar = m;
}
void PvP(char *jogador1nome, int vida1, char *jogador2nome, int vida2, Personagem persona, Classe_personagem classe2)
{
    gotoxy(120, 3);
    printf("%s vida: %d HP", jogador2nome, vida2);
    gotoxy(120, 5);
    printf("%s vida: %d HP", jogador1nome, vida1);
    gotoxy(120, 6);
    printf("  mana: %d GAS", persona.mana);
    gotoxy(1,25);
    printa_skills();
}


int loopExecucao()
{

    if(njmera!=0)
    {
        Personagem *Ppersonagem;
        Ppersonagem=(Personagem*)malloc(sizeof(Personagem));
        Ppersonagem=inserirpersonagemjogador(Ppersonagem, 5, 1, 10, 10, 0);
        njmera ++;
    }
//struct dos inimigos

    Inimigo nomes_inimigos[15];
    nomes_inimigos[1].codigo_inimigo = 1;
    strcpy(nomes_inimigos[1].nome, "Esquilo");
    nomes_inimigos[1].vida = 5;
    nomes_inimigos[1].xp = 3;
    nomes_inimigos[1].dano = 2;

    nomes_inimigos[2].codigo_inimigo = 2;
    strcpy(nomes_inimigos[2].nome, "Lobo");
    nomes_inimigos[2].vida = 7;
    nomes_inimigos[2].xp = 5;
    nomes_inimigos[2].dano = 5;

    nomes_inimigos[3].codigo_inimigo = 3;
    strcpy(nomes_inimigos[3].nome, "Cobra");
    nomes_inimigos[3].vida = 5;
    nomes_inimigos[3].xp = 3;
    nomes_inimigos[3].dano = 4;

    nomes_inimigos[4].codigo_inimigo = 4;
    strcpy(nomes_inimigos[4].nome, "Popeye");
    nomes_inimigos[4].vida = 15;
    nomes_inimigos[4].xp = 15;
    nomes_inimigos[4].dano = 15;

    nomes_inimigos[5].codigo_inimigo = 5;
    strcpy(nomes_inimigos[5].nome, "Aranha");
    nomes_inimigos[5].vida = 3;
    nomes_inimigos[5].xp = 2;
    nomes_inimigos[5].dano = 1;

    nomes_inimigos[6].codigo_inimigo = 6;
    strcpy(nomes_inimigos[6].nome, "Cavaleiro");
    nomes_inimigos[6].vida = 10;
    nomes_inimigos[6].xp = 9;
    nomes_inimigos[6].dano = 8;

    nomes_inimigos[7].codigo_inimigo = 7;
    strcpy(nomes_inimigos[7].nome, "Cleber");
    nomes_inimigos[7].vida = 12;
    nomes_inimigos[7].xp = 10;
    nomes_inimigos[7].dano = 10;


    nomes_inimigos[8].codigo_inimigo = 8;
    strcpy(nomes_inimigos[8].nome, "Mr death");
    nomes_inimigos[8].vida = 15;
    nomes_inimigos[8].xp = 12;
    nomes_inimigos[8].dano = 9;

    nomes_inimigos[9].codigo_inimigo = 9;
    strcpy(nomes_inimigos[9].nome, "Joelson ");
    nomes_inimigos[9].vida = 10;
    nomes_inimigos[9].xp = 8;
    nomes_inimigos[9].dano = 7;

    nomes_inimigos[10].codigo_inimigo = 10;
    strcpy(nomes_inimigos[10].nome, "Tetinha");
    nomes_inimigos[10].vida = 10;
    nomes_inimigos[10].xp = 8;
    nomes_inimigos[10].dano = 0;

    nomes_inimigos[11].codigo_inimigo = 11;
    strcpy(nomes_inimigos[11].nome, "Jurema");
    nomes_inimigos[11].vida = 12;
    nomes_inimigos[11].xp = 8;
    nomes_inimigos[11].dano = 9;

    nomes_inimigos[12].codigo_inimigo = 12;
    strcpy(nomes_inimigos[12].nome, "Robson");
    nomes_inimigos[12].vida = 15;
    nomes_inimigos[12].xp = 12;
    nomes_inimigos[12].dano = 10;

    nomes_inimigos[13].codigo_inimigo = 13;
    strcpy(nomes_inimigos[13].nome, "Bevaudo");
    nomes_inimigos[13].vida = 15;
    nomes_inimigos[13].xp = 15;
    nomes_inimigos[13].dano = 12;

    nomes_inimigos[14].codigo_inimigo = 14;
    strcpy(nomes_inimigos[14].nome, "Bevaudo pistola");
    nomes_inimigos[14].vida = 20;
    nomes_inimigos[14].xp = 15;
    nomes_inimigos[14].dano = 15;

    nomes_inimigos[15].codigo_inimigo = 15;
    strcpy(nomes_inimigos[15].nome, "Jailson Mendes");
    nomes_inimigos[15].vida = 10;
    nomes_inimigos[15].xp = 8;
    nomes_inimigos[15].dano = 7;




    Classe_personagem classes[4];
    classes[0].codigo_classe = 0;
    strcpy(classes[0].nome, "Guerreiro");
    strcpy(classes[0].skill[0], "Pancada       ");
    classes[0].dano_skill[0] = 1;
    classes[0].custo_mana[0] = 0;
    strcpy(classes[0].skill[1], "Dilacerar     ");
    classes[0].dano_skill[1] = 3;
    classes[0].custo_mana[1] = 1;
    strcpy(classes[0].skill[2], "Carga Furiosa ");
    classes[0].dano_skill[2] = 5;
    classes[0].custo_mana[2] = 2;
    strcpy(classes[0].skill[3], "Executar      ");
    classes[0].dano_skill[3] = 10;
    classes[0].custo_mana[3] = 3;
    classes[0].atributo_de_vida = 10;

    classes[1].codigo_classe = 1;
    strcpy(classes[1].nome, "Mago");
    strcpy(classes[1].skill[0], "Cajadada      ");
    classes[1].dano_skill[0] = 1;
    classes[1].custo_mana[0] = 0;
    strcpy(classes[1].skill[1], "Seta de Fogo  ");
    classes[1].dano_skill[1] = 3;
    classes[1].custo_mana[1] = 3;
    strcpy(classes[1].skill[2], "Meteoro       ");
    classes[1].dano_skill[2] = 7;
    classes[1].custo_mana[2] = 7;
    strcpy(classes[1].skill[3], "Bola de Fogo  ");
    classes[1].dano_skill[3] = 12;
    classes[1].custo_mana[3] = 10;
    classes[1].atributo_de_vida = 6;


    classes[2].codigo_classe = 2;
    strcpy(classes[2].nome, "Druida");
    strcpy(classes[2].skill[0], "Patada        ");
    classes[2].dano_skill[0] = 2;
    classes[2].custo_mana[0] = 0;
    strcpy(classes[2].skill[1], "Mordida       ");
    classes[2].dano_skill[1] = 3;
    classes[2].custo_mana[1] = 1;
    strcpy(classes[2].skill[2], "Fogo Lunar    ");
    classes[2].dano_skill[2] = 4;
    classes[2].custo_mana[2] = 3;
    strcpy(classes[2].skill[3], "Fogo Solar    ");
    classes[2].dano_skill[3] = 9;
    classes[2].custo_mana[3] = 4;
    classes[2].atributo_de_vida = 15;

    classes[3].codigo_classe = 3;
    strcpy(classes[3].nome, "Cacador");
    strcpy(classes[3].skill[0], "Bater com Arco");
    classes[3].dano_skill[0] = 1;
    classes[3].custo_mana[0] = 0;
    strcpy(classes[3].skill[1], "Atirar Flecha ");
    classes[3].dano_skill[1] = 3;
    classes[3].custo_mana[1] = 1;
    strcpy(classes[3].skill[2], "Dardo com coco");
    classes[3].dano_skill[2] = 5;
    classes[3].custo_mana[2] = 3;
    strcpy(classes[3].skill[3], "Flecha de Fogo");
    classes[3].dano_skill[3] = 10;
    classes[3].custo_mana[3] = 7;
    classes[3].atributo_de_vida = 7;


    Personagem personagem_criado;
    strcpy(personagem_criado.nome, nome_escolhido);
    personagem_criado.nivel = 1;
    personagem_criado.mana = 10+(personagem_criado.nivel*3);
    personagem_criado.vida = classes[classe_escolhida].atributo_de_vida * (personagem_criado.nivel) + 50;
    personagem_criado.xp = 0;
     char musica[50];
    carregaSkills(classes[classe_escolhida]);
    int continua = 1;
    char personagem = '0';
    while(continua == 1)
    {
        if(segun >= HORA && segun <9999)
            {
                envianivelatt(personagem_criado.nivel,vida_atual);

                segun = 0;
                minut = 0;
            }
        if(seta == 1)
        {
            printa_inimigo(16);
            segun = 1000000;
            PvP(nome_escolhido, vida_atual, nome_inimigo, vidaini, personagem_criado, classes[classe_escolhida]);

            if(posso_jogar == 1)
            {
                PvP(nome_escolhido, vida_atual, nome_inimigo, vidaini, personagem_criado, classes[classe_escolhida]);
                telaPvP(nome_escolhido, vida_atual, nome_inimigo, vidaini, personagem_criado, classes[classe_escolhida]);
                if(vidaini<=0){
                    system("CLS");
                    printf("TU EH PHODA, TU GANHASTE");
                    tocaFear();
                    exit(1);
                }
                if(vida_atual <= 0){
                    system("CLS");
                    printf("VOCE PERDEU(PERDI)");
                    ///MUSICA AQUI
                    Sleep(60000);
                    exit(1);
                }

            }
            while(posso_jogar ==0);
        }
        else
        {

            gotoxy(Xatual, Yatual);
            printf("%c", personagem);

            switch(pegaTecla())
            {
            case 'a':
                Xantigo = Xatual;
                Yantigo = Yatual;
                Xatual --;
                break;
            case 'd':
                Xantigo = Xatual;
                Yantigo = Yatual;
                Xatual ++;
                break;
            case 's':
                Yantigo = Yatual;
                Xantigo = Xatual;
                Yatual ++;
                break;
            case 'w':
                Yantigo = Yatual;
                Xantigo = Xatual;
                Yatual --;
                break;
            }
            if((matrizmapa[Yatual][Xatual]) != ' ' && (matrizmapa[Yatual][Xatual])!= '>')
            {
                Xatual = Xantigo;
                Yatual = Yantigo;
            }
            if((matrizmapa[Yatual][Xatual]) == '>')
            {
                Xatual = 2;
                Yatual = 25;
                mapaatual ++;
                carregaMapa();
                printa();

            }
            if((matrizmapa[Yatual][Xatual]) == '<')
            {
                Xatual = 98;
                Yatual = 25;
                mapaatual --;
                carregaMapa();
                printa();

            }


            int x = ver_se_tem_inimigo();
            if(x != 0)
            {
                printa_inimigo(x);
                combate(nomes_inimigos[x], personagem_criado, classes[classe_escolhida]);
                personagem_criado.xp = xp_recebido;
                if(xp_recebido >= 7)
                {
                    xp_recebido = 0;
                    personagem_criado.nivel ++;
                    personagem_criado.vida += 10;
                }
                personagem_criado.mana = 10+(personagem_criado.nivel*3);
                personagem_criado.vida = classes[classe_escolhida].atributo_de_vida * (personagem_criado.nivel) + 50;
                if(vida_atual > 0)
                {
                    vida_atual = classes[classe_escolhida].atributo_de_vida*(personagem_criado.nivel)+50;
                    mana_atual = 10+(personagem_criado.nivel*3);
                }
                else
                {
                    system("CLS");
                    printf("Voce morreu");
                    usleep(500000);

                    tela_escolhas();
                }
            }

        conta_cliques ++;
        atualizaTela();

    }}
}


