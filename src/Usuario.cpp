#include "Usuario.hpp"
Usuario::Usuario() : id(-1), nome(""), idade(0) {}
Usuario::Usuario(int id, const std::string& nome, int idade): id(id), nome(nome), idade(idade){};
Usuario::~Usuario(){};

int Usuario::get_idade() const{
    return idade;
};
int Usuario::get_id() const{
    return id;
};
const std::string& Usuario::get_nome() const{
    return nome;
};
void Usuario::set_idade(int idade){
    this->idade = idade;
};
void Usuario::set_nome(const std::string& nome){
    this->nome = nome;
};