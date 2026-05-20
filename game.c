#include <stdlib.h>
#include <time.h>
#include "game.h"

static float lane_speeds[NUM_LANES] = { 1.5f, 2.0f, 1.2f, 1.8f };
static int   lane_dirs[NUM_LANES]   = { 1, -1,1, -1};



void show_start_screen(void) {
    clear();

    // titulo em ASCII art
    attron(COLOR_PAIR(1));
    mvprintw(1, 5, "  __  __                    ____                         ____                  _ ");
    mvprintw(2, 5, " |  \\/  | ___  _____      _/ ___|_ __ ___  ___ ___ _   _|  _ \\ ___   __ _  __| |");
    mvprintw(3, 5, " | |\\/| |/ _ \\/ _ \\ \\ /\\ / / |   | '__/ _ \\/ __/ __| | | | |_) / _ \\ / _` |/ _` |");
    mvprintw(4, 5, " | |  | |  __/ (_) \\ V  V /| |___| | | (_) \\__ \\__ \\ |_| |  _ < (_) | (_| | (_| |");
    mvprintw(5, 5, " |_|  |_|\\___|\\___/ \\_/\\_/  \\____|_|  \\___/|___/___/\\__, |_| \\_\\___/ \\__,_|\\__,_|");
    mvprintw(6, 5, "                                                      |___/                        ");
    mvprintw(8,  15, "=^.^=  =^.^=  =^.^=  =^.^=  =^.^=  =^.^=");
    


    
    mvprintw(10, 15, "Ajuda o Meow a chegar aos peixes ><>");
    mvprintw(11, 15, "sem ser atropelado pelos carros @[]@");

   
    mvprintw(13, 15, "CONTROLOS:");
    mvprintw(14, 15, "  Setas  --  mover o gato");
    mvprintw(15, 15, "  Q      --  sair");

    mvprintw(17, 15, "Tens 9 vidas. Boa sorte!");
    mvprintw(19, 15, ">>> Pressiona qualquer tecla para comecar <<<");
    attroff(COLOR_PAIR(1));

    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

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
        cars[lane][i].row = 3 + lane * 3; // calcular a linha correspondente à lane atual
        cars[lane][i].col   = (float)(i * (COLS / CARS_PER_LANE)); // distribuir os carros uniformemente na lane
        cars[lane][i].speed = lane_speeds[lane]; // atribuir a velocidade da lane
        cars[lane][i].direction   = lane_dirs[lane]; // atribuir a direção da lane
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
            gs->cars[l][i].col += gs->cars[l][i].speed * gs->cars[l][i].direction;

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

void draw(GameState *gs) {

clear();

attron(COLOR_PAIR(1));
mvprintw(0, 0, "SCORE: %d  VIDAS: %d", gs->cat.score, gs->cat.lives); // função mvprintw para exibir a pontuação e vidas do gato no topo da tela

attron(COLOR_PAIR(4));
mvprintw(1, 0, "><>  ><>  ><>  ><>  ><>  ><>  ><> ><> ><> ><> ><> ><> ><> ><> "); // desenhar a linha de chegada
attroff(COLOR_PAIR(4));


//mvprintw(2, 0, "------------------------------------------------------------"); // desenhar a linha de início


draw_lane_recursive(gs, 0);
attron(COLOR_PAIR(1));
mvprintw(gs->cat.row, gs->cat.col, "=^.^=");
attroff(COLOR_PAIR(1));

//mvprintw(6, 0, "------------------------------------------------------------");


attron(COLOR_PAIR(5));
mvprintw(3 + NUM_LANES * 3 + 1, 0, "HOME");
attroff(COLOR_PAIR(5));
refresh();
}

void draw_lane_recursive(GameState *gs, int lane) {
    if (lane >= NUM_LANES) return;
    
    int row = 3 + lane * 3;

    // linha vazia acima
    mvprintw(row - 1, 0, "                                                            ");
    
    // linha da estrada com os carros
    for (int c = 0; c < COLS; c++)
        mvprintw(row, c, ".");

    // carros
    for (int i = 0; i < CARS_PER_LANE; i++)
        mvprintw(row, (int)gs->cars[lane][i].col, "@[]@");

    // linha vazia abaixo
    mvprintw(row + 1, 0, "                                                            ");

    draw_lane_recursive(gs, lane + 1);
}

void show_milestone_screen(int score) {
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(3,  15, "=^.^= =^.^= =^.^= =^.^= =^.^=");
    mvprintw(5,  15, "    /\\_____/\\  ");
    mvprintw(6,  15, "   /  o   o  \\ ");
    mvprintw(7,  15, "  ( ==  ^  == )");
    mvprintw(8,  15, "   )         ( ");
    mvprintw(9,  15, "  (           )");
    mvprintw(10, 15, " ( (  )   (  ) )");
    mvprintw(11, 15, "(__(__)___(__)__)");
    mvprintw(13, 15, "BOM TRABALHO! O Meow esta muito contente!");
    mvprintw(14, 15, "Pontuacao: %d", score);
    mvprintw(16, 15, "Pressiona qualquer tecla para continuar...");
    attroff(COLOR_PAIR(1));
    flushinp(); // limpar o buffer de entrada para evitar que a próxima tecla seja lida imediatamente
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

void show_gameover_screen(int score) {
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(3, 10, "  ____    _    __  __ _____    _____     _______ ____  ");
    mvprintw(4, 10, " / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\ ");
    mvprintw(5, 10, "| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |");
    mvprintw(6, 10, "| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ < ");
    mvprintw(7, 10, " \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\");
    mvprintw(9, 10, "O Meow gastou as suas 9 vidas... x_x");
    mvprintw(11, 10, "Pontuacao final: %d", score);
    mvprintw(13, 10, "Pressiona qualquer tecla para sair...");
    attroff(COLOR_PAIR(1));
    refresh();
    nodelay(stdscr, FALSE);
    getch();
}





