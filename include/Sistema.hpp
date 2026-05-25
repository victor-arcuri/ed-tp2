#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include "Dicionario.hpp"
#include "Grafo.hpp"
#include "Lista.hpp"
#include "Medicoes.hpp"
#include "Tema.hpp"
#include "Usuario.hpp"
#include <ostream>

/**
 * Classe Sistema
 * ---------------------------------------------------
 * Orquestrador principal da rede social ConectaDCC. Mantém os dois grafos,
 * as coleções de domínio e o Dicionario, implementando todos os comandos
 * do protocolo de entrada/saída.
 */
class Sistema {
	Lista<Usuario> usuarios;
	Lista<Tema> temas;
	Grafo grafo_temas;
	Grafo grafo_social;
	Dicionario dicionario;

	int num_usuarios;
	int num_temas;
	mutable bool primeiro_output;
	mutable MedicoesOp medicoes[NUM_OP_SISTEMA];

	/**
	 * Função nova_linha
	 * ---------------------------------------------------
	 * Emite uma quebra de linha antes de cada saída, exceto na primeira,
	 * garantindo que os resultados sejam separados por '\n' sem linha final extra.
	 */
	void nova_linha() const;

	/**
	 * Função ordenar_vizinhos
	 * ---------------------------------------------------
	 * Ordena em-place uma lista de ponteiros para nós em ordem crescente de id_externo,
	 * garantindo que as saídas de consulta sejam apresentadas em ordem ascendente de id.
	 *
	 * lista: Lista<No*> a ser ordenada.
	 */
	void ordenar_vizinhos(Lista<No *> &lista) const;
	void quickSort_internos(Lista<No *> &lista, int inicio, int fim) const;
	int particionar_vizinhos(Lista<No *> &lista, int inicio, int fim) const;
	void trocar_nos(Lista<No *> &lista, int i, int j) const;

	/**
	 * Função bfs_distancias_social
	 * ---------------------------------------------------
	 * Executa BFS no grafo_social a partir do nó id_interno_social e retorna as
	 * distâncias mínimas dirigidas até todos os demais usuários.
	 *
	 * id_interno_social: índice interno do nó de origem no grafo_social.
	 *
	 * retorno: Lista<int> indexada por id interno, onde dist[v] é a distância de
	 *          u até v, ou -1 se v não for alcançável a partir de u.
	 */
	Lista<int> bfs_distancias_social(int id_interno_social) const;

	/**
	 * Função calcular_jaccard
	 * ---------------------------------------------------
	 * Calcula a similaridade de Jaccard entre os conjuntos de temas de dois usuários,
	 * usada como métrica de afinidade na recomendação de usuários.
	 *
	 * id_temas_u: índice interno do usuário u no grafo_temas.
	 * id_temas_v: índice interno do usuário v no grafo_temas.
	 *
	 * retorno: float entre 0 e 1 representando |temas_u ∩ temas_v| / |temas_u ∪ temas_v|.
	 */
	double calcular_jaccard(int id_temas_u, int id_temas_v) const;

  public:
	Sistema(TipoGrafo tipoInicial);
	~Sistema();

	/**
	 * Função alterar_armazenamento
	 * ---------------------------------------------------
	 * Altera a representação interna de ambos os grafos, migrando todas as arestas.
	 * Corresponde ao comando A <L|M|E> do protocolo.
	 *
	 * tipo: nova representação interna desejada (GRAFO_LISTA, GRAFO_MATRIZ ou GRAFO_ARESTA).
	 */
	void alterar_armazenamento(TipoGrafo tipo);

	/**
	 * Função adicionar_usuario
	 * ---------------------------------------------------
	 * Cria um novo usuário, registra-o em ambos os grafos e conecta-o aos temas de interesse.
	 * Corresponde ao comando U <nome> <idade> <ids...> do protocolo.
	 *
	 * nome: nome do usuário.
	 * idade: idade do usuário.
	 * tema_ids: lista de ids externos dos temas de interesse do usuário.
	 *
	 * retorno: id externo atribuído ao novo usuário.
	 */
	const int adicionar_usuario(const std::string &nome, int idade, const Lista<int> &tema_ids);

	/**
	 * Função adicionar_tema
	 * ---------------------------------------------------
	 * Cria um novo tema e registra-o no grafo_temas.
	 * Corresponde ao comando T <nome> <tipo> do protocolo.
	 *
	 * nome: nome do tema.
	 * tipo: TipoTema do tema (ACADEMICO ou PESSOAL).
	 *
	 * retorno: id externo atribuído ao novo tema.
	 */
	const int adicionar_tema(const std::string &nome, TipoTema tipo);

