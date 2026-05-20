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

## Sessao 2- 20/05/2026 11:19

