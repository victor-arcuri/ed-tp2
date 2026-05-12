#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Lista.hpp"
#include <stdexcept>

#define MATRIZ_DIMENSOES_INICIAIS 0
#define LISTA_TAMANHO_INICIAL 0

class MatrizAdjacencia {
	int dimensoes;
	Lista<Lista<int>> dados;

  public:
	MatrizAdjacencia(int tamanho_inicial = MATRIZ_DIMENSOES_INICIAIS);
	~MatrizAdjacencia();

	void aumentar_dimensao();
	void alterar_valor(int id1, int id2, int valor, bool bidirecional);
	int obter_valor(int id1, int id2);

	Lista<int> &operator[](int index);
	const Lista<int> &operator[](int index) const;
};

enum TipoNo { USUARIO, TEMA };

struct No {
	TipoNo tipo;
	int id_externo;
	int id_grafo;
};

class ListaAdjacencia {
	int tamanho;
	Lista<Lista<No *>> nos;

  public:
	ListaAdjacencia(int tamanho_inicial = LISTA_TAMANHO_INICIAL);
	~ListaAdjacencia();

	void adicionar_aresta(No *no1, No *no2, bool bidirecional);
	void remover_aresta(No *no1, No *no2, bool bidirecional);
	void aumentar_tamanho();

	Lista<No *> &operator[](int index);
};

enum TipoGrafo { GRAFO_MATRIZ, GRAFO_LISTA };

class Grafo {
	Lista<No *> nos;
	int id_nos;

	TipoGrafo tipo;
	MatrizAdjacencia *matriz;
	ListaAdjacencia *lista;

  public:
	Grafo(TipoGrafo tipo);
	~Grafo();

	const No *retornar_no_por_id_interno(int id_grafo) const;
	int criar_no(TipoNo tipo, int id);
	void criar_aresta(int id_grafo1, int id_grafo2, bool bidirecional);
	void trocar_tipo(TipoGrafo novo_tipo);
	void remover_aresta(int id_interno_grafo1, int id_interno_grafo2, bool bidirecional);

	Lista<No *> obter_vizinhos_apontados_por(int id_interno_grafo) const;
	Lista<No *> obter_vizinhos_que_apontam_para(int id_iterno_grafo) const;
	Lista<No *> obter_vizinhos_bidirecional(int id_iterno_grafo) const;

	bool checar_aresta(int id_interno_grafo1, int id_interno_grafo2) const;
};

#endif