#include "Sistema.hpp"

Sistema::Sistema(TipoGrafo tipoInicial):    grafo_temas(tipoInicial),                            
                                            grafo_social(tipoInicial),                           
                                            dicionario(usuarios, temas, grafo_temas, grafo_social), 
                                            num_usuarios(0),                                     
                                            num_temas(0)
{

};

Sistema::~Sistema(){};

void Sistema::alterar_armazenamento(TipoGrafo tipo){
    grafo_social.trocar_tipo(tipo);
    grafo_temas.trocar_tipo(tipo);
};

const int Sistema::adicionar_usuario(const std::string& nome, int idade, Lista<int> tema_ids){
    Usuario usuario = Usuario(num_usuarios, nome, idade);
    usuarios.inserir(usuario);
    int id_interno_social = grafo_social.criar_no(USUARIO, num_usuarios);
    int id_interno_temas = grafo_temas.criar_no(USUARIO, num_usuarios);
    
    dicionario.registrar_usuario_social(id_interno_social);
    dicionario.registrar_usuario_temas(id_interno_temas);

    for (int i = 0; i < tema_ids.get_tamanho(); i++){
        int id_tema_desejado = tema_ids[i];
        const No& no_tema = dicionario.get_no_tema_grafo_temas(id_tema_desejado);
        grafo_temas.criar_aresta(id_interno_temas, no_tema.id_grafo, false);
    }
    int id_criado = num_usuarios;
    num_usuarios++;
    return id_criado;
};

const int Sistema::adicionar_tema(const std::string& nome, TipoTema tipo){
    Tema tema(num_temas, nome, tipo);
    temas.inserir(tema);
    int id_interno_temas = grafo_temas.criar_no(TEMA, num_temas);
    dicionario.registrar_tema_temas(id_interno_temas);
    int id_criado = num_temas;
    num_temas++;
    return id_criado;
};

void Sistema::seguir_usuario(int id1, int id2){
    const No& no1 = dicionario.get_no_usuario_grafo_social(id1);
    const No& no2 = dicionario.get_no_usuario_grafo_social(id2);
    grafo_social.criar_aresta(no1.id_grafo, no2.id_grafo, false);
};

void Sistema::remover_seguimento_usuario(int id1, int id2){
    const No& no_usuario1 = dicionario.get_no_usuario_grafo_social(id1);
    int id_interno1 = no_usuario1.id_grafo;

    const No& no_usuario2 = dicionario.get_no_usuario_grafo_social(id2);
    int id_interno2 = no_usuario2.id_grafo;

    grafo_social.remover_aresta(id_interno1, id_interno2, false);
};

void Sistema::consultar_temas(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> temas = grafo_temas.obter_vizinhos_apontados_por(id_interno);
};

void Sistema::consultar_seguidores(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> seguidores = grafo_social.obter_vizinhos_que_apontam_para(id_interno);
};
void Sistema::consultar_seguidos(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> seguidos = grafo_social.obter_vizinhos_apontados_por(id_interno);
};
void Sistema::consultar_amigos(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> amigos = grafo_social.obter_vizinhos_bidirecional(id_interno);

};
void Sistema::consultar_relacao(int id1, int id2) const{
    const No& no_usuario1 = dicionario.get_no_usuario_grafo_social(id1);
    int id_interno1 = no_usuario1.id_grafo;
    const No& no_usuario2 = dicionario.get_no_usuario_grafo_social(id2);
    int id_interno2 = no_usuario2.id_grafo;
    bool primeiro_segue_segundo = grafo_social.checar_aresta(id_interno1, id_interno2);
    bool segundo_segue_primeiro = grafo_social.checar_aresta(id_interno2, id_interno1);
    int caso;
    if (primeiro_segue_segundo && segundo_segue_primeiro){
        caso = 3;
    } else if (primeiro_segue_segundo){
        caso = 1;
    } else if (segundo_segue_primeiro){
        caso = 2;
    } else {
        caso = 0;
    }
};
void Sistema::consultar_interesse(int id_usuario, int id_tema) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
    int id_interno_usuario = no_usuario.id_grafo;
    const No& no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
    int id_interno_tema = no_tema.id_grafo;
    bool possui_interesse = grafo_temas.checar_aresta(id_interno_usuario, id_interno_tema);
};

void Sistema::consultar_popularidade(int id_tema) const{
    const No& no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
    int id_interno_tema = no_tema.id_grafo;
    Lista<No*> interessados = grafo_temas.obter_vizinhos_que_apontam_para(id_interno_tema);
    int quant_interessados = interessados.get_tamanho();
};