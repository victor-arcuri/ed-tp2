#ifndef DICIONARIO_HPP
#define DICIONARIO_HPP

#include "Grafo.hpp"
#include "Lista.hpp"
#include "Tema.hpp"
#include "Usuario.hpp"

/**
 * Classe Dicionario
 * ---------------------------------------------------
 * Camada de tradução entre identificadores externos de domínio e índices internos
 * dos grafos. Mantém três vetores de mapeamento: map_usuario_social,
 * map_usuario_temas e map_tema_temas.
 */
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

	/**
	 * Função registrar_usuario_social
	 * ---------------------------------------------------
	 * Registra o índice interno do novo usuário no grafo social, na posição
	 * correspondente ao seu id externo.
	 *
	 * id_grafo: índice interno atribuído pelo Grafo ao criar o nó do usuário em grafo_social.
	 */
	void registrar_usuario_social(int id_grafo);

	/**
	 * Função registrar_usuario_temas
	 * ---------------------------------------------------
	 * Registra o índice interno do novo usuário no grafo de temas, na posição
	 * correspondente ao seu id externo.
	 *
	 * id_grafo: índice interno atribuído pelo Grafo ao criar o nó do usuário em grafo_temas.
	 */
	void registrar_usuario_temas(int id_grafo);

	/**
	 * Função registrar_tema_temas
	 * ---------------------------------------------------
	 * Registra o índice interno do novo tema no grafo de temas, na posição
	 * correspondente ao seu id externo.
	 *
	 * id_grafo: índice interno atribuído pelo Grafo ao criar o nó do tema em grafo_temas.
	 */
	void registrar_tema_temas(int id_grafo);

	/**
	 * Função get_usuario
	 * ---------------------------------------------------
	 * Retorna o usuário de id externo especificado.
	 *
	 * id: identificador externo do usuário.
	 *
	 * retorno: referência constante ao Usuario correspondente.
	 */
	const Usuario &get_usuario(int id) const;

	/**
	 * Função get_tema
	 * ---------------------------------------------------
	 * Retorna o tema de id externo especificado.
	 *
	 * id: identificador externo do tema.
	 *
	 * retorno: referência constante ao Tema correspondente.
	 */
	const Tema &get_tema(int id) const;

	/**
	 * Função get_no_usuario_grafo_social
	 * ---------------------------------------------------
	 * Retorna o nó do grafo social correspondente ao usuário de id externo especificado.
	 *
	 * id: identificador externo do usuário.
	 *
	 * retorno: referência constante ao No do usuário em grafo_social.
	 * Lança exceção se o id for inválido.
	 */
	const No &get_no_usuario_grafo_social(int id) const;

	/**
	 * Função get_no_usuario_grafo_temas
	 * ---------------------------------------------------
	 * Retorna o nó do grafo de temas correspondente ao usuário de id externo especificado.
	 *
	 * id: identificador externo do usuário.
	 *
	 * retorno: referência constante ao No do usuário em grafo_temas.
	 * Lança exceção se o id for inválido.
	 */
	const No &get_no_usuario_grafo_temas(int id) const;

	/**
	 * Função get_no_tema_grafo_temas
	 * ---------------------------------------------------
	 * Retorna o nó do grafo de temas correspondente ao tema de id externo especificado.
	 *
	 * id: identificador externo do tema.
	 *
	 * retorno: referência constante ao No do tema em grafo_temas.
	 * Lança exceção se o id for inválido.
	 */
	const No &get_no_tema_grafo_temas(int id) const;
};

#endif