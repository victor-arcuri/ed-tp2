#ifndef LISTA_HPP
#define LISTA_HPP

#include <stdexcept>

#define CAPACIDADE_INICIAL 2
#define FATOR_CRESCIMENTO 2

template<typename T>
class Lista{
    int capacidade;
    int tamanho;
    T* dados;

    void redimensionar(){
        capacidade*=FATOR_CRESCIMENTO;
        T* novos_dados = new T[capacidade];
        for (int i = 0; i < tamanho; i++){
            novos_dados[i] = dados[i];
        }
        delete[] dados;
        dados = novos_dados;
    };
    
    public:
        Lista(): capacidade(CAPACIDADE_INICIAL), tamanho(0){
            dados = new T[capacidade];
        };
        ~Lista(){
            delete[] dados;
        };
        
        int get_tamanho() const{
            return tamanho;
        }

        void inserir(const T& elemento){
            if (tamanho == capacidade){
                redimensionar();
            }
            dados[tamanho] = elemento;
            tamanho++;
        };

        void remover(int pos){
            if (pos < 0 || pos >= tamanho - 1) {
                throw std::out_of_range("Erro: índice inválido!");
            }
            for (int i = pos; i < tamanho; i++){
                dados[i] = dados[i+1];
            }
            tamanho--;
        }

        T& obter(int pos){
            if (pos < 0 || pos >= tamanho) {
                throw std::out_of_range("Erro: índice inválido!");
            }
            return dados[pos];
        };

        const T& obter(int pos) const {
            if (pos < 0 || pos >= tamanho) {
                throw std::out_of_range("Erro: índice inválido!");
            }
            return dados[pos];
        };

        T& operator[](int pos){
            return obter(pos);
        };
        const T& operator[](int pos) const{
            return obter(pos);
        };
};

#endif