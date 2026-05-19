#include <stdlib.h>
#include <time.h>
#include "game.h"

static float lane_speeds[NUM_LANES] = {1.5f,2.0f,1.2f}; //
static int   lane_dirs[NUM_LANES]   = {1,-1,1}; 

// inicializa o estado do jogo
// gs: ponteiro para o estado do jogo a ser inicializado// 
// o operador "->" para atribuir os valores iniciais do gato, como a 
// posição inicial (linha 18 e coluna central),
// o número de vidas (CAT_LIVES) e a pontuação inicial (0)
//(aceder aos "atributos" do gato dentro da estrutura GameState e atribuir os valores iniciais)

void init_game(GameState *gs) {
    gs->cat.row   = 18;
    gs->cat.col   = COLS / 2;
    gs->cat.lives = CAT_LIVES;
    gs->cat.score = 0;
    gs->running = 1; // 0 = jogo parado, 1 = jogo em execução
    gs->frame   = 0; // contador de frames para controlar a geração de obstáculos
    init_cars_recursive(gs->cars, 0);
}

// função recursiva para inicializar os carros em cada lane 

void init_cars_recursive(Car cars[][CARS_PER_LANE], int lane) {
    if (lane >= NUM_LANES) return;

    for (int i = 0; i < CARS_PER_LANE; i++) {
        cars[lane][i].row   = lane * 2 + 3;  // lanes nas linhas 3, 5, 7 
        cars[lane][i].col   = (float)(i * (COLS / CARS_PER_LANE)); // distribuir os carros uniformemente na lane
        cars[lane][i].speed = lane_speeds[lane]; // atribuir a velocidade da lane
        cars[lane][i].dir   = lane_dirs[lane]; // atribuir a direção da lane
    }
    init_cars_recursive(cars, lane + 1); //chamar a funcao recursiva para a proxima lane 

}

// função para lidar com a entrada do usuário (teclas de seta para mover o gato e 'q' para sair)

void handle_input(GameState *gs, int key) {
   switch (key) {
    case KEY_UP: 
    if (gs->cat.row > 0) gs->cat.row--;
    break;

    case KEY_DOWN:
    if (gs->cat.row < ROWS - 1) gs->cat.row++;
    break;

    case KEY_LEFT:
    if (gs->cat.col > 0) gs->cat.col--;
    break;
    case KEY_RIGHT:
    if (gs->cat.col < COLS - 1) gs->cat.col++;
    break;
    case 'q':
    gs->running = 0; // parar o jogo
    break;                  
}


}

// função para atualizar a posição dos carros com base em sua velocidade e direção
// se um carro sair da tela, ele é reposicionado do outro lado para criar um loop contínuo de tráfego

void update_cars(GameState *gs) {


    for (int l = 0; l < NUM_LANES; l++) {
        for (int i = 0; i < CARS_PER_LANE; i++) {
            // atualizar a posição do carro
            gs->cars[l][i].col += gs->cars[l][i].speed * gs->cars[l][i].dir;

            // se o carro sair da tela, reposicioná-lo do outro lado
            if (gs->cars[l][i].col < 0) {
                gs->cars[l][i].col = COLS - 1;
            } else if (gs->cars[l][i].col >= COLS) {
                gs->cars[l][i].col = 0;
            }
        }
    }



}

// função para verificar colisões entre o gato e os carros


int check_collision(GameState *gs) {

    // percorrer todas as lanes e carros para verificar se o gato colidiu com algum carro

    for (int l = 0; l < NUM_LANES; l++) {
        for (int i = 0; i < CARS_PER_LANE; i++) {   
   

    // verificar se o gato está na mesma linha do carro e se a coluna do gato está dentro do alcance do carro (considerando que o carro ocupa 4 colunas)        
   if (gs->cat.row == gs->cars[l][i].row && gs->cat.col >= (int)gs->cars[l][i].col && 
    gs->cat.col <= (int)gs->cars[l][i].col + 3) {
    return 1;
}
}
    }
    return 0; // sem colisão
}   

void check_goal(GameState *gs) {

    // verificar se o gato alcançou a linha de chegada (linha 0)
    if (gs->cat.row == 0) {
        gs->cat.score += 50; // aumentar a pontuação
        gs->cat.row = 18; // reposicionar o gato na posição inicial
        gs->cat.col = COLS / 2;
        gs->frame = 0; // resetar o contador de frames para gerar novos carros
        for (int l = 0; l < NUM_LANES; l++)
            for (int i = 0; i < CARS_PER_LANE; i++)
                gs->cars[l][i].speed *= 1.1f;
    }

}

void cat_die(GameState *gs) {
    flash();
    napms(300);
    gs->cat.lives--; // perder uma vida
    if (gs->cat.lives <= 0) {
        gs->running = 0; // parar o jogo se não houver mais vidas
    } else {
        gs->cat.row = 18; // reposicionar o gato na posição inicial
        gs->cat.col = COLS / 2;
        gs->frame = 0; // resetar o contador de frames para gerar novos carros
    }
}



