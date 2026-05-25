#include "Sistema.hpp"

#include <cmath>
#include <iostream>

static const char *NOME_OP_SISTEMA[NUM_OP_SISTEMA] = {
	"alterar_armazenamento",
	"adicionar_usuario",
	"adicionar_tema",
	"seguir_usuario",
	"remover_seguimento",
	"consultar_temas",
	"consultar_seguidores",
	"consultar_seguidos",
	"consultar_amigos",
	"consultar_relacao",
	"consultar_interesse",
	"consultar_popularidade",
	"consultar_recomendacao"
};

static const char *NOME_OP_GRAFO[NUM_OP_GRAFO] = {
	"criar_no",
	"criar_aresta",
	"remover_aresta",
	"vizinhos_apontados_por",
	"vizinhos_apontam_para",
	"vizinhos_bidirecional",
	"checar_aresta",
	"trocar_tipo"
};

Sistema::Sistema(TipoGrafo tipoInicial)
	: grafo_temas(tipoInicial), grafo_social(tipoInicial), dicionario(usuarios, temas, grafo_temas, grafo_social),
	  num_usuarios(0), num_temas(0), primeiro_output(true) {

	  };

void Sistema::nova_linha() const {
	if (!primeiro_output)
		std::cout << "\n";
	primeiro_output = false;
}

Sistema::~Sistema() {};

void Sistema::trocar_nos(Lista<No *> &lista, int i, int j) const {
	No *temp = lista[i];
	lista[i] = lista[j];
	lista[j] = temp;
}

