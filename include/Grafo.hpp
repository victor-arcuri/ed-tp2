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

    void remover_aresta(No* no1, No* no2, bool bidirecional){
        int pos = nos[no1->id_grafo].encontrar(no2);
        if (pos == -1) return;
        nos[no1->id_grafo].remover(pos);
        if (!bidirecional) return;
        pos = nos[no2->id_grafo].encontrar(no1);
        if (pos == -1) return;
        nos[no2->id_grafo].remover(pos);
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

    const No* retornar_no_por_id_interno(int id_grafo) const {
        if (id_grafo >= 0 && id_grafo < nos.get_tamanho()) {
            return nos[id_grafo];
        }
        return nullptr;
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

    Lista<No*> obter_vizinhos_apontados_por(int id_interno_grafo) const{
        if (tipo == GRAFO_MATRIZ){
            Lista<No*> vizinhos;
            const Lista<int>& vizinhosMatriz = (*matriz)[id_interno_grafo];
            for (int i = 0; i < vizinhosMatriz.get_tamanho(); i++){
                if (vizinhosMatriz[i] == 1) vizinhos.inserir(nos[i]); 
            }
            return vizinhos;
        } else if (tipo == GRAFO_LISTA){
            return (*lista)[id_interno_grafo];
        }
        return Lista<No*>();
    }

    Lista<No*> obter_vizinhos_que_apontam_para(int id_iterno_grafo) const{
        if (tipo == GRAFO_MATRIZ){
            Lista<No*> vizinhos;

            for (int i = 0; i < nos.get_tamanho(); i++){
                if ((*matriz)[i][id_iterno_grafo] == 1) vizinhos.inserir(nos[i]);
            }
            return vizinhos;
        } else if (tipo == GRAFO_LISTA){
            Lista<No*> vizinhos;
            for (int i = 0; i < nos.get_tamanho(); i++){
                const Lista<No*>& vizinhos_do_elemento = (*lista)[i];
                for (int j = 0; j < vizinhos_do_elemento.get_tamanho(); j++){
                    if (vizinhos_do_elemento[j]->id_grafo == id_iterno_grafo){
                        vizinhos.inserir(nos[i]);
                        break;
                    }
                }
            }
            return vizinhos;
        }
        return Lista<No*>();
    }

    Lista<No*> obter_vizinhos_bidirecional(int id_iterno_grafo) const {
        Lista<No*> vizinhos;

        if (tipo == GRAFO_MATRIZ) {
            const Lista<int>& linha = (*matriz)[id_iterno_grafo];
            
            for (int i = 0; i < nos.get_tamanho(); i++) {
                if (linha[i] == 1 && (*matriz)[i][id_iterno_grafo] == 1) {
                    vizinhos.inserir(nos[i]);
                }
            }
        } 
        else if (tipo == GRAFO_LISTA) {
            const Lista<No*>& seguidos_1 = (*lista)[id_iterno_grafo];
            
            for (int i = 0; i < seguidos_1.get_tamanho(); i++) {
                No* possivel_bidirecional = seguidos_1[i];
                int id_possivel = possivel_bidirecional->id_grafo;
                
                const Lista<No*>& seguidos_2 = (*lista)[id_possivel];
                bool bidirecional = false;
                
                for (int j = 0; j < seguidos_2.get_tamanho(); j++) {
                    if (seguidos_2[j]->id_grafo == id_iterno_grafo) {
                        bidirecional = true;
                        break;
                    }
                }
                
                if (bidirecional) {
                    vizinhos.inserir(possivel_bidirecional);
                }
            }
        }
        return vizinhos;
    }

    bool checar_aresta(int id_interno_grafo1, int id_interno_grafo2) const {
        if (tipo == GRAFO_MATRIZ) {
            const Lista<int>& linha1 = (*matriz)[id_interno_grafo1];
            if (linha1[id_interno_grafo2] == 1) return true;
        } 
        else if (tipo == GRAFO_LISTA) {
            const Lista<No*>& vizinhos = (*lista)[id_interno_grafo1];
            for (int i = 0; i < vizinhos.get_tamanho(); i++){
                if (vizinhos[i]->id_grafo == id_interno_grafo2){
                    return true;
                }
            }    
        }
        return false;
    }

    void remover_aresta(int id_interno_grafo1, int id_interno_grafo2, bool bidirecional) {
        if (tipo == GRAFO_MATRIZ) {
            matriz->alterar_valor(id_interno_grafo1, id_interno_grafo2, 0, bidirecional);
        } 
        else if (tipo == GRAFO_LISTA) {
            lista->remover_aresta(nos[id_interno_grafo1], nos[id_interno_grafo2], bidirecional);
        }
    }

};

#endif