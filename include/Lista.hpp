#ifndef LISTA_HPP
#define LISTA_HPP

#include <stdexcept>

#define CAPACIDADE_INICIAL 2
#define FATOR_CRESCIMENTO 2

/**
 * Classe Lista
 * ---------------------------------------------------
 * Vetor dinâmico genérico com estratégia de duplicação de capacidade.
 * Único contêiner de sequência permitido no projeto.
 */
template <typename T> class Lista {
	int capacidade;
	int tamanho;
	T *dados;

	/**
	 * Função redimensionar
	 * ---------------------------------------------------
	 * Dobra a capacidade de armazenamento da Lista, realocando a memória e
	 * copiando os elementos para o novo espaço.
	 */
	void redimensionar() {
		capacidade *= FATOR_CRESCIMENTO;
		T *novos_dados = new T[capacidade];
		for (int i = 0; i < tamanho; i++) {
			novos_dados[i] = dados[i];
		}
		delete[] dados;
		dados = novos_dados;
	}

  public:
	Lista() : capacidade(CAPACIDADE_INICIAL), tamanho(0) { dados = new T[capacidade]; }

	~Lista() { delete[] dados; }

	Lista(const Lista &outro) : capacidade(outro.capacidade), tamanho(outro.tamanho) {
		dados = new T[capacidade];
		for (int i = 0; i < tamanho; i++) {
			dados[i] = outro.dados[i];
		}
	}

	Lista &operator=(const Lista &outro) {
		if (this == &outro)
			return *this;
		delete[] dados;
		capacidade = outro.capacidade;
		tamanho = outro.tamanho;
		dados = new T[capacidade];
		for (int i = 0; i < tamanho; i++) {
			dados[i] = outro.dados[i];
		}
		return *this;
	}

	/**
	 * Função get_tamanho
	 * ---------------------------------------------------
	 * Retorna o número de elementos armazenados na Lista.
	 *
	 * retorno: número inteiro representando a quantidade de elementos.
	 */
	int get_tamanho() const { return tamanho; }

	/**
	 * Função inserir
	 * ---------------------------------------------------
	 * Insere um elemento ao final da Lista, redimensionando se necessário.
	 *
	 * elemento: referência constante ao elemento a ser inserido.
	 */
	void inserir(const T &elemento) {
		if (tamanho == capacidade) {
			redimensionar();
		}
		dados[tamanho] = elemento;
		tamanho++;
	}

	/**
	 * Função encontrar
	 * ---------------------------------------------------
	 * Busca linearmente um elemento na Lista e retorna sua posição.
	 *
	 * elemento: referência constante ao elemento a ser buscado.
	 *
	 * retorno: índice do elemento na Lista, ou -1 caso não seja encontrado.
	 */
	int encontrar(const T &elemento) const {
		for (int i = 0; i < tamanho; i++) {
			if (dados[i] == elemento) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * Função remover
	 * ---------------------------------------------------
	 * Remove o elemento na posição especificada, deslocando os elementos
	 * subsequentes para manter a ordem original.
	 *
	 * pos: índice do elemento a ser removido.
	 *
	 * retorno: void. Lança exceção se o índice for inválido.
	 */
	void remover(int pos) {
		if (pos < 0 || pos >= tamanho) {
			throw std::out_of_range("Erro: índice inválido!");
		}
		for (int i = pos; i < tamanho - 1; i++) {
			dados[i] = dados[i + 1];
		}
		tamanho--;
	}

	/**
	 * Função obter
	 * ---------------------------------------------------
	 * Retorna uma referência ao elemento na posição especificada.
	 *
	 * pos: índice do elemento desejado.
	 *
	 * retorno: referência ao elemento na posição. Lança exceção se o índice for inválido.
	 */
	T &obter(int pos) {
		if (pos < 0 || pos >= tamanho) {
			throw std::out_of_range("Erro: índice inválido!");
		}
		return dados[pos];
	}

	const T &obter(int pos) const {
		if (pos < 0 || pos >= tamanho) {
			throw std::out_of_range("Erro: índice inválido!");
		}
		return dados[pos];
	}

	T &operator[](int pos) { return obter(pos); }
	const T &operator[](int pos) const { return obter(pos); }
};

#endif