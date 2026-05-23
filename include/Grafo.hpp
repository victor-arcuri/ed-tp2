#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Lista.hpp"
#include <stdexcept>

#define MATRIZ_DIMENSOES_INICIAIS 0
#define LISTA_TAMANHO_INICIAL 0

/**
 * Classe MatrizAdjacencia
 * ---------------------------------------------------
 * Representação de grafo por matriz de adjacência quadrada implementada
 * como Lista<Lista<int>>. Cada posição [i][j] vale 1 se há aresta de i
 * para j, e 0 caso contrário.
 */
class MatrizAdjacencia {
	int dimensoes;
	Lista<Lista<int>> dados;

  public:
	MatrizAdjacencia(int tamanho_inicial = MATRIZ_DIMENSOES_INICIAIS);
	~MatrizAdjacencia();

	/**
	 * Função aumentar_dimensao
	 * ---------------------------------------------------
	 * Acrescenta uma nova linha e coluna à matriz, expandindo-a para
	 * acomodar um novo nó no grafo.
	 */
	void aumentar_dimensao();

	/**
	 * Função alterar_valor
	 * ---------------------------------------------------
	 * Define o valor de uma célula da matriz de adjacência.
	 *
	 * id1: índice interno do nó de origem.
	 * id2: índice interno do nó de destino.
	 * valor: 0 (sem aresta) ou 1 (com aresta).
	 * bidirecional: se verdadeiro, também atualiza a posição [id2][id1].
	 */
	void alterar_valor(int id1, int id2, int valor, bool bidirecional);

	/**
	 * Função obter_valor
	 * ---------------------------------------------------
	 * Retorna o valor da célula [id1][id2] da matriz.
	 *
	 * id1: índice interno do nó de origem.
	 * id2: índice interno do nó de destino.
	 *
	 * retorno: 1 se há aresta de id1 para id2, 0 caso contrário.
	 * Lança exceção se os índices forem inválidos.
	 */
	int obter_valor(int id1, int id2);

	Lista<int> &operator[](int index);
	const Lista<int> &operator[](int index) const;
};

/**
 * enum TipoNo
 * ---------------------------------------------------
 * Diferencia vértices de usuários (USUARIO) de vértices de temas (TEMA)
 * dentro de um mesmo Grafo.
 */
enum TipoNo { USUARIO, TEMA };

/**
 * struct No
 * ---------------------------------------------------
 * Representa um vértice do grafo. Associa o índice interno de posição no
 * array de nós (id_grafo) ao identificador externo de domínio (id_externo)
 * e ao tipo do vértice (USUARIO ou TEMA).
 */
struct No {
	TipoNo tipo;
	int id_externo;
	int id_grafo;
};

/**
 * Classe ListaAdjacencia
 * ---------------------------------------------------
 * Representação de grafo por lista de adjacência usando Lista<Lista<No*>>.
 * Cada linha i armazena ponteiros para os nós vizinhos de saída do nó i.
 */
class ListaAdjacencia {
	int tamanho;
	Lista<Lista<No *>> nos;

  public:
	ListaAdjacencia(int tamanho_inicial = LISTA_TAMANHO_INICIAL);
	~ListaAdjacencia();

	/**
	 * Função adicionar_aresta
	 * ---------------------------------------------------
	 * Insere no1→no2 na lista de adjacência. Se bidirecional, também insere no2→no1.
	 *
	 * no1: ponteiro para o nó de origem.
	 * no2: ponteiro para o nó de destino.
	 * bidirecional: se verdadeiro, adiciona a aresta nos dois sentidos.
	 */
	void adicionar_aresta(No *no1, No *no2, bool bidirecional);

	/**
	 * Função remover_aresta
	 * ---------------------------------------------------
	 * Remove a aresta no1→no2 da lista de adjacência. Se bidirecional, também remove no2→no1.
	 *
	 * no1: ponteiro para o nó de origem.
	 * no2: ponteiro para o nó de destino.
	 * bidirecional: se verdadeiro, remove a aresta nos dois sentidos.
	 */
	void remover_aresta(No *no1, No *no2, bool bidirecional);

	/**
	 * Função aumentar_tamanho
	 * ---------------------------------------------------
	 * Adiciona uma nova lista vazia de vizinhos para acomodar um novo nó no grafo.
	 */
	void aumentar_tamanho();

