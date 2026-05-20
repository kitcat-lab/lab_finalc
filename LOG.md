## Sessão 1– 19/05/2026 16:44

**Funcionalidades implementadas:**
- Criação do `game.h`: structs (‘Cat’, `Car`, `GameState`), constantes e promessas 
- `init_game` :inicialização do estado do jogo
- `init_cars_recursive`: inicialização recursiva dos carros por lane
- `handle_input`: movimento do gato com setas do teclado
- `update_cars` :movimento dos carros com wrap horizontal
- `check_collision` :deteção de colisão gato-carro
- `check_goal` :deteção de chegada à meta e aumento de dificuldade
- `cat_die` :perda de vida e reposicionamento do gato

**Maior dificuldade:**
Pensar nas funções e na estrutura do código antes de começar a escrever, planear a lógica antes de implementar.

**Como resolvi (ou não resolvi):**
Fui construindo função a função devagar, percebendo o que cada uma precisa antes de escrever o código. E que podemos ir alterando tudo.

**Próximo passo planeado:** 
Implementar a função `draw` com ncurses e os treats aleatórios.

**Linhas de código escritas (estimativa):** ~150

## Sessao 2- 20/05/2026 14:27

**Funcionalidades implementadas:**
- `draw`: desenho do estado do jogo com ncurses
- `draw_lane_recursive`: desenho recursivo das lanes e carros
- `show_start_screen`: ecrã inicial com título e instruções
- `show_gameover_screen`: ecrã de fim de jogo com ASCII art e pontuação final
- `show_milestone_screen`: ecrã de celebração a cada 500 pontos com ASCII art do Meow
- Cores com `attron`/`attroff` para gato, carros, peixes e UI
- `main.c` completo com loop principal, gestão de input e condições de vitória/derrota
- Testes e correções de bugs

**Maior dificuldade:**
Posicionamento correto das lanes e dos carros no ecrã, ajustar o tamanho do ecrã, e o bug do milestone que dispara repetidamente enquanto o score permanece no mesmo múltiplo de 500.

**Como resolvi (ou não resolvi):**
O posicionamento das lanes foi resolvido ajustando a fórmula `3 + lane * 3` e aumentando o ROWS para 40. O bug do milestone ainda não foi corrigido. 

**Próximo passo planeado:**
Corrigir o bug do milestone, implementar as sugestões do formador. 
criar funcao treats aleatorios nas lanes 
