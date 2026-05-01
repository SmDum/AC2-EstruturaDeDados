# 🏰 Labirinto de Dados — AC2 Estrutura de Dados

Projeto acadêmico desenvolvido em **C** para a disciplina de Estrutura de Dados. O programa resolve um labirinto usando **backtracking completo**, explorando todos os caminhos possíveis para encontrar aquele que maximiza a coleta de tesouros (moedas).

---

## 📋 Descrição

O jogador parte de um ponto inicial `P` e precisa encontrar a saída `S` do labirinto. Durante o percurso, pode coletar itens armazenados em uma **mochila**. O algoritmo não para no primeiro caminho encontrado — ele explora **todas as rotas válidas** e retorna aquela com o maior número de tesouros coletados.

---

## 🗂️ Estrutura do Projeto

```
AC2-EstruturaDeDados/
├── main.c          # Ponto de entrada do programa
├── labirinto.c/.h  # Carregamento, impressão e salvamento do mapa
├── busca.c/.h      # Algoritmo de backtracking completo
├── pilha.c/.h      # Estrutura de dados Pilha (controle do caminho)
├── mochila.c/.h    # Estrutura de dados Mochila (itens coletados)
├── mapa.txt        # Arquivo de entrada com o labirinto
├── nomes.txt       # Arquivo auxiliar de nomes
├── solucao.txt     # Saída gerada com o melhor caminho encontrado
└── labirinto.exe   # Executável pré-compilado (Windows)
```

---

## ⚙️ Como Funciona

1. O labirinto é carregado do arquivo `mapa.txt`
2. As estruturas de dados (pilha e mochila) são inicializadas
3. O algoritmo de **backtracking** é executado, explorando todos os caminhos possíveis
4. O melhor resultado (maior número de moedas) é aplicado ao mapa
5. O caminho encontrado é exibido no terminal e salvo em `solucao.txt`

### Legenda do Mapa

| Símbolo | Significado         |
|---------|---------------------|
| `P`     | Posição inicial     |
| `S`     | Saída do labirinto  |
| `#`     | Parede (bloqueio)   |
| ` `     | Espaço livre        |
| `*`     | Melhor caminho encontrado (na solução) |
| Outros  | Tesouros/moedas coletáveis |

---

## 🚀 Como Compilar e Executar

### Pré-requisitos

- Compilador C (GCC recomendado)
- Sistema operacional: Windows, Linux ou macOS

### Compilação

```bash
gcc main.c labirinto.c busca.c pilha.c mochila.c -o labirinto
```

### Execução

```bash
./labirinto
```

> No Windows, você também pode executar diretamente o `labirinto.exe` já disponível no repositório.

---

## 🔧 Configuração

No arquivo `main.c`, você pode ajustar as seguintes constantes:

```c
#define ARQUIVO_MAPA    "mapa.txt"    // Arquivo de entrada do labirinto
#define ARQUIVO_SOLUCAO "solucao.txt" // Arquivo de saída com a solução
#define DELAY_MS        0             // Delay entre passos da visualização (ms)
```

- `DELAY_MS 0` — sem pausa (recomendado para labirintos grandes)
- `DELAY_MS 50` — levemente animado
- `DELAY_MS 100` — mais lento, fácil de acompanhar

Para usar um mapa diferente, basta editar o arquivo `mapa.txt` ou apontar `ARQUIVO_MAPA` para outro arquivo.

---

## 🧠 Estruturas de Dados Utilizadas

- **Pilha** (`pilha.c/.h`) — rastreia o caminho percorrido durante o backtracking
- **Mochila** (`mochila.c/.h`) — armazena os itens coletados ao longo do caminho
- **Matriz (Labirinto)** (`labirinto.c/.h`) — representa o mapa como uma grade bidimensional

---

## 📄 Exemplo de Saída

```
Labirinto carregado: 10 linhas x 20 colunas
Inicio: (1, 1) | Saida: (8, 18)

ATENCAO: A busca explorara TODOS os caminhos possiveis
para encontrar o que maximiza o numero de tesouros.
Pressione ENTER para iniciar...

[BUSCA] Explorando todos os caminhos...

<------------------------------------------------------------------------------------------------
 MISSAO CUMPRIDA! MELHOR CAMINHO ENCONTRADO!
------------------------------------------------------------------------------------------------>

--- MOCHILA FINAL (MELHOR RESULTADO) ---
...

--- CAMINHO PERCORRIDO ---
Tamanho do caminho: 42 passos
```

---

## 👨‍💻 Autor

Desenvolvido por **Giovanni de Luca Gomieri, Guilherme Alves dos Santos, Matheus Cravo Purcena Gonçalves e Samuel de Moraes Delgado** como atividade avaliativa (AC2) da disciplina de Estrutura de Dados.

---

## 📚 Disciplina

> Estrutura de Dados — Atividade Contínua 2 (AC2)