	Lista<No *> &operator[](int index);
};

/**
 * struct Aresta
 * ---------------------------------------------------
 * Par (id_origem, id_destino) que representa uma aresta dirigida entre dois
 * nós pelos seus índices internos no grafo.
 */
struct Aresta {
	int id_origem;
	int id_destino;
};

/**
 * Classe ListaArestas
 * ---------------------------------------------------
 * Terceira representação interna do grafo, otimizada para grafos esparsos.
 * Armazena apenas as arestas existentes em uma Lista<Aresta>, sem alocar
 * memória proporcional ao quadrado do número de nós.
 */
class ListaArestas {
	Lista<Aresta> arestas;

  public:
	ListaArestas();
	~ListaArestas();

	/**
	 * Função adicionar_aresta
	 * ---------------------------------------------------
	 * Insere a aresta (id1→id2) na lista. Se bidirecional, também insere (id2→id1).
	 *
	 * id1: índice interno do nó de origem.
	 * id2: índice interno do nó de destino.
	 * bidirecional: se verdadeiro, adiciona a aresta nos dois sentidos.
	 */
	void adicionar_aresta(int id1, int id2, bool bidirecional);

	/**
	 * Função remover_aresta
	 * ---------------------------------------------------
	 * Remove a aresta (id1→id2) da lista. Se bidirecional, também remove (id2→id1).
	 *
	 * id1: índice interno do nó de origem.
	 * id2: índice interno do nó de destino.
	 * bidirecional: se verdadeiro, remove a aresta nos dois sentidos.
	 */
	void remover_aresta(int id1, int id2, bool bidirecional);

	/**
	 * Função checar_aresta
	 * ---------------------------------------------------
	 * Verifica se existe a aresta (id1→id2) na lista.
	 *
	 * id1: índice interno do nó de origem.
	 * id2: índice interno do nó de destino.
	 *
	 * retorno: true se a aresta existir, false caso contrário.
	 */
	bool checar_aresta(int id1, int id2) const;

	/**
	 * Função obter_destinos_de
	 * ---------------------------------------------------
	 * Retorna os índices internos de todos os nós alcançáveis a partir de id_origem.
	 *
	 * id_origem: índice interno do nó de saída.
	 *
	 * retorno: Lista<int> com os índices internos dos nós de destino.
	 */
	Lista<int> obter_destinos_de(int id_origem) const;

	/**
	 * Função obter_origens_de
	 * ---------------------------------------------------
	 * Retorna os índices internos de todos os nós que possuem aresta apontando para id_destino.
	 *
	 * id_destino: índice interno do nó de entrada.
	 *
	 * retorno: Lista<int> com os índices internos dos nós de origem.
	 */
	Lista<int> obter_origens_de(int id_destino) const;

	/**
	 * Função get_arestas
	 * ---------------------------------------------------
	 * Retorna a coleção interna de arestas, usada durante a migração de representação.
	 *
	 * retorno: referência constante à Lista<Aresta> com todas as arestas armazenadas.
	 */
	const Lista<Aresta> &get_arestas() const;
};

/**
 * enum TipoGrafo
 * ---------------------------------------------------
 * Define a representação interna usada pelo Grafo: GRAFO_MATRIZ (matriz de adjacência),
 * GRAFO_LISTA (lista de adjacência) ou GRAFO_ARESTA (lista de arestas, ponto extra).
 */
enum TipoGrafo { GRAFO_MATRIZ, GRAFO_LISTA, GRAFO_ARESTA };

/**
 * Classe Grafo
 * ---------------------------------------------------
 * TAD genérico de grafo. Mantém uma lista de nós e uma das três representações
 * internas intercambiáveis em tempo de execução: MatrizAdjacencia, ListaAdjacencia
 * ou ListaArestas.
 */
class Grafo {
	Lista<No *> nos;
	int id_nos;

	TipoGrafo tipo;
	MatrizAdjacencia *matriz;
	ListaAdjacencia *lista;
	ListaArestas *lista_arestas;

  public:
	Grafo(TipoGrafo tipo);
	~Grafo();

