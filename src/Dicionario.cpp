#include "Dicionario.hpp"
#include <exception>
Dicionario::Dicionario(Lista<Usuario> &u, Lista<Tema> &t, Grafo &gt, Grafo &gs):    usuarios(u),
                                                                                    temas(t),
                                                                                    grafo_temas(gt),
                                                                                    grafo_social(gs)
{};

Dicionario::~Dicionario() {};

void Dicionario::registrar_usuario_social(int id_grafo) {
    map_usuario_social.inserir(id_grafo);
}

void Dicionario::registrar_usuario_temas(int id_grafo) {
    map_usuario_temas.inserir(id_grafo);
}

void Dicionario::registrar_tema_temas(int id_grafo) {
    map_tema_temas.inserir(id_grafo);
}

const Usuario& Dicionario::get_usuario(int id) const {
    return usuarios.obter(id);
};
const Tema& Dicionario::get_tema(int id) const {
    return temas.obter(id);
};
const No& Dicionario::get_no_usuario_grafo_social(int id) const {
    int id_interno = map_usuario_social.obter(id); 
    
    const No* no_ptr = grafo_social.retornar_no_por_id_interno(id_interno);
    
    if (no_ptr != nullptr) return *no_ptr;
    throw std::invalid_argument("Erro! Id inválido");
};
const No& Dicionario::get_no_usuario_grafo_temas(int id) const {
    int id_interno = map_usuario_temas.obter(id); 
    
    const No* no_ptr = grafo_temas.retornar_no_por_id_interno(id_interno);
    
    if (no_ptr != nullptr) return *no_ptr;
    throw std::invalid_argument("Erro! Id inválido");
};
const No& Dicionario::get_no_tema_grafo_temas(int id) const {
    int id_interno = map_tema_temas.obter(id); 
    
    const No* no_ptr = grafo_temas.retornar_no_por_id_interno(id_interno);
    
    if (no_ptr != nullptr) return *no_ptr;
    throw std::invalid_argument("Erro! Id inválido");
};
