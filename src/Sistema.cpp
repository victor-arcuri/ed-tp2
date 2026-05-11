#include "Sistema.hpp"
#include <iostream>

Sistema::Sistema(TipoGrafo tipoInicial):    grafo_temas(tipoInicial),                            
                                            grafo_social(tipoInicial),                           
                                            dicionario(usuarios, temas, grafo_temas, grafo_social), 
                                            num_usuarios(0),                                     
                                            num_temas(0)
{

};

Sistema::~Sistema(){};

void Sistema::trocar_nos(Lista<No*>& lista, int i, int j) const {
    No* temp = lista[i];
    lista[i] = lista[j];
    lista[j] = temp;
}

int Sistema::particionar_vizinhos(Lista<No*>& lista, int inicio, int fim) const {
    int pivo = lista[fim]->id_externo;
    int i = (inicio - 1);

    for (int j = inicio; j < fim; j++) {
        if (lista[j]->id_externo <= pivo) {
            i++;
            trocar_nos(lista, i, j);
        }
    }
    trocar_nos(lista, i + 1, fim);
    return (i + 1);
}

void Sistema::quickSort_internos(Lista<No*>& lista, int inicio, int fim) const {
    if (inicio < fim) {
        int indice_pivo = particionar_vizinhos(lista, inicio, fim);
        
        quickSort_internos(lista, inicio, indice_pivo - 1);
        quickSort_internos(lista, indice_pivo + 1, fim);
    }
}

void Sistema::ordenar_vizinhos(Lista<No*>& lista) const {
    if (lista.get_tamanho() > 1) {
        quickSort_internos(lista, 0, lista.get_tamanho() - 1);
    }
}

void Sistema::alterar_armazenamento(TipoGrafo tipo){
    grafo_social.trocar_tipo(tipo);
    grafo_temas.trocar_tipo(tipo);
    std::cout << "A " << tipo << "\n";
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
    std::cout << "U " << id_criado << "\n";
    return id_criado;
};

const int Sistema::adicionar_tema(const std::string& nome, TipoTema tipo){
    Tema tema(num_temas, nome, tipo);
    temas.inserir(tema);
    int id_interno_temas = grafo_temas.criar_no(TEMA, num_temas);
    dicionario.registrar_tema_temas(id_interno_temas);
    int id_criado = num_temas;
    num_temas++;
    std::cout << "T " << id_criado << "\n";
    return id_criado;
};

void Sistema::seguir_usuario(int id1, int id2){
    const No& no1 = dicionario.get_no_usuario_grafo_social(id1);
    const No& no2 = dicionario.get_no_usuario_grafo_social(id2);
    grafo_social.criar_aresta(no1.id_grafo, no2.id_grafo, false);
    std::string nome_1 = usuarios[no1.id_externo].get_nome();
    std::string nome_2 = usuarios[no2.id_externo].get_nome();
    std::cout << "S " << nome_1 << " "<< nome_2 << "\n";
};

void Sistema::remover_seguimento_usuario(int id1, int id2){
    const No& no_usuario1 = dicionario.get_no_usuario_grafo_social(id1);
    int id_interno1 = no_usuario1.id_grafo;

    const No& no_usuario2 = dicionario.get_no_usuario_grafo_social(id2);
    int id_interno2 = no_usuario2.id_grafo;

    grafo_social.remover_aresta(id_interno1, id_interno2, false);

    std::string nome_1 = usuarios[no_usuario1.id_externo].get_nome();
    std::string nome_2 = usuarios[no_usuario2.id_externo].get_nome();
    std::cout << "R " << nome_1 << " "<< nome_2 << "\n";
};

void Sistema::consultar_temas(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
    const Usuario& usuario = dicionario.get_usuario(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> temas = grafo_temas.obter_vizinhos_apontados_por(id_interno);
    ordenar_vizinhos(temas);
    std::cout << "LT " << usuario.get_nome();
    for(int i = 0; i < temas.get_tamanho(); i++) {
        std::cout << " " << dicionario.get_tema(temas[i]->id_externo).get_nome();
    }
    std::cout << std::endl;
};

void Sistema::consultar_seguidores(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    const Usuario& usuario = dicionario.get_usuario(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> seguidores = grafo_social.obter_vizinhos_que_apontam_para(id_interno);
    ordenar_vizinhos(seguidores);
 
    std::cout << "LC " << usuario.get_nome();
    for(int i = 0; i < seguidores.get_tamanho(); i++) {
        std::cout << " " << dicionario.get_usuario(seguidores[i]->id_externo).get_nome();
    }
    std::cout << std::endl;
};
void Sistema::consultar_seguidos(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    const Usuario& usuario = dicionario.get_usuario(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> seguidos = grafo_social.obter_vizinhos_apontados_por(id_interno);
    ordenar_vizinhos(seguidos);
    
    std::cout << "LS " << usuario.get_nome();
    for(int i = 0; i < seguidos.get_tamanho(); i++) {
        std::cout << " " << dicionario.get_usuario(seguidos[i]->id_externo).get_nome();
    }
    std::cout << std::endl;
};
void Sistema::consultar_amigos(int id_usuario) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
    const Usuario& usuario = dicionario.get_usuario(id_usuario);
    int id_interno = no_usuario.id_grafo;
    Lista<No*> amigos = grafo_social.obter_vizinhos_bidirecional(id_interno);
    ordenar_vizinhos(amigos);
    
    std::cout << "LA " << usuario.get_nome();
    for(int i = 0; i < amigos.get_tamanho(); i++) {
        std::cout << " " << dicionario.get_usuario(amigos[i]->id_externo).get_nome();
    }
    std::cout << std::endl;

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
    std::string nome_1 = usuarios[no_usuario1.id_externo].get_nome();
    std::string nome_2 = usuarios[no_usuario2.id_externo].get_nome();
    std::cout << "Q " << nome_1 << " "<< nome_2 << " " << caso << "\n";
};
void Sistema::consultar_interesse(int id_usuario, int id_tema) const{
    const No& no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
    int id_interno_usuario = no_usuario.id_grafo;
    const No& no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
    int id_interno_tema = no_tema.id_grafo;
    bool possui_interesse = grafo_temas.checar_aresta(id_interno_usuario, id_interno_tema);

    int valor = possui_interesse ? 1 : 0;

    std::string nome_usuario = usuarios[no_usuario.id_externo].get_nome();
    std::string nome_tema = temas[no_tema.id_externo].get_nome();
    std::cout << "G " << nome_usuario << " "<< nome_tema << " " << valor << "\n";
};

void Sistema::consultar_popularidade(int id_tema) const{
    const No& no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
    int id_interno_tema = no_tema.id_grafo;
    Lista<No*> interessados = grafo_temas.obter_vizinhos_que_apontam_para(id_interno_tema);
    int quant_interessados = interessados.get_tamanho();
    
    std::string nome_tema = temas[no_tema.id_externo].get_nome();
    std::cout << "F " << nome_tema << " " << quant_interessados << "\n";
};