int Sistema::particionar_vizinhos(Lista<No *> &lista, int inicio, int fim) const {
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

void Sistema::quickSort_internos(Lista<No *> &lista, int inicio, int fim) const {
	if (inicio < fim) {
		int indice_pivo = particionar_vizinhos(lista, inicio, fim);

		quickSort_internos(lista, inicio, indice_pivo - 1);
		quickSort_internos(lista, indice_pivo + 1, fim);
	}
}

void Sistema::ordenar_vizinhos(Lista<No *> &lista) const {
	if (lista.get_tamanho() > 1) {
		quickSort_internos(lista, 0, lista.get_tamanho() - 1);
	}
}

void Sistema::alterar_armazenamento(TipoGrafo tipo) {
	auto t0 = std::chrono::high_resolution_clock::now();
	grafo_social.trocar_tipo(tipo);
	grafo_temas.trocar_tipo(tipo);
	std::string modo = (tipo == GRAFO_LISTA) ? "L" : (tipo == GRAFO_MATRIZ) ? "M" : "E";
	nova_linha();
	std::cout << "A " << modo;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_ALTERAR_ARMAZENAMENTO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

const int Sistema::adicionar_usuario(const std::string &nome, int idade, const Lista<int> &tema_ids) {
	auto t0 = std::chrono::high_resolution_clock::now();
	Usuario usuario = Usuario(num_usuarios, nome, idade);
	usuarios.inserir(usuario);
	int id_interno_social = grafo_social.criar_no(USUARIO, num_usuarios);
	int id_interno_temas = grafo_temas.criar_no(USUARIO, num_usuarios);

	dicionario.registrar_usuario_social(id_interno_social);
	dicionario.registrar_usuario_temas(id_interno_temas);

	for (int i = 0; i < tema_ids.get_tamanho(); i++) {
		int id_tema_desejado = tema_ids[i];
		const No &no_tema = dicionario.get_no_tema_grafo_temas(id_tema_desejado);
		grafo_temas.criar_aresta(id_interno_temas, no_tema.id_grafo, false);
	}
	int id_criado = num_usuarios;
	num_usuarios++;
	nova_linha();
	std::cout << "U " << id_criado;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_ADICIONAR_USUARIO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
	return id_criado;
};

const int Sistema::adicionar_tema(const std::string &nome, TipoTema tipo) {
	auto t0 = std::chrono::high_resolution_clock::now();
	Tema tema(num_temas, nome, tipo);
	temas.inserir(tema);
	int id_interno_temas = grafo_temas.criar_no(TEMA, num_temas);
	dicionario.registrar_tema_temas(id_interno_temas);
	int id_criado = num_temas;
	num_temas++;
	nova_linha();
	std::cout << "T " << id_criado;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_ADICIONAR_TEMA].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
	return id_criado;
};

void Sistema::seguir_usuario(int id1, int id2) {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no1 = dicionario.get_no_usuario_grafo_social(id1);
	const No &no2 = dicionario.get_no_usuario_grafo_social(id2);
	grafo_social.criar_aresta(no1.id_grafo, no2.id_grafo, false);
	std::string nome_1 = usuarios[no1.id_externo].get_nome();
	std::string nome_2 = usuarios[no2.id_externo].get_nome();
	nova_linha();
	std::cout << "S " << nome_1 << " " << nome_2;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_SEGUIR_USUARIO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::remover_seguimento_usuario(int id1, int id2) {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario1 = dicionario.get_no_usuario_grafo_social(id1);
	int id_interno1 = no_usuario1.id_grafo;

	const No &no_usuario2 = dicionario.get_no_usuario_grafo_social(id2);
	int id_interno2 = no_usuario2.id_grafo;

	grafo_social.remover_aresta(id_interno1, id_interno2, false);

	std::string nome_1 = usuarios[no_usuario1.id_externo].get_nome();
	std::string nome_2 = usuarios[no_usuario2.id_externo].get_nome();
	nova_linha();
	std::cout << "R " << nome_1 << " " << nome_2;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_REMOVER_SEGUIMENTO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_temas(int id_usuario) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
	const Usuario &usuario = dicionario.get_usuario(id_usuario);
	int id_interno = no_usuario.id_grafo;
	Lista<No *> temas = grafo_temas.obter_vizinhos_apontados_por(id_interno);
	ordenar_vizinhos(temas);
	nova_linha();
	std::cout << "LT " << usuario.get_nome();
	for (int i = 0; i < temas.get_tamanho(); i++) {
		std::cout << " " << dicionario.get_tema(temas[i]->id_externo).get_nome();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_TEMAS].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_seguidores(int id_usuario) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
	const Usuario &usuario = dicionario.get_usuario(id_usuario);
	int id_interno = no_usuario.id_grafo;
	Lista<No *> seguidores = grafo_social.obter_vizinhos_que_apontam_para(id_interno);
	ordenar_vizinhos(seguidores);

	nova_linha();
	std::cout << "LC " << usuario.get_nome();
	for (int i = 0; i < seguidores.get_tamanho(); i++) {
		std::cout << " " << dicionario.get_usuario(seguidores[i]->id_externo).get_nome();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_SEGUIDORES].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_seguidos(int id_usuario) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
	const Usuario &usuario = dicionario.get_usuario(id_usuario);
	int id_interno = no_usuario.id_grafo;
	Lista<No *> seguidos = grafo_social.obter_vizinhos_apontados_por(id_interno);
	ordenar_vizinhos(seguidos);

	nova_linha();
	std::cout << "LS " << usuario.get_nome();
	for (int i = 0; i < seguidos.get_tamanho(); i++) {
		std::cout << " " << dicionario.get_usuario(seguidos[i]->id_externo).get_nome();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_SEGUIDOS].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_amigos(int id_usuario) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario = dicionario.get_no_usuario_grafo_social(id_usuario);
	const Usuario &usuario = dicionario.get_usuario(id_usuario);
	int id_interno = no_usuario.id_grafo;
	Lista<No *> amigos = grafo_social.obter_vizinhos_bidirecional(id_interno);
	ordenar_vizinhos(amigos);

	nova_linha();
	std::cout << "LA " << usuario.get_nome();
	for (int i = 0; i < amigos.get_tamanho(); i++) {
		std::cout << " " << dicionario.get_usuario(amigos[i]->id_externo).get_nome();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_AMIGOS].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_relacao(int id1, int id2) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario1 = dicionario.get_no_usuario_grafo_social(id1);
	int id_interno1 = no_usuario1.id_grafo;
	const No &no_usuario2 = dicionario.get_no_usuario_grafo_social(id2);
	int id_interno2 = no_usuario2.id_grafo;
	bool primeiro_segue_segundo = grafo_social.checar_aresta(id_interno1, id_interno2);
	bool segundo_segue_primeiro = grafo_social.checar_aresta(id_interno2, id_interno1);
	int caso;
	if (primeiro_segue_segundo && segundo_segue_primeiro) {
		caso = 3;
	} else if (primeiro_segue_segundo) {
		caso = 1;
	} else if (segundo_segue_primeiro) {
		caso = 2;
	} else {
		caso = 0;
	}
	std::string nome_1 = usuarios[no_usuario1.id_externo].get_nome();
	std::string nome_2 = usuarios[no_usuario2.id_externo].get_nome();
	nova_linha();
	std::cout << "Q " << nome_1 << " " << nome_2 << " " << caso;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_RELACAO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_interesse(int id_usuario, int id_tema) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_usuario = dicionario.get_no_usuario_grafo_temas(id_usuario);
	int id_interno_usuario = no_usuario.id_grafo;
	const No &no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
	int id_interno_tema = no_tema.id_grafo;
	bool possui_interesse = grafo_temas.checar_aresta(id_interno_usuario, id_interno_tema);

	int valor = possui_interesse ? 1 : 0;

	std::string nome_usuario = usuarios[no_usuario.id_externo].get_nome();
	std::string nome_tema = temas[no_tema.id_externo].get_nome();
	nova_linha();
	std::cout << "G " << nome_usuario << " " << nome_tema << " " << valor;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_INTERESSE].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

void Sistema::consultar_popularidade(int id_tema) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_tema = dicionario.get_no_tema_grafo_temas(id_tema);
	int id_interno_tema = no_tema.id_grafo;
	Lista<No *> interessados = grafo_temas.obter_vizinhos_que_apontam_para(id_interno_tema);
	int quant_interessados = interessados.get_tamanho();

	std::string nome_tema = temas[no_tema.id_externo].get_nome();
	nova_linha();
	std::cout << "F " << nome_tema << " " << quant_interessados;
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_POPULARIDADE].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
};

