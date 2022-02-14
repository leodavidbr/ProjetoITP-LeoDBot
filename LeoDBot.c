#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 50
//  Exemplo para saída no terminal:
//  fprintf(stderr, "Meu id = %s\n", myId);


void 
leitura(int a, int l, int matriz[a][l][5], int minha_posicao[2], char myId[MAX_STR]){
// Limpeza das posicoes dos bots
    for (int i = 0; i < a; i++)
        for (int j = 0; j < l; j++)
                matriz[i][j][3] = 0;
// LER E FORMATAR ARRAY --OK
    int temp, qntBots, ABot, BBot;
    char id_outro_bot[MAX_STR];

    for (int i = 0; i < a; i++) {   
        for (int j = 0; j < l; j++) 
        {
        scanf("%d", &temp);
        //formatacao
        if(temp == 1)
        {
            // is porto
            matriz[i][j][2] = 1;
        }
        else if(temp == 0 )
        {
            // Caso o tipo de peixe ainda apareca, depois de zerar a area, modificar a condicao --OK
            // sem peixe
            matriz[i][j][0]= 0;
            matriz[i][j][1]= 0;
        }
        else
        {
            matriz[i][j][0] = temp / 10;
            matriz[i][j][1] = temp % 10;
        }
        }
    }

// LER COORDENADAS DOS BOTS --OK
scanf(" BOTS %d", &qntBots);
for (int i = 0; i < qntBots; i++)
{
    scanf(" %s %d %d", id_outro_bot, &ABot, &BBot); //Verificar espaco no %s --OK
    if(strcmp(id_outro_bot, myId) == 0){
        // Guardando separado a posicao do meu bot
        minha_posicao[0] = ABot;
        minha_posicao[1] = BBot;
        matriz[ABot][BBot][3] = 2;
    }
    else if(matriz[ABot][BBot][2] != 1){ // Nao registro quem esta no porto
        matriz[ABot][BBot][3] = 1;
    }
}


}

int preco(int peixe){
    //!!!!!! ficar de olho
    if (peixe == 0)
        return 0;
    if (peixe == 1)
        return 100;
    if (peixe == 2)
        return 150;
    if (peixe == 3)
        return 200;
    else{
        fprintf(stderr, "ERRO: TENTANDO VER TIPO DE PEIXE QUE NAO EXISTE: %d\n", peixe);
        return 0;
    } 
}

int recursao_local(int a, int l,int matriz[a][l][5],int minha_posicao[2], int destino[2], int porto, int qual_opcao,int opcoes[350][2], int raio){
int interacao = 0;

for (int i = 0; i < a; i++)
        for (int j = 0; j < l; j++)
        {
            matriz[i][j][4] = abs(minha_posicao[0] - i) + abs(minha_posicao[1] - j);
            // Criterios pra ser um possivel destino
            if (porto == 1)
            {
                if(matriz[i][j][2] == 1)
                {
                    opcoes[interacao][0] = i;
                    opcoes[interacao][1] = j;
                    interacao += 1;
                }
                continue;
            }
            else if (matriz[i][j][4] < raio && matriz[i][j][3] == 0 && (i != 0 || j != 0))
            {
                opcoes[interacao][0] = i;
                opcoes[interacao][1] = j;
                interacao += 1;
            }
        }

    if (porto == 1)
        return interacao;

    if (interacao < 4)
        return recursao_local(a, l, matriz, minha_posicao, destino, porto, qual_opcao, opcoes, raio + 2);
    return interacao;
}

void melhor_local(int a, int l,int matriz[a][l][5],int minha_posicao[2], int destino[2], int porto, int qual_opcao){
int opcoes[350][2], interacao = 0, temp0, temp1, criterio0, criterio1;

    interacao = recursao_local(a, l, matriz, minha_posicao, destino, porto, qual_opcao, opcoes, 4);// O ultimo e o raio da primeira busca
    
    for (int i = 0; i < interacao-1; i++)
        for (int j = 0; j < interacao-i-1; j++)
        {
            //criterio
            if (porto == 0)
            {
            // criterio para pesca
            criterio0 = matriz[opcoes[j][0]][opcoes[j][1]][1] * preco(matriz[opcoes[j][0]][opcoes[j][1]][0]);
            criterio1 = matriz[opcoes[j+1][0]][opcoes[j+1][1]][1] * preco(matriz[opcoes[j+1][0]][opcoes[j+1][1]][0]);
            }
            else
            {
            // Criterio para portos (menor, melhor)
            criterio0 = matriz[opcoes[j][0]][opcoes[j][1]][4] * -1;
            criterio1 = matriz[opcoes[j+1][0]][opcoes[j+1][1]][4] * -1;
            }

            if (criterio0 < criterio1){
                temp0 = opcoes[j][0];
                temp1 = opcoes[j][1];
                opcoes[j][0] = opcoes[j+1][0];
                opcoes[j][1] = opcoes[j+1][1];
                opcoes[j+1][0] = temp0; 
                opcoes[j+1][1] = temp1;
            }
        }
    destino[0] = opcoes[qual_opcao][0];
    destino[1] = opcoes[qual_opcao][1];
    // Aqui normalmente vai ser o melhor local
    // Se a funcao andar chamar com outro parametro por causa de obstaculo entao bora
}


