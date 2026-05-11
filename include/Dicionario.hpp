#ifndef DICIONARIO_HPP
#define DICIONARIO_HPP

#include "Usuario.hpp"
#include "Tema.hpp"
#include "Lista.hpp"
#include "Grafo.hpp"

class Dicionario {
    Lista<Usuario> &usuarios;
    Lista<Tema> &temas;
    Grafo &grafo_temas;
    Grafo &grafo_social;

    Lista<int> map_usuario_social;
    Lista<int> map_usuario_temas;
    Lista<int> map_tema_temas;
    
public:
    Dicionario(Lista<Usuario> &u, Lista<Tema> &t, Grafo &gt, Grafo &gs);
    ~Dicionario();

    void registrar_usuario_social(int id_grafo);
    void registrar_usuario_temas(int id_grafo);
    void registrar_tema_temas(int id_grafo);

    const Usuario& get_usuario(int id) const;
    const Tema& get_tema(int id) const;
    const No& get_no_usuario_grafo_social(int id) const;
    const No& get_no_usuario_grafo_temas(int id) const;
    const No& get_no_tema_grafo_temas(int id) const;
};

#endif