#ifndef TEMA_HPP
#define TEMA_HPP

#include <string>

enum TipoTema { ACADEMICO, PESSOAL };

class Tema {
	int id;
	std::string nome;
	TipoTema tipo;

  public:
	Tema();
	Tema(int id, const std::string &nome, TipoTema tipo);
	~Tema();

	int get_tipo() const;
	int get_id() const;
	const std::string &get_nome() const;

	void set_tipo(TipoTema tipo);
	void set_nome(const std::string &nome);
};

#endif