# ConectaDCC — Documentação Técnica

**DCC221 – Estruturas de Dados – TP2 (2026/1, UFMG)**
**Aluno:** Victor Arcuri

---

## Sumário

1. [Visão Geral do Sistema](#1-visão-geral-do-sistema)
2. [Restrições de Implementação](#2-restrições-de-implementação)
3. [Arquitetura e Camadas](#3-arquitetura-e-camadas)
4. [TADs e Classes](#4-tads-e-classes)
   - 4.1 [Lista\<T\>](#41-listat)
   - 4.2 [MedicoesOp e Enums de Operações](#42-medicoesop-e-enums-de-operações)
   - 4.3 [Structs e Enums do Grafo](#43-structs-e-enums-do-grafo)
   - 4.4 [MatrizAdjacencia](#44-matrizadjacencia)
   - 4.5 [ListaAdjacencia](#45-listaadjacencia)
   - 4.6 [ListaArestas (ponto extra)](#46-listaarestas-ponto-extra)
   - 4.7 [Grafo](#47-grafo)
   - 4.8 [Usuario e Tema](#48-usuario-e-tema)
   - 4.9 [Dicionario](#49-dicionario)
   - 4.10 [Sistema](#410-sistema)
5. [Protocolo de Entrada/Saída](#5-protocolo-de-entradasaída)
6. [Análise de Complexidade](#6-análise-de-complexidade)
7. [Análise Experimental](#7-análise-experimental)
8. [Conclusão](#8-conclusão)

---

## 1. Visão Geral do Sistema

O ConectaDCC modela uma rede social acadêmica onde **usuários** seguem uns aos outros e declaram interesse em **temas**. O sistema mantém simultaneamente dois grafos independentes:

| Grafo | Direção | Vértices | Significado da aresta |
|---|---|---|---|
| `grafo_social` | Dirigido | Apenas usuários | u→v significa "u segue v" |
| `grafo_temas` | Não dirigido (bipartido) | Usuários + Temas | u—t significa "u tem interesse em t" |

Ambos os grafos suportam troca dinâmica de representação interna em tempo de execução, sem perda de dados, escolhendo entre três implementações: **Lista de Adjacência (L)**, **Matriz de Adjacência (M)** e **Lista de Arestas (E)**.

---

## 2. Restrições de Implementação

- **C++11** — padrão obrigatório imposto pelo avaliador VPL.
- **Sem contêineres STL** — `std::vector`, `std::list`, `std::map` e similares são proibidos. `std::string` e `<chrono>` são permitidos.
- **Sem ponteiros inteligentes** — toda gerência de memória é manual via `new`/`delete`.
- **`Lista<T>`** é o único contêiner de sequência genérica; usado em todas as estruturas internas.

---

## 3. Arquitetura e Camadas

```
main.cpp
  └─ Sistema
       ├─ Lista<Usuario>          (dados de perfil)
       ├─ Lista<Tema>             (dados de perfil)
       ├─ Grafo grafo_social      (quem segue quem)
       ├─ Grafo grafo_temas       (interesses usuário-tema)
       └─ Dicionario              (mapeamento id_externo ↔ id_grafo)

Grafo
  ├─ Lista<No*>                   (vetor de vértices)
  └─ uma das representações:
       ├─ MatrizAdjacencia        (Lista<Lista<int>>)
       ├─ ListaAdjacencia         (Lista<Lista<No*>>)
       └─ ListaArestas            (Lista<Aresta>)
```

O fluxo de controle completo é: `main.cpp` lê comandos do `stdin`, despacha para `Sistema`, que traduz IDs externos em índices internos via `Dicionario` e delega operações estruturais aos dois `Grafo`s.

---

## 4. TADs e Classes

### 4.1 `Lista<T>`

**Arquivo:** `include/Lista.hpp`

Vetor dinâmico genérico com estratégia de duplicação de capacidade. É o único contêiner de sequência do projeto e serve de base para todas as demais estruturas.

**Campos internos:**

| Campo | Tipo | Descrição |
|---|---|---|
| `dados` | `T*` | Array alocado no heap |
| `tamanho` | `int` | Número de elementos presentes |
| `capacidade` | `int` | Capacidade atual alocada |

**Métodos públicos:**

| Método | Complexidade | Descrição |
|---|---|---|
| `inserir(elem)` | O(1) amortizado | Insere ao final; dobra capacidade quando cheia |
| `remover(pos)` | O(n) | Remove na posição, desloca elementos à direita |
| `encontrar(elem)` | O(n) | Busca linear por igualdade (`==`) |
| `obter(pos)` | O(1) | Retorna referência ao elemento; lança se fora dos limites |
| `get_tamanho()` | O(1) | Retorna o número de elementos |

**Método privado:**

- `redimensionar()` — dobra `capacidade`, realoca e copia. Chamado apenas por `inserir` quando `tamanho == capacidade`.

**Invariante de projeto:** a capacidade começa em 2 (`CAPACIDADE_INICIAL`) e nunca encolhe. O operador `[]` é um alias para `obter`.

---

### 4.2 `MedicoesOp` e Enums de Operações

**Arquivo:** `include/Medicoes.hpp`

Infraestrutura de instrumentação temporal embutida nos TADs. Usa `<chrono>` para medir o tempo de cada operação em nanossegundos.

**`struct MedicoesOp`**

Acumulador para uma única operação. Campos: `tempo_total_ns` (`long long`) e `contagem` (`int`).

- `registrar(duracao_ns)` — adiciona a duração ao acumulador e incrementa a contagem.
- `tempo_medio_ns()` — retorna `tempo_total_ns / contagem`, ou 0 se nenhuma chamada foi feita.

**`enum OperacaoGrafo`**

Índices do array `medicoes[NUM_OP_GRAFO]` interno ao `Grafo`:

```
OP_G_CRIAR_NO, OP_G_CRIAR_ARESTA, OP_G_REMOVER_ARESTA,
OP_G_VIZINHOS_APONTADOS, OP_G_VIZINHOS_APONTAM_PARA,
OP_G_VIZINHOS_BIDIRECIONAL, OP_G_CHECAR_ARESTA, OP_G_TROCAR_TIPO
```

**`enum OperacaoSistema`**

Índices do array `medicoes[NUM_OP_SISTEMA]` interno ao `Sistema`:

```
OP_S_ALTERAR_ARMAZENAMENTO, OP_S_ADICIONAR_USUARIO, OP_S_ADICIONAR_TEMA,
OP_S_SEGUIR_USUARIO, OP_S_REMOVER_SEGUIMENTO, OP_S_CONSULTAR_TEMAS,
OP_S_CONSULTAR_SEGUIDORES, OP_S_CONSULTAR_SEGUIDOS, OP_S_CONSULTAR_AMIGOS,
OP_S_CONSULTAR_RELACAO, OP_S_CONSULTAR_INTERESSE, OP_S_CONSULTAR_POPULARIDADE,
OP_S_CONSULTAR_RECOMENDACAO
```

Os arrays são declarados `mutable` para que métodos `const` (como as consultas) possam acumular medições sem violar a semântica de constância.

---

### 4.3 Structs e Enums do Grafo

**Arquivo:** `include/Grafo.hpp`

**`enum TipoNo`** — `USUARIO` ou `TEMA`. Diferencia os vértices dos dois domínios dentro do mesmo grafo.

**`struct No`** — vértice do grafo com três campos:

| Campo | Tipo | Descrição |
|---|---|---|
| `id_externo` | `int` | ID de domínio (0-indexed, atribuído pelo Sistema) |
| `id_grafo` | `int` | Índice interno no array `nos[]` do Grafo |
| `tipo` | `TipoNo` | `USUARIO` ou `TEMA` |

**`struct Aresta`** — par `(id_origem, id_destino)` de índices internos. Usado exclusivamente pela `ListaArestas`.

**`enum TipoGrafo`** — `GRAFO_LISTA`, `GRAFO_MATRIZ` ou `GRAFO_ARESTA`.

---

### 4.4 `MatrizAdjacencia`

**Arquivo:** `include/Grafo.hpp` / `src/Grafo.cpp`

Representação de grafo por **matriz de adjacência quadrada** implementada como `Lista<Lista<int>>`. Cada posição `[i][j]` vale `1` se há aresta de `i` para `j`, e `0` caso contrário.

**Campos:** `dimensoes` (int) e `dados` (`Lista<Lista<int>>`).

**Métodos:**

| Método | Complexidade | Descrição |
|---|---|---|
| `aumentar_dimensao()` | O(n) | Acrescenta coluna em cada linha existente e uma nova linha completa |
| `alterar_valor(i, j, v, bidi)` | O(1) | Define `dados[i][j] = v`; se bidirecional, também `dados[j][i]` |
| `obter_valor(i, j)` | O(1) | Retorna `dados[i][j]` |

**Custo de memória:** O(n²) onde n é o número de vértices. Para grafos esparsos, isso representa desperdício significativo de memória.

---

### 4.5 `ListaAdjacencia`

**Arquivo:** `include/Grafo.hpp` / `src/Grafo.cpp`

Representação por **lista de adjacência** usando `Lista<Lista<No*>>`. Cada linha `i` armazena ponteiros para os nós vizinhos de **saída** do nó `i`.

**Campos:** `tamanho` (int) e `nos` (`Lista<Lista<No*>>`).

**Métodos:**

| Método | Complexidade | Descrição |
|---|---|---|
| `adicionar_aresta(no1, no2, bidi)` | O(1) amortizado | Insere `no2` na lista de `no1`; se bidirecional, insere `no1` na lista de `no2` |
| `remover_aresta(no1, no2, bidi)` | O(grau(no1)) | Busca linear e remoção em `nos[no1->id_grafo]` |
| `aumentar_tamanho()` | O(1) amortizado | Insere lista vazia para um novo nó |

**Custo de memória:** O(n + m) onde m é o número de arestas. Ideal para grafos esparsos.

---

### 4.6 `ListaArestas` (ponto extra)

**Arquivo:** `include/Grafo.hpp` / `src/Grafo.cpp`

Terceira representação interna, implementada como **lista plana de arestas** (`Lista<Aresta>`). Armazena apenas os pares `(id_origem, id_destino)` existentes, sem alocar memória proporcional a n².

**Campos:** `arestas` (`Lista<Aresta>`).

**Métodos:**

| Método | Complexidade | Descrição |
|---|---|---|
| `adicionar_aresta(id1, id2, bidi)` | O(1) amortizado | Insere struct Aresta ao final |
| `remover_aresta(id1, id2, bidi)` | O(\|E\|) | Busca linear em toda a lista para encontrar o par |
| `checar_aresta(id1, id2)` | O(\|E\|) | Busca linear em toda a lista |
| `obter_destinos_de(id)` | O(\|E\|) | Filtra arestas com `id_origem == id` |
| `obter_origens_de(id)` | O(\|E\|) | Filtra arestas com `id_destino == id` |
| `get_arestas()` | O(1) | Acesso à lista interna (usado na migração de representação) |

**Custo de memória:** O(m), o melhor entre as três representações para grafos muito esparsos. A penalidade é que toda operação de consulta de vizinhança requer varredura completa da lista.

---

### 4.7 `Grafo`

**Arquivo:** `include/Grafo.hpp` / `src/Grafo.cpp`

TAD genérico de grafo. Encapsula uma das três representações e expõe uma interface uniforme independente da representação ativa.

**Campos privados:**

| Campo | Tipo | Descrição |
|---|---|---|
| `nos` | `Lista<No*>` | Vetor de todos os vértices do grafo |
| `id_nos` | `int` | Próximo id_grafo a ser atribuído |
| `tipo` | `TipoGrafo` | Representação ativa |
| `matriz` | `MatrizAdjacencia*` | Não-nulo apenas se `tipo == GRAFO_MATRIZ` |
| `lista` | `ListaAdjacencia*` | Não-nulo apenas se `tipo == GRAFO_LISTA` |
| `lista_arestas` | `ListaArestas*` | Não-nulo apenas se `tipo == GRAFO_ARESTA` |
| `medicoes` | `mutable MedicoesOp[NUM_OP_GRAFO]` | Acumuladores de tempo por operação |

**Invariante:** exatamente um dos três ponteiros de representação é não-nulo em qualquer momento.

**Métodos públicos:**

| Método | Descrição |
|---|---|
| `criar_no(tipo, id)` | Aloca `No`, atribui `id_grafo = id_nos++`, expande a representação ativa |
| `criar_aresta(id1, id2, bidi)` | Delega para a representação ativa |
| `remover_aresta(id1, id2, bidi)` | Delega para a representação ativa |
| `checar_aresta(id1, id2)` | Delega para a representação ativa |
| `obter_vizinhos_apontados_por(id)` | Retorna Lista\<No*\> com vizinhos de saída |
| `obter_vizinhos_que_apontam_para(id)` | Retorna Lista\<No*\> com vizinhos de entrada |
| `obter_vizinhos_bidirecional(id)` | Retorna Lista\<No*\> com vizinhos em ambas as direções |
| `trocar_tipo(novo_tipo)` | Migra todas as arestas para a nova representação sem perda de dados |
| `get_medicoes()` | Retorna o array de medições para o relatório experimental |

**Migração de representação (`trocar_tipo`):** aloca a nova estrutura, percorre todas as arestas da representação antiga e as re-insere na nova, depois libera a antiga. Suporta todas as seis direções de troca entre L, M e E.

---

### 4.8 `Usuario` e `Tema`

**Arquivos:** `include/Usuario.hpp`, `include/Tema.hpp`

Classes de dados simples que armazenam os atributos de domínio.

**`Usuario`:** `id` (int), `nome` (string), `idade` (int). Getters: `get_id()`, `get_nome()`, `get_idade()`.

**`Tema`:** `id` (int), `nome` (string), `tipo` (`TipoTema`). Getters: `get_id()`, `get_nome()`, `get_tipo()`.

**`enum TipoTema`:** `ACADEMICO` ou `PESSOAL`.

Instâncias ficam armazenadas nas `Lista<Usuario>` e `Lista<Tema>` do `Sistema`. Os grafos não replicam esses dados; referenciam apenas por `id_externo` no struct `No`.

---

### 4.9 `Dicionario`

**Arquivo:** `include/Dicionario.hpp`

Camada de tradução entre identificadores externos de domínio (0-indexed, atribuídos pelo `Sistema`) e índices internos dos grafos (`id_grafo`). Mantém três arrays de mapeamento:

| Array | Índice | Valor |
|---|---|---|
| `map_usuario_social` | `id_usuario` | `id_grafo` do usuário em `grafo_social` |
| `map_usuario_temas` | `id_usuario` | `id_grafo` do usuário em `grafo_temas` |
| `map_tema_temas` | `id_tema` | `id_grafo` do tema em `grafo_temas` |

O `Dicionario` guarda **referências** (não cópias) para as listas e grafos do `Sistema`, evitando duplicação de dados.

**Métodos principais:**

- `registrar_usuario_social(id_grafo)` — chamado logo após `Grafo::criar_no` para registrar o mapeamento.
- `get_no_usuario_grafo_social(id)` — traduz `id_externo` → `No&` no grafo social.
- `get_no_usuario_grafo_temas(id)` — traduz `id_externo` → `No&` no grafo de temas.
- `get_no_tema_grafo_temas(id)` — traduz `id_externo` de tema → `No&` no grafo de temas.

---

### 4.10 `Sistema`

**Arquivo:** `include/Sistema.hpp` / `src/Sistema.cpp`

Orquestrador principal. Implementa todos os comandos do protocolo e mantém o estado completo da rede.

**Campos:** `Lista<Usuario> usuarios`, `Lista<Tema> temas`, `Grafo grafo_social`, `Grafo grafo_temas`, `Dicionario dicionario`, contadores `num_usuarios`/`num_temas`, e o array `mutable MedicoesOp medicoes[NUM_OP_SISTEMA]`.

**Métodos auxiliares privados:**

- `nova_linha()` — controla separação entre saídas consecutivas (imprime `\n` antes de cada saída exceto a primeira).
- `ordenar_vizinhos(lista)` — quicksort in-place sobre `Lista<No*>` por `id_externo` crescente, garantindo que todas as saídas de consulta de vizinhança estejam ordenadas.
- `bfs_distancias_social(id)` — BFS no `grafo_social` que retorna distâncias mínimas dirigidas a partir de um nó de origem. Usado na recomendação.
- `calcular_jaccard(id_u, id_v)` — calcula `|A ∩ B| / |A ∪ B|` sobre os conjuntos de temas de dois usuários via vizinhança no `grafo_temas`. Usado na recomendação.

**Comandos implementados:**

| Método | Comando | O que faz |
|---|---|---|
| `alterar_armazenamento(tipo)` | `A L/M/E` | Chama `trocar_tipo` nos dois grafos |
| `adicionar_usuario(nome, idade, tema_ids)` | `U` | Cria nó nos dois grafos, conecta ao `Dicionario` e cria arestas no `grafo_temas` |
| `adicionar_tema(nome, tipo)` | `T` | Cria nó no `grafo_temas` e registra no `Dicionario` |
| `seguir_usuario(id1, id2)` | `S` | Cria aresta dirigida `id1→id2` no `grafo_social` |
| `remover_seguimento_usuario(id1, id2)` | `R` | Remove aresta `id1→id2` do `grafo_social` |
| `consultar_temas(id)` | `LT` | `obter_vizinhos_apontados_por` no `grafo_temas` |
| `consultar_seguidores(id)` | `LC` | `obter_vizinhos_que_apontam_para` no `grafo_social` |
| `consultar_seguidos(id)` | `LS` | `obter_vizinhos_apontados_por` no `grafo_social` |
| `consultar_amigos(id)` | `LA` | `obter_vizinhos_bidirecional` no `grafo_social` |
| `consultar_relacao(id1, id2)` | `Q` | Dois `checar_aresta` + código 0/1/2/3 |
| `consultar_interesse(id_u, id_t)` | `G` | `checar_aresta` no `grafo_temas` |
| `consultar_popularidade(id_t)` | `F` | `obter_vizinhos_que_apontam_para` no `grafo_temas` |
| `consultar_recomendacao(id, k, w1, w2)` | `P` | BFS + Jaccard, ordena candidatos por score |

**`imprimir_relatorio(out)`** — emitida para `stderr` ao final da execução; imprime tabela com colunas `Operacao | Chamadas | Total(ns) | Media(ns)` para Sistema, Grafo Social e Grafo de Temas.

---

## 5. Protocolo de Entrada/Saída

A primeira linha da entrada é sempre `A <L|M|E>`, definindo a representação inicial. Linhas subsequentes contêm um dos comandos abaixo. Toda saída vai para `stdout`; as métricas de tempo vão para `stderr`.

| Comando | Formato | Saída |
|---|---|---|
| `A` | `A <L\|M\|E>` | `A <modo>` |
| `T` | `T <nome> <A\|P>` | `T <id> <nome>` |
| `U` | `U <nome> <idade> <id_t1> ...` | `U <id> <nome>` |
| `S` | `S <id1> <id2>` | `S <id1> <id2>` |
| `R` | `R <id1> <id2>` | `R <id1> <id2>` |
| `LT` | `LT <id>` | `LT <id1> <id2> ...` (ordenados) |
| `LC` | `LC <id>` | `LC <id1> <id2> ...` (ordenados) |
| `LS` | `LS <id>` | `LS <id1> <id2> ...` (ordenados) |
| `LA` | `LA <id>` | `LA <id1> <id2> ...` (ordenados) |
| `Q` | `Q <id1> <id2>` | `Q <0\|1\|2\|3>` |
| `G` | `G <id_u> <id_t>` | `G <0\|1>` |
| `F` | `F <id_t>` | `F <contagem>` |
| `P` | `P <id> <k> <w1> <w2>` | `P <id1> <id2> ...` (top-k) |

Os IDs externos são atribuídos sequencialmente a partir de 0, separadamente para usuários e temas.

---

## 6. Análise de Complexidade

Sejam n = número de usuários, m = número de arestas no `grafo_social`, e grau médio d = m/n.

### Operações do TAD Grafo

| Operação | Lista (L) | Matriz (M) | Arestas (E) |
|---|---|---|---|
| `criar_no` | O(1) | **O(n)** (expande a matriz) | O(1) |
| `criar_aresta` | O(1) amortizado | O(1) | O(1) amortizado |
| `remover_aresta` | O(d) busca na lista | O(1) | **O(m)** busca na lista plana |
| `checar_aresta` | O(d) | **O(1)** | **O(m)** |
| `viz. saída (apontados_por)` | **O(d)** | O(n) varredura de linha | O(m) |
| `viz. entrada (apontam_para)` | O(n + m) | O(n) varredura de coluna | O(m) |
| `viz. bidirecional` | O(d²) | O(n) | O(d × m) |
| `trocar_tipo` | O(n + m) | O(n²) | O(m) |

**Memória:**

| Representação | Custo |
|---|---|
| Lista (L) | O(n + m) |
| Matriz (M) | O(n²) |
| Arestas (E) | O(m) |

### Operações do Sistema

| Operação | Complexidade dominante |
|---|---|
| `adicionar_usuario` | O(k) onde k = nº de temas de interesse |
| `seguir_usuario` / `remover_seguimento` | Igual a `criar_aresta` / `remover_aresta` |
| `consultar_seguidores` | Igual a `viz. entrada` |
| `consultar_seguidos` | Igual a `viz. saída` |
| `consultar_amigos` | Igual a `viz. bidirecional` |
| `consultar_relacao` | 2× `checar_aresta` |
| `consultar_interesse` | 1× `checar_aresta` (no `grafo_temas`) |
| `consultar_popularidade` | Igual a `viz. entrada` (no `grafo_temas`) |
| `consultar_recomendacao` | O(n + m) BFS + O(n × k) Jaccard |

---

## 7. Análise Experimental

### Metodologia

Os experimentos cobrem **36 configurações**: 3 modos (L/M/E) × 2 perfis (atualizações / consultas) × 3 tamanhos (n=50 / 300 / 1000) × 2 densidades (esparso / denso). Para cada configuração, um único arquivo de entrada é gerado com semente determinista e executado com `./bin/tp2.out`, coletando as métricas em `stderr`.

- **Perfil atualizações:** 75% das operações são `S`/`R`; 25% são consultas.
- **Perfil consultas:** 20% das operações são `S`/`R`; 80% são consultas.
- **Rede esparsa:** ~5 seguimentos por usuário em média.
- **Rede densa:** ~50% de densidade no `grafo_social`.

Todos os valores abaixo são **tempos médios por chamada em nanossegundos**, medidos com `std::chrono::high_resolution_clock`.

---

### 7.1 Comparação de modos — Grafo Social (rede média, esparso)

| Operação | L (ns) | M (ns) | E (ns) | Melhor |
|---|---|---|---|---|
| `criar_no` | 220 | 4 804 | 36 | **E** |
| `criar_aresta` | 59 | 34 | 34 | M/E |
| `remover_aresta` | 80 | 39 | 2 702 | **M** |
| `viz. saída` | 90 | 1 973 | 4 293 | **L** |
| `viz. entrada` | 9 892 | 4 584 | 4 255 | **E/M** |
| `viz. bidirecional` | 150 | 1 956 | 13 366 | **L** |
| `checar_aresta` | 54 | 42 | 4 179 | M/L |

**Observações:**

- **Modo L** domina nas consultas de vizinhança de saída e em amigos mútuos porque o acesso à lista de vizinhos é O(grau), sem varrer vértices desnecessários.
- **Modo M** tem `criar_no` ~22× mais lento que L (expande a matriz inteira), mas `checar_aresta` é O(1) e `viz. entrada` é O(n) sem overhead de indireção.
- **Modo E** é o mais rápido para criar nós (sem estrutura por nó), mas paga O(|E|) em todas as consultas de vizinhança e remoção, o que o torna inadequado para workloads com muitas consultas.

---

### 7.2 Escalonamento com o tamanho da rede (viz. entrada, esparso, consultas)

| n | L (ns) | M (ns) | E (ns) |
|---|---|---|---|
| 50 | 1 550 | 746 | 831 |
| 300 | 9 892 | 4 584 | 4 255 |
| 1 000 | 34 860 | 24 292 | 13 791 |

**Razões de crescimento (n=50→300 / n=300→1000):**

- **L:** 6,4× / 3,5× → crescimento **O(n)**, consistente com varredura de todas as listas de adjacência.
- **M:** 6,1× / 5,3× → crescimento **O(n)**, consistente com varredura de coluna de tamanho n.
- **E:** 5,1× / 3,2× → crescimento **O(|E|)** = O(n) para rede esparsa; vantagem relativa cresce com n pois |E| cresce mais lentamente que n² para grafos esparsos.

O `criar_no` no modo M escala quadraticamente com n, como esperado (O(n) por chamada × n chamadas = O(n²) no total):

| n | M criar_no (ns/chamada) |
|---|---|
| 50 | 951 |
| 300 | 4 804 |
| 1 000 | 11 580 |

Razões 5,0× e 2,4× são coerentes com crescimento linear por chamada — o custo total de inserção de todos os n nós é O(n²).

---

### 7.3 Impacto da densidade (rede média, modo L vs M vs E)

| Operação | L esparso | L denso | M esparso | M denso | E esparso | E denso |
|---|---|---|---|---|---|---|
| `viz. saída` | 90 | 106 | 1 973 | 2 212 | 4 293 | 20 520 |
| `viz. entrada` | 9 892 | 26 165 | 4 584 | 4 992 | 4 255 | 20 201 |
| `viz. bidirecional` | 150 | 1 050 | 1 956 | 2 196 | 13 366 | 224 629 |
| `checar_aresta` | 54 | 109 | 42 | 44 | 4 179 | 19 371 |

**Observações:**

- O modo **M** é o menos sensível à densidade em `viz. entrada` e `checar_aresta`: ambas as operações são O(n), independente de m.
- O modo **L** em `viz. entrada` cresce de 9 892 ns para 26 165 ns (2,6×) quando a rede passa de esparsa para densa, pois a varredura percorre mais listas mais longas.
- O modo **E** é o mais afetado pela densidade: `viz. bidirecional` passa de 13 366 ns para 224 629 ns (16,8×) porque cada verificação de aresta reversa é O(|E|) e |E| cresce muito no grafo denso.

---

### 7.4 Perfil de carga — tempo total do workload (rede média, esparso)

| Modo | Atualizações (µs) | Consultas (µs) |
|---|---|---|
| L | ~2,0 | ~15,0 |
| M | ~2,0 | ~9,5 |
| E | ~8,0 | ~58,0 |

- O **modo L** e **M** têm custo total de atualizações similar porque as operações `S`/`R` são baratas nos dois (O(1)/O(d)).
- No perfil de **consultas**, o **modo M** leva vantagem sobre L porque `consultar_seguidores` (viz. entrada) é O(n) na matriz contra O(n + m) nas listas.
- O **modo E** é sistematicamente o mais lento no perfil de consultas: cada operação de vizinhança varre toda a lista de arestas.

---

### 7.5 Sistema — operações de alto nível (rede média, esparso, consultas)

| Operação | L (ns) | M (ns) | E (ns) |
|---|---|---|---|
| `adicionar_usuario` | 1 615 | 11 633 | 1 157 |
| `seguir_usuario` | 391 | 359 | 354 |
| `consultar_seguidores` | 10 453 | 5 132 | 4 738 |
| `consultar_seguidos` | 493 | 2 495 | 4 803 |
| `consultar_popularidade` | 10 334 | 6 031 | 7 535 |
| `consultar_amigos` | 325 | 2 195 | 13 620 |
| `consultar_relacao` | 565 | 552 | 8 892 |
| `consultar_interesse` | 471 | 470 | 5 993 |

- `adicionar_usuario` no modo M é ~7× mais lento que L/E porque cria nós em ambos os grafos, pagando O(n) duas vezes.
- `consultar_seguidores` é a operação mais cara no modo L (10 453 ns) e a segunda mais cara no M (5 132 ns), pois ambos precisam varrer toda a estrutura para encontrar arestas de entrada.
- `consultar_amigos` no modo E (13 620 ns) é ~42× mais lento que no modo L (325 ns) na rede média, ilustrando o custo proibitivo de `viz. bidirecional` quando |E| cresce.

---

## 8. Conclusão

A escolha da representação interna tem impacto direto e significativo no desempenho da rede:

- **Lista de Adjacência (L)** é a representação mais equilibrada para workloads mistos. Oferece O(d) para vizinhança de saída e amigos mútuos, e suporta bem grafos esparsos em memória e tempo. A única operação custosa é `viz. entrada` (O(n + m)), que é intrínseca ao modelo dirigido.

- **Matriz de Adjacência (M)** torna `checar_aresta` e `viz. entrada` mais previsíveis (ambos O(n)), ao custo de O(n²) em memória e O(n) para `criar_no`. É preferível quando o grafo é denso e as operações de entrada são frequentes.

- **Lista de Arestas (E)** minimiza o custo de criação de nós e aresta, e usa O(m) de memória, mas torna todas as consultas de vizinhança O(|E|). Seu desempenho degrada rapidamente com o crescimento do grafo e é inviável para workloads de consulta intensivos em redes médias ou grandes.

A análise experimental confirma as previsões teóricas de complexidade em todas as configurações testadas: o `criar_no` no modo M cresce como O(n) por chamada, a `viz. entrada` em todos os modos cresce como O(n) para redes esparsas, e o modo E se degrada proporcionalmente a |E| nas operações de vizinhança.
