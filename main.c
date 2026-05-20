#include <ncurses.h>
#include "game.h"

int main() {
    initscr(); // inicializar ncurses
    start_color(); // habilitar cores
    noecho(); // não mostrar os caracteres digitados
    curs_set(FALSE); // esconder o cursor
    keypad(stdscr, TRUE); // habilitar teclas de seta
    cbreak();              // input imediato sem Enter
    nodelay(stdscr, TRUE); // getch() não-bloqueante

    // cores com ncurses
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // par 1 = magenta sobre preto (gato)
    init_pair(2, COLOR_RED, COLOR_BLACK);     // par 2 = vermelho sobre preto (carros)
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // par 3 = verde sobre preto (relva )
    init_pair(4, COLOR_CYAN,    COLOR_BLACK); // par 4 = cyan sobre preto (peixes)
    init_pair(5, COLOR_YELLOW,  COLOR_BLACK); // par 5 = amarelo sobre preto (UI)

    GameState gs;
    init_game(&gs);
    show_start_screen();

    while (gs.running) {
        int key = getch(); // ler a entrada do usuário
        handle_input(&gs, key); // processar a entrada
        update_cars(&gs); // atualizar a posição dos carros

        if (check_collision(&gs)) {
            cat_die(&gs); // lidar com a morte do gato
        } else {
            check_goal(&gs); // verificar se o gato alcançou a linha de chegada
        }
        if (gs.cat.score % 500 == 0 && gs.cat.score > 0)
            show_milestone_screen(gs.cat.score);
        draw(&gs); // desenhar o estado do jogo na tela
        napms(FRAME_MS); // esperar um pouco antes de atualizar o próximo frame
        gs.frame++; // incrementar o contador de frames
    

        
    }
    show_gameover_screen(gs.cat.score); // mostrar a tela de game over

    endwin(); // encerrar ncurses
    return 0;
}