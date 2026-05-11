#include "Sistema.hpp"

Sistema::Sistema(TipoGrafo tipoInicial):    grafo_temas(tipoInicial),                            
                                            grafo_social(tipoInicial),                           
                                            dicionario(usuarios, temas, grafo_temas, grafo_social), 
                                            num_usuarios(0),                                     
                                            num_temas(0)
{

};

Sistema::~Sistema(){};

void Sistema::alterar_armazenamento(TipoGrafo tipo){};
const int Sistema::adicionar_usuario(const std::string& nome, int idade, Lista<int> tema_ids){};
const int Sistema::adicionar_tema(const std::string& nome, TipoTema tipo){};
void Sistema::seguir_usuario(int id1, int id2){};
void Sistema::remover_seguimento_usuario(int id1, int id2){};
void Sistema::consultar_temas(int id_usuario) const{};
void Sistema::consultar_seguidores(int id_usuario) const{};
void Sistema::consultar_seguidos(int id_usuario) const{};
void Sistema::consultar_amigos(int id_usuario) const{};
void Sistema::consultar_relacao(int id1, int id2) const{};
void Sistema::consultar_interesse(int id_usuario, int id_tema) const{};
void Sistema::consultar_popularidade(int id_tema) const{};