	/**
	 * Função seguir_usuario
	 * ---------------------------------------------------
	 * Cria a aresta dirigida id1→id2 no grafo_social.
	 * Corresponde ao comando S <id1> <id2> do protocolo.
	 *
	 * id1: id externo do usuário que passa a seguir.
	 * id2: id externo do usuário a ser seguido.
	 */
	void seguir_usuario(int id1, int id2);

	/**
	 * Função remover_seguimento_usuario
	 * ---------------------------------------------------
	 * Remove a aresta dirigida id1→id2 do grafo_social.
	 * Corresponde ao comando R <id1> <id2> do protocolo.
	 *
	 * id1: id externo do usuário que para de seguir.
	 * id2: id externo do usuário que deixa de ser seguido.
	 */
	void remover_seguimento_usuario(int id1, int id2);

	/**
	 * Função consultar_temas
	 * ---------------------------------------------------
	 * Lista os temas de interesse do usuário em ordem crescente de id.
	 * Corresponde ao comando LT <id> do protocolo.
	 *
	 * id_usuario: id externo do usuário a ser consultado.
	 */
	void consultar_temas(int id_usuario) const;

	/**
	 * Função consultar_seguidores
	 * ---------------------------------------------------
	 * Lista os usuários que seguem o usuário informado, em ordem crescente de id.
	 * Corresponde ao comando LC <id> do protocolo.
	 *
	 * id_usuario: id externo do usuário a ser consultado.
	 */
	void consultar_seguidores(int id_usuario) const;

	/**
	 * Função consultar_seguidos
	 * ---------------------------------------------------
	 * Lista os usuários que o usuário informado segue, em ordem crescente de id.
	 * Corresponde ao comando LS <id> do protocolo.
	 *
	 * id_usuario: id externo do usuário a ser consultado.
	 */
	void consultar_seguidos(int id_usuario) const;

	/**
	 * Função consultar_amigos
	 * ---------------------------------------------------
	 * Lista os usuários com relação de seguimento mútuo com o usuário informado,
	 * em ordem crescente de id. Corresponde ao comando LA <id> do protocolo.
	 *
	 * id_usuario: id externo do usuário a ser consultado.
	 */
	void consultar_amigos(int id_usuario) const;

	/**
	 * Função consultar_relacao
	 * ---------------------------------------------------
	 * Verifica e imprime a relação de seguimento entre dois usuários.
	 * Resultado: 0 (nenhuma), 1 (id1→id2), 2 (id2→id1), 3 (mútua).
	 * Corresponde ao comando Q <id1> <id2> do protocolo.
	 *
	 * id1: id externo do primeiro usuário.
	 * id2: id externo do segundo usuário.
	 */
	void consultar_relacao(int id1, int id2) const;

	/**
	 * Função consultar_interesse
	 * ---------------------------------------------------
	 * Verifica se o usuário tem interesse no tema especificado.
	 * Corresponde ao comando G <id_u> <id_t> do protocolo.
	 *
	 * id_usuario: id externo do usuário.
	 * id_tema: id externo do tema.
	 */
	void consultar_interesse(int id_usuario, int id_tema) const;

	/**
	 * Função consultar_popularidade
	 * ---------------------------------------------------
	 * Imprime o número de usuários interessados no tema especificado.
	 * Corresponde ao comando F <id_t> do protocolo.
	 *
	 * id_tema: id externo do tema a ser consultado.
	 */
	void consultar_popularidade(int id_tema) const;

	/**
	 * Função consultar_recomendacao
	 * ---------------------------------------------------
	 * Recomenda os topk usuários mais relevantes para o usuário informado, excluindo
	 * o próprio e quem ele já segue. A pontuação de cada candidato v é calculada como
	 * peso_prox × prox(u,v) + peso_afin × afin(u,v), onde prox usa distância BFS no
	 * grafo_social e afin é a similaridade de Jaccard dos conjuntos de temas.
	 * Empates são desfeitos por id_externo crescente.
	 * Corresponde ao comando P <id_u> <topk> <peso_prox> <peso_afin> do protocolo.
	 *
	 * id_usuario: id externo do usuário que solicita recomendação.
	 * topk: número máximo de recomendações a retornar.
	 * peso_prox: peso da métrica de proximidade no grafo social.
	 * peso_afin: peso da métrica de afinidade de temas.
	 */
	void consultar_recomendacao(int id_usuario, int topk, double peso_prox, double peso_afin) const;

	/**
	 * Função imprimir_relatorio
	 * ---------------------------------------------------
	 * Imprime para o fluxo de saída informado as estatísticas de tempo
	 * coletadas durante a execução: por operação do Sistema e por operação
	 * de cada Grafo (social e de temas). Deve ser chamada após o encerramento
	 * do loop de comandos, tipicamente com std::cerr para não interferir
	 * na saída padrão esperada pelo VPL.
	 *
	 * out: fluxo de saída onde o relatório será escrito (ex: std::cerr).
	 */
	void imprimir_relatorio(std::ostream &out) const;
};

#endif