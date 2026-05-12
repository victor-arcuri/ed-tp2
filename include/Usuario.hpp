#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

class Usuario {
	int id;
	std::string nome;
	int idade;

  public:
	Usuario();
	Usuario(int id, const std::string &nome, int idade);
	~Usuario();

	int get_idade() const;
	int get_id() const;
	const std::string &get_nome() const;

	void set_idade(int idade);
	void set_nome(const std::string &nome);
};

#endif