char andar(int a, int l,int matriz[a][l][5],int minha_posicao[2], int destino[2], int porto, int tentativa){
    int direcao[2], identidade[2];
    direcao[0] = destino[0] - minha_posicao[0];
    direcao[1] = destino[1] - minha_posicao[1];
    if(direcao[0]> 0)
        identidade[0] = 1;
    else if(direcao[0]< 0)
        identidade[0] = -1;
    else 
        identidade[0] = 0;

    if(direcao[1]> 0)
        identidade[1] = 1;
    else if(direcao[1]< 0)
        identidade[1] = -1;
    else 
        identidade[1] = 0;

    //Se a distancia vertical eh maior
    if(abs(direcao[0]) > abs(direcao[1])){
        // Se pra onde eu quero me mover nao tem gente
        if(matriz[minha_posicao[0]+ identidade[0]][minha_posicao[1]][3] != 1){
            if(direcao[0] > 0){
                return 'd';
            }
            else 
                return 'u';
        }//Se eu quero  e posso me mover pro outro lado 
        else if (identidade[1] !=0 && matriz[minha_posicao[0]][minha_posicao[1]+ identidade[1] ][3] != 1)
        {
            if(direcao[1] > 0){ // mexi no [1]
                return 'r';
            }
            else 
                return 'l';
        }
    }
    if(1)
    {
        if(matriz[minha_posicao[0]][minha_posicao[1]+ identidade[1] ][3] != 1 && identidade[1] != 0){
            if(direcao[1] > 0){
                return 'r';
            }
            else 
                return 'l';
        }
        else if (identidade[0] !=0 && matriz[minha_posicao[0]+ identidade[0]][minha_posicao[1]][3] != 1)
        {
            if(direcao[0] > 0){
                return 'd';
            }
            else 
                return 'u';
        }
    }
    // O que fazer se tiver obstaculo ruim de desviar 
    // Esperar, dar a volta, ou [[[procurar outro canto]]].
    // Provavelmente vou chamar a funcao melhor lugar modificada aqui e depois a funcao andar. (isso mesmo)
    tentativa += 1;
    melhor_local(a, l, matriz, minha_posicao, destino, porto, tentativa); // destino vira o segundo melhor lugar
    return andar(a, l, matriz, minha_posicao, destino, porto, tentativa);
    // muita coisa pode dar errado aqui, tenho que ficar de olho
    // o maior problema eh encontrar alguem saindo do meu porto destino, mas so tentar desviar pode ser dificl (se ele for por mesmo canto...)
    fprintf(stderr, "ERRO: ENCONTREI UM OBSTACULO\n");
}

int devo_pescar(int a, int l,int matriz[a][l][5],int minha_posicao[2], int saldo,int destino[2], int* carga){
if(matriz[minha_posicao[0]][minha_posicao[1]][1] > 1)
    if(*carga < 10)
    {
        if(minha_posicao[0] != destino[0] || minha_posicao[1] != destino[1]){
            // Se eu nao estou no meu destino
            if(*carga + matriz[destino[0]][destino[1]][1]-1 < 10)
                return 1;
            else
                return 0;
        }
        return 1;
    }
return 0;
}


char logica(int a, int l,int matriz[a][l][5],int minha_posicao[2], int saldo,int destino[2], int* carga){
    char acao;
        acao = 'j'; // Debbug
    //fprintf(stderr, "CARGA: %d", *carga);
    //Tentar vender
    if(matriz[minha_posicao[0]][minha_posicao[1]][2] == 1 && *carga > 2)
    {
        *carga = 0;
        return('s');
    }
    //Tentar pescar
    if(devo_pescar(a, l, matriz, minha_posicao, saldo, destino, carga) == 1)
    {
        *carga += 1;
        return('f');
    }
    //Indo vender
    if (*carga > 7)
    {
        if(matriz[destino[0]][destino[1]][2] != 1){
            melhor_local(a, l, matriz, minha_posicao, destino, 1, 0);
            /*
        // Se meu destino não era porto, agora e
        int portos[50][2], contagem = 0;
        for (int i = 0; i < a; i++)
            for (int j = 0; j < l; j++)
                if(matriz[i][j][2] == 1)
                {

                    //fprintf(stderr, "Coordenada porto %d: a:%d  l:%d\n ", contagem +1, i, j);
                    portos[contagem][0] = i;
                    portos[contagem][1] = j;
                    contagem += 1;
                }
        for (int i = 1; i < contagem; i++)
        {
            // Uma possivel melhoria seria atualizar as distancias na minha matriz em uma funcao e acessar os valores aqui (muito trabalho pra pouco retorno)
            if (abs(minha_posicao[0] - portos[i][0]) + abs(minha_posicao[1] - portos[i][1]) < abs(minha_posicao[0] - portos[0][0]) + abs(minha_posicao[1] - portos[0][1]))
            {
                // Eu acabo destruindo a informacao, mas nao salvo nem uso ela
                // Se o caminho pro porto estiver obstruido eu me lasco ********************
                portos[0][0] = portos[i][0];
                portos[0][1] = portos[i][1];
            }
        }
        destino[0] = portos[0][0];
        destino[1] = portos[0][1];
        fprintf(stderr, "Destino indo vender: a: %d  l:%d", destino[0], destino[1]);
        */
        }
        return andar(a, l, matriz, minha_posicao, destino, 1, 0);
    }
    else{
        //Indo pescar
        //Se eu estiver sem destino devo procurar um novo
        if(matriz[destino[0]][destino[1]][1] < 2) // ficar de olho nisso aqui
            melhor_local(a, l, matriz, minha_posicao, destino, 0, 0);
        // Me movo na direcao do destino em qualquer caso
            return andar(a, l, matriz, minha_posicao, destino, 0, 0);

    }

    return acao;
}

