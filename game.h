#ifndef GAME_H // se GAME_H não estiver definido
#define GAME_H // define GAME_H para evitar múltiplas inclusões

#include <ncurses.h>

// constantes, structs e promessas de funções para o jogo


#define COLS 60 // define a constante COLS como 60 -> o número de colunas do terminal
#define ROWS 20 // define a constante ROWS como 20 -> o número de linhas do terminal
#define NUM_LANES 3 // define a constante NUM_LANES como 3 -> numero de lanes 
#define CARS_PER_LANE 2 // define a constante CARS_PER_LANE como 2 -> número de carros por lane
#define CAT_LIVES 3 // define a constante CAT_LIVES como 3 -> número de vidas do gato
#define SCORE_PER_GOAL 10 // -> pontos ganhos por alcançar o objetivo
#define FRAME_MS 100 // define a constante FRAME_MS como 100 -> tempo em milissegundos para cada frame do jogo

typedef struct {
    int row;
    int col;
    int lives;
    int score;
} Cat;

typedef struct {
    int row;
    float col;
    float speed; // velocidade do carro em colunas por frame
    int direction; // 1 para direita, -1 para esquerda
} Car;

typedef struct {
    Cat cat;
    Car cars[NUM_LANES][CARS_PER_LANE];
    int running;
    int frame;
} GameState;

void init_game(GameState *gs);
void handle_input(GameState *gs, int key);
void update_cars(GameState *gs);
int  check_collision(GameState *gs);
void check_goal(GameState *gs);
void draw(GameState *gs);

#endif // fim do bloco de inclusão condicional