Lista<int> Sistema::bfs_distancias_social(int id_interno_social) const {
	Lista<int> dist;
	for (int i = 0; i < num_usuarios; i++) {
		dist.inserir(-1);
	}

	Lista<int> fila;
	int frente = 0;

	dist[id_interno_social] = 0;
	fila.inserir(id_interno_social);

	while (frente < fila.get_tamanho()) {
		int atual = fila[frente++];
		Lista<No *> vizinhos = grafo_social.obter_vizinhos_apontados_por(atual);
		for (int i = 0; i < vizinhos.get_tamanho(); i++) {
			int id_viz = vizinhos[i]->id_grafo;
			if (dist[id_viz] == -1) {
				dist[id_viz] = dist[atual] + 1;
				fila.inserir(id_viz);
			}
		}
	}
	return dist;
}

/**
 * Calcula |A ∩ B| / |A ∪ B| via laço duplo O(|u|×|v|). Usa a identidade
 * |A ∪ B| = |A| + |B| − |A ∩ B| para evitar uma terceira iteração.
 * Retorna 0 quando ambos os conjuntos são vazios (evita divisão por zero).
 */
double Sistema::calcular_jaccard(int id_temas_u, int id_temas_v) const {
	Lista<No *> temas_u = grafo_temas.obter_vizinhos_apontados_por(id_temas_u);
	Lista<No *> temas_v = grafo_temas.obter_vizinhos_apontados_por(id_temas_v);

	int tam_u = temas_u.get_tamanho();
	int tam_v = temas_v.get_tamanho();

	if (tam_u == 0 && tam_v == 0)
		return 0.0;

	int intersecao = 0;
	for (int i = 0; i < tam_u; i++) {
		for (int j = 0; j < tam_v; j++) {
			if (temas_u[i]->id_externo == temas_v[j]->id_externo) {
				intersecao++;
				break;
			}
		}
	}

	int uniao = tam_u + tam_v - intersecao;
	if (uniao == 0)
		return 0.0;
	return (double)intersecao / (double)uniao;
}

struct Candidato {
	int id_externo;
	double score;
};

static const double EPS = 1e-9;

static void insertion_sort_candidatos(Lista<Candidato> &cands) {
	for (int i = 1; i < cands.get_tamanho(); i++) {
		Candidato chave = cands[i];
		int j = i - 1;
		while (j >= 0) {
			bool deve_mover;
			double diff = cands[j].score - chave.score;
			if (diff < -EPS)
				deve_mover = true;
			else if (diff > EPS)
				deve_mover = false;
			else
				deve_mover = cands[j].id_externo > chave.id_externo;
			if (!deve_mover)
				break;
			cands[j + 1] = cands[j];
			j--;
		}
		cands[j + 1] = chave;
	}
}