void acao(char entrada){
    // verificar se preciso do \n
switch (entrada)
{
case 'l':
    printf("LEFT\n");
    break;
case 'r':
    printf("RIGHT\n");
    break;
case 'u':
    printf("UP\n");
    break;
case 'd':
    printf("DOWN\n");
    break;
case 'f':
    printf("FISH\n");
    break;
case 's':
    printf("SELL\n");
    break;
default:
    fprintf(stderr, "ACAO RECEBEU ARGUMENTO ERRADO");
    break;
}
}

void leitura_final(char ato, int *saldo){
    char retorno[MAX_STR];
    // verificar espaco dos scanf
    if (ato =='l'|| ato == 'r'|| ato == 'u' || ato == 'd'){
        scanf(" %s", retorno);
        if (strcmp(retorno, "DONE") == 0)
            return;
        else if (strcmp(retorno, "BUSY") == 0)
            fprintf(stderr, "ERRO: TENTANDO IR PARA LOCAL OCUPADO\n");
        else if (strcmp(retorno, "OUT") == 0)
            fprintf(stderr, "ERRO: SAINDO DO MAPA\n");
        else
            fprintf(stderr, "ERRO: LEITURA_FINAL LENDO STRING ERRADA: %s\n", retorno);
    }
    else if(ato == 'f'){
        scanf(" %s", retorno);
        if(strcmp(retorno, "SEABASS") == 0 || strcmp(retorno, "SNAPPER")==0 || strcmp(retorno, "MULLET")==0)
            return;
        else if(strcmp(retorno, "NONE")==0)
            fprintf(stderr, "ERRO: PESCANDO EM LUGAR VAZIO\n");
        else if(strcmp(retorno, "IMPACT")==0)
            fprintf(stderr, "ERRO: PESQUEI ATE 0\n");
        else
            fprintf(stderr, "ERRO: LEITURA_FINAL LENDO STRING ERRADA: %s\n", retorno);
    }
    else if (ato == 's'){
        scanf(" %d", saldo);
    }
    else 
        fprintf(stderr, "ERRO: ATO ESTA ERRADO: %c\n", ato);


}


int main(){

    char myId[MAX_STR];   // identificador do bot em questão
    char temp[MAX_STR];
    char ato;

    setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
    setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
    setbuf(stderr, NULL);

// === Partida comeca === //
    int a, l, minha_posicao[2], saldo, destino[2], carga = 0;

    scanf("AREA %d %d", &a, &l);
    scanf(" ID %s", myId);

    // [0] -> Tipo do peixe
            // 1: Tainha(100)  2: Cioba(150)  3: Robalo(200)
    // [1] -> Kg de peixe
    // [2] -> Is porto?
    // [3] -> Ocupado?
            // 0: nao  1: ocupado  2: eu
    // [4] -> Distancia pro bot 
            // so usar quando indo pescar
    int matriz[a][l][5];


    for (int i = 0; i < a; i++)
        for (int j = 0; j < l; j++)
            for (int k = 0; k < 5; k++)
                matriz[i][j][k] = 0;
    
            


// === LOOP RODADAS === //
    while(1){

// LEITURA DO ARRAY e LEITURA COORDENADAS BOTS --OK
leitura(a, l, matriz, minha_posicao, myId);

// LOGICA
    ato = logica(a, l, matriz, minha_posicao, saldo, destino, &carga);
// ENVIO DA ACAO --OK
    acao(ato);
// LEITURA DO RESULTADO DA ACAO --OK
    // Se eu quiser adcionar logica envolvendo o saldo eh aqui
    leitura_final(ato, &saldo);


}
    return 0;
}

// Eu sei que usando structs seria melhor e eu sei usar structs
// Mas estava com uma duvida na parte da realocacao de ponteiros 
// dentro de structs e achei que nao teria tempo pra entender 
// por causa do prazo curto, acabou sobrando tempo, mas agora
// ja implementei sem.