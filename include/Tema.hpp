#ifndef TEMA_HPP
#define TEMA_HPP

#include <string>

/**
 * enum TipoTema
 * ---------------------------------------------------
 * Classifica um tema como ACADEMICO ou PESSOAL.
 */
enum TipoTema { ACADEMICO, PESSOAL };

/**
 * Classe Tema
 * ---------------------------------------------------
 * Representa um tema de interesse da rede social, armazenando nome e tipo.
 * Instâncias são mantidas na Lista<Tema> do Sistema e referenciadas pelo
 * grafo_temas via struct No.
 */
class Tema {
	int id;
	std::string nome;
	TipoTema tipo;

  public:
	Tema();
	Tema(int id, const std::string &nome, TipoTema tipo);
	~Tema();

	/**
	 * Função get_tipo
	 * ---------------------------------------------------
	 * Retorna o tipo do tema.
	 *
	 * retorno: número inteiro representando o TipoTema (ACADEMICO ou PESSOAL).
	 */
	int get_tipo() const;

	/**
	 * Função get_id
	 * ---------------------------------------------------
	 * Retorna o identificador externo do tema.
	 *
	 * retorno: número inteiro representando o id do tema.
	 */
	int get_id() const;

	/**
	 * Função get_nome
	 * ---------------------------------------------------
	 * Retorna o nome do tema.
	 *
	 * retorno: referência constante à string com o nome do tema.
	 */
	const std::string &get_nome() const;

	void set_tipo(TipoTema tipo);
	void set_nome(const std::string &nome);
};

#endif