void Sistema::consultar_recomendacao(int id_usuario, int topk, double peso_prox, double peso_afin) const {
	auto t0 = std::chrono::high_resolution_clock::now();
	const No &no_social_u = dicionario.get_no_usuario_grafo_social(id_usuario);
	const No &no_temas_u = dicionario.get_no_usuario_grafo_temas(id_usuario);
	int id_s_u = no_social_u.id_grafo;
	int id_t_u = no_temas_u.id_grafo;

	Lista<int> dist = bfs_distancias_social(id_s_u);

	Lista<Candidato> candidatos;
	for (int id_ext_v = 0; id_ext_v < num_usuarios; id_ext_v++) {
		if (id_ext_v == id_usuario)
			continue;
		const No &no_social_v = dicionario.get_no_usuario_grafo_social(id_ext_v);
		int id_s_v = no_social_v.id_grafo;
		if (grafo_social.checar_aresta(id_s_u, id_s_v))
			continue;

		double p = 0.0;
		if (dist[id_s_v] > 0)
			p = 2.0 / (double)dist[id_s_v];

		const No &no_temas_v = dicionario.get_no_usuario_grafo_temas(id_ext_v);
		int id_t_v = no_temas_v.id_grafo;
		double a = calcular_jaccard(id_t_u, id_t_v);

		Candidato c;
		c.id_externo = id_ext_v;
		c.score = peso_prox * p + peso_afin * a;
		candidatos.inserir(c);
	}

	insertion_sort_candidatos(candidatos);

	int qtd_saida = (topk < candidatos.get_tamanho()) ? topk : candidatos.get_tamanho();
	nova_linha();
	std::cout << "P " << usuarios[id_usuario].get_nome();
	for (int i = 0; i < qtd_saida; i++) {
		std::cout << " " << usuarios[candidatos[i].id_externo].get_nome();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	medicoes[OP_S_CONSULTAR_RECOMENDACAO].registrar(
		std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
}

void Sistema::imprimir_relatorio(std::ostream &out) const {
	out << "\n=== Relatorio de Medicoes ===\n";
	out << "--- Sistema ---\n";
	out << "Operacao                 Chamadas  Total(ns)        Media(ns)\n";
	for (int i = 0; i < NUM_OP_SISTEMA; i++) {
		out << NOME_OP_SISTEMA[i];
		int padding = 25 - (int)std::string(NOME_OP_SISTEMA[i]).size();
		for (int p = 0; p < padding; p++)
			out << ' ';
		out << medicoes[i].contagem << "\t\t"
		    << medicoes[i].tempo_total_ns << "\t\t"
		    << medicoes[i].tempo_medio_ns() << "\n";
	}

	const MedicoesOp *m_social = grafo_social.get_medicoes();
	out << "--- Grafo Social ---\n";
	out << "Operacao                 Chamadas  Total(ns)        Media(ns)\n";
	for (int i = 0; i < NUM_OP_GRAFO; i++) {
		out << NOME_OP_GRAFO[i];
		int padding = 25 - (int)std::string(NOME_OP_GRAFO[i]).size();
		for (int p = 0; p < padding; p++)
			out << ' ';
		out << m_social[i].contagem << "\t\t"
		    << m_social[i].tempo_total_ns << "\t\t"
		    << m_social[i].tempo_medio_ns() << "\n";
	}

	const MedicoesOp *m_temas = grafo_temas.get_medicoes();
	out << "--- Grafo de Temas ---\n";
	out << "Operacao                 Chamadas  Total(ns)        Media(ns)\n";
	for (int i = 0; i < NUM_OP_GRAFO; i++) {
		out << NOME_OP_GRAFO[i];
		int padding = 25 - (int)std::string(NOME_OP_GRAFO[i]).size();
		for (int p = 0; p < padding; p++)
			out << ' ';
		out << m_temas[i].contagem << "\t\t"
		    << m_temas[i].tempo_total_ns << "\t\t"
		    << m_temas[i].tempo_medio_ns() << "\n";
	}
}
