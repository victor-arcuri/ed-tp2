#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <stdexcept>
#include "Lista.hpp"

#define MATRIZ_DIMENSOES_INICIAIS 0

#define LISTA_TAMANHO_INICIAL 0

class MatrizAdjacencia {
    int dimensoes;
    Lista<Lista<int>> dados;

public:
    MatrizAdjacencia(int tamanho_inicial = MATRIZ_DIMENSOES_INICIAIS) : dimensoes(tamanho_inicial) {
        for (int i = 0; i < dimensoes; i++) {
            Lista<int> linha_vazia;
            for (int j = 0; j < dimensoes; j++){
                linha_vazia.inserir(0);
            }
            dados.inserir(linha_vazia); 
        }
    };

    void aumentar_dimensao(){
        for (int i = 0; i < dimensoes; i++){
            dados[i].inserir(0);
        }
        dimensoes++;
        Lista<int> linha_vazia;
        for (int i = 0; i < dimensoes; i++){
            linha_vazia.inserir(0);
        }
        dados.inserir(linha_vazia);
    }

    void alterar_valor(int id1, int id2, int valor, bool bidirecional){
        if (valor != 0 && valor != 1) throw std::invalid_argument("Erro: O valor da aresta deve ser 0 ou 1.");
        dados[id1][id2] = valor;
        if (bidirecional) dados[id2][id1] = valor;
    }

    int obter_valor(int id1, int id2){
        if ((id1 < 0 || id1 >= dimensoes) || (id2 < 0 || id2 >= dimensoes)) throw std::out_of_range("Erro: Indíce(s) inválido(s).");
        return dados[id1][id2];
    }

    Lista<int>& operator[](int index) {
        if (index < 0 || index >= dimensoes) throw std::out_of_range("Erro: Indíce(s) inválido(s).");
        return dados[index];
    }

    const Lista<int>& operator[](int index) const {
        if (index < 0 || index >= dimensoes) throw std::out_of_range("Erro: Indíce(s) inválido(s).");
        return dados[index];
    }

    ~MatrizAdjacencia() {};

};

enum TipoNo {
    USUARIO,
    TEMA
};

struct No {
    TipoNo tipo;
    int id_externo;
    int id_grafo;
};

class ListaAdjacencia{
    int tamanho;
    Lista<Lista<No*>> nos;

public:
    ListaAdjacencia(int tamanho_inicial = LISTA_TAMANHO_INICIAL): tamanho(tamanho_inicial){
        for (int i = 0; i < tamanho; i++) {
            Lista<No*> linha_vazia;
            nos.inserir(linha_vazia);
        }
    };
    ~ListaAdjacencia() {};

    void adicionar_aresta(No* no1, No* no2, bool bidirecional){
        nos[no1->id_grafo].inserir(no2);
        if (bidirecional) nos[no2->id_grafo].inserir(no1);
    }

    void aumentar_tamanho(){
        Lista<No*> vizinhos_vazios; 
        nos.inserir(vizinhos_vazios);
        tamanho++;
    }

    Lista<No*>& operator[](int index) {
        return nos[index];
    }
};

enum TipoGrafo{
    GRAFO_MATRIZ,
    GRAFO_LISTA
};

class Grafo{
    Lista<No*> nos;
    int id_nos;

    TipoGrafo tipo;
    MatrizAdjacencia* matriz;
    ListaAdjacencia* lista;

public:
    Grafo(TipoGrafo tipo): id_nos(0), tipo(tipo){
        matriz = nullptr;
        lista = nullptr;
        
        if (tipo == GRAFO_MATRIZ) {
            matriz = new MatrizAdjacencia(0);
        } else {
            lista = new ListaAdjacencia(0);
        }
    };
    ~Grafo(){
        for (int i = 0; i < nos.get_tamanho(); i++) {
            delete nos[i];
        }
        if (matriz != nullptr) delete matriz;
        if (lista != nullptr) delete lista;
    }
    int criar_no(TipoNo tipo, int id){
        No* novo_no = new No;
        novo_no->id_externo = id;
        novo_no->id_grafo = id_nos;
        novo_no->tipo = tipo;
        id_nos++;
        nos.inserir(novo_no);
        if (this->tipo == GRAFO_MATRIZ){
            matriz->aumentar_dimensao();
        } 
        else if (this->tipo == GRAFO_LISTA){
            lista->aumentar_tamanho();
        }
        return id_nos-1;
    }

    void criar_aresta(int id_grafo1, int id_grafo2, bool bidirecional){
        if (this->tipo == GRAFO_MATRIZ){
            matriz->alterar_valor(id_grafo1, id_grafo2, 1, bidirecional);
        } 
        else if (this->tipo == GRAFO_LISTA){
            lista->adicionar_aresta(nos[id_grafo1], nos[id_grafo2], bidirecional);
        }
    }

    void trocar_tipo(TipoGrafo novo_tipo) {
        if (this->tipo == novo_tipo) return;

        int qtd_nos = nos.get_tamanho();

        if (novo_tipo == GRAFO_MATRIZ) {
            matriz = new MatrizAdjacencia(qtd_nos);
            for (int i = 0; i < qtd_nos; i++) {
                Lista<No*>& vizinhos = (*lista)[i]; 
                
                for (int j = 0; j < vizinhos.get_tamanho(); j++) {
                    int id_vizinho = vizinhos[j]->id_grafo;
                    matriz->alterar_valor(i, id_vizinho, 1, false);
                }
            }
            
            delete lista;
            lista = nullptr;

        } else if (novo_tipo == GRAFO_LISTA) {
            lista = new ListaAdjacencia(qtd_nos);
            
            for (int i = 0; i < qtd_nos; i++) {
                for (int j = 0; j < qtd_nos; j++) {
                    if (matriz->obter_valor(i, j) == 1) { 
                        lista->adicionar_aresta(nos[i], nos[j], false);
                    }
                }
            }
            delete matriz;
            matriz = nullptr;
        }
        this->tipo = novo_tipo; 
    }

};

#endif