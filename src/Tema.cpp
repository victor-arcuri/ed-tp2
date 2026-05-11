#include "Tema.hpp"

Tema::Tema() : id(-1), nome(""), tipo(PESSOAL) {}
Tema::Tema(int id, const std::string& nome, TipoTema tipo): id(id), nome(nome), tipo(tipo){};
Tema::~Tema(){};

int Tema::get_tipo() const{
    return tipo;
};
int Tema::get_id() const{
    return id;
};
const std::string& Tema::get_nome() const{
    return nome;
};

void Tema::set_tipo(TipoTema tipo){
    this->tipo = tipo;
};
void Tema::set_nome(const std::string& nome){
    this->nome = nome;
};
