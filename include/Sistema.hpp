#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include "Usuario.hpp"
#include "Tema.hpp"
#include "Lista.hpp"
#include "Dicionario.hpp"
#include "Grafo.hpp"

class Sistema {
    Lista<Usuario> usuarios;
    Lista<Tema> temas;
    Grafo grafo_temas;
    Grafo grafo_social;
    Dicionario dicionario;

    int num_usuarios;
    int num_temas;


public:
    Sistema();
    ~Sistema();

    void alterar_armazenamento(TipoGrafo tipo);
    const int adicionar_usuario(std::string& nome, int idade, Lista<int> tema_ids);
    const int adicionar_tema(std::string& nome, TipoTema tipo);
    void seguir_usuario(int id1, int id2);
    void remover_seguimento_usuario(int id1, int id2);
    void consultar_temas(int id_usuario) const;
    void consultar_seguidores(int id_usuario) const;
    void consultar_seguidos(int id_usuario) const;
    void consultar_seguidos(int id_usuario) const;
    void consultar_amigos(int id_usuario) const;
    void consultar_relacao(int id1, int id2) const;
    void consultar_interesse(int id_usuario, int id_tema) const;
    void consultar_popularidade(int id_tema) const;
    
};

#endif