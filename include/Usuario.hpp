#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

/**
 * Classe Usuario
 * ---------------------------------------------------
 * Representa um usuário da rede social, armazenando seus dados de perfil
 * (id, nome e idade). Instâncias são mantidas na Lista<Usuario> do Sistema
 * e referenciadas pelos grafos via struct No.
 */
class Usuario {
	int id;
	std::string nome;
	int idade;

  public:
	Usuario();
	Usuario(int id, const std::string &nome, int idade);
	~Usuario();

	/**
	 * Função get_idade
	 * ---------------------------------------------------
	 * Retorna a idade do usuário.
	 *
	 * retorno: número inteiro representando a idade.
	 */
	int get_idade() const;

	/**
	 * Função get_id
	 * ---------------------------------------------------
	 * Retorna o identificador externo do usuário.
	 *
	 * retorno: número inteiro representando o id do usuário.
	 */
	int get_id() const;

	/**
	 * Função get_nome
	 * ---------------------------------------------------
	 * Retorna o nome do usuário.
	 *
	 * retorno: referência constante à string com o nome do usuário.
	 */
	const std::string &get_nome() const;

	void set_idade(int idade);
	void set_nome(const std::string &nome);
};

#endif