	/**
	 * Função retornar_no_por_id_interno
	 * ---------------------------------------------------
	 * Retorna o ponteiro para o nó de índice interno id_grafo.
	 *
	 * id_grafo: índice interno do nó no array de nós do grafo.
	 *
	 * retorno: ponteiro constante para o No, ou nullptr se o índice for inválido.
	 */
	const No *retornar_no_por_id_interno(int id_grafo) const;

	/**
	 * Função criar_no
	 * ---------------------------------------------------
	 * Cria e registra um novo vértice no grafo, expandindo a representação interna.
	 *
	 * tipo: TipoNo do novo vértice (USUARIO ou TEMA).
	 * id: identificador externo de domínio associado ao vértice.
	 *
	 * retorno: índice interno (id_grafo) atribuído ao novo nó.
	 */
	int criar_no(TipoNo tipo, int id);

	/**
	 * Função criar_aresta
	 * ---------------------------------------------------
	 * Cria uma aresta entre dois vértices identificados por seus índices internos.
	 *
	 * id_grafo1: índice interno do nó de origem.
	 * id_grafo2: índice interno do nó de destino.
	 * bidirecional: se verdadeiro, cria a aresta nos dois sentidos.
	 */
	void criar_aresta(int id_grafo1, int id_grafo2, bool bidirecional);

	/**
	 * Função trocar_tipo
	 * ---------------------------------------------------
	 * Migra todas as arestas para uma nova representação interna, liberando a antiga.
	 * Permite alternar entre GRAFO_LISTA, GRAFO_MATRIZ e GRAFO_ARESTA em tempo de
	 * execução sem perda de dados.
	 *
	 * novo_tipo: a representação interna desejada após a troca.
	 */
	void trocar_tipo(TipoGrafo novo_tipo);

	/**
	 * Função remover_aresta
	 * ---------------------------------------------------
	 * Remove a aresta entre dois vértices identificados por seus índices internos.
	 *
	 * id_interno_grafo1: índice interno do nó de origem.
	 * id_interno_grafo2: índice interno do nó de destino.
	 * bidirecional: se verdadeiro, remove a aresta nos dois sentidos.
	 */
	void remover_aresta(int id_interno_grafo1, int id_interno_grafo2, bool bidirecional);

	/**
	 * Função obter_vizinhos_apontados_por
	 * ---------------------------------------------------
	 * Retorna todos os nós para os quais o nó de índice id_interno_grafo aponta
	 * (vizinhos de saída).
	 *
	 * id_interno_grafo: índice interno do nó de consulta.
	 *
	 * retorno: Lista<No*> com os vizinhos de saída do nó.
	 */
	Lista<No *> obter_vizinhos_apontados_por(int id_interno_grafo) const;

	/**
	 * Função obter_vizinhos_que_apontam_para
	 * ---------------------------------------------------
	 * Retorna todos os nós que possuem aresta apontando para o nó de índice
	 * id_interno_grafo (vizinhos de entrada).
	 *
	 * id_interno_grafo: índice interno do nó de consulta.
	 *
	 * retorno: Lista<No*> com os vizinhos de entrada do nó.
	 */
	Lista<No *> obter_vizinhos_que_apontam_para(int id_iterno_grafo) const;

	/**
	 * Função obter_vizinhos_bidirecional
	 * ---------------------------------------------------
	 * Retorna os nós que possuem arestas bidirecionais com o nó de índice
	 * id_interno_grafo, ou seja, nós v tal que existem arestas u→v e v→u
	 * (amigos mútuos no grafo social).
	 *
	 * id_interno_grafo: índice interno do nó de consulta.
	 *
	 * retorno: Lista<No*> com os vizinhos bidirecionais do nó.
	 */
	Lista<No *> obter_vizinhos_bidirecional(int id_iterno_grafo) const;

	/**
	 * Função checar_aresta
	 * ---------------------------------------------------
	 * Verifica se existe aresta do nó id_interno_grafo1 para o nó id_interno_grafo2.
	 *
	 * id_interno_grafo1: índice interno do nó de origem.
	 * id_interno_grafo2: índice interno do nó de destino.
	 *
	 * retorno: true se a aresta existir, false caso contrário.
	 */
	bool checar_aresta(int id_interno_grafo1, int id_interno_grafo2) const;
};

#endif