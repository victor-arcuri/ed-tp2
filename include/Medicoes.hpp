#ifndef MEDICOES_HPP
#define MEDICOES_HPP

#include <chrono>

/**
 * struct MedicoesOp
 * ---------------------------------------------------
 * Acumulador de medições de tempo para uma única operação de um TAD.
 * Armazena o tempo total em nanossegundos e o número de chamadas realizadas,
 * permitindo calcular o tempo médio por invocação. Deve ser embutido
 * diretamente nos TADs instrumentados para que as medidas sejam parte
 * integrante do TAD, conforme recomendado pela análise experimental.
 */
struct MedicoesOp {
	long long tempo_total_ns;
	int contagem;

	MedicoesOp() : tempo_total_ns(0), contagem(0) {}

	/**
	 * Função registrar
	 * ---------------------------------------------------
	 * Acumula a duração de uma execução ao total da operação e incrementa
	 * o contador de chamadas.
	 *
	 * duracao_ns: duração da execução em nanossegundos.
	 */
	void registrar(long long duracao_ns) {
		tempo_total_ns += duracao_ns;
		contagem++;
	}

	/**
	 * Função tempo_medio_ns
	 * ---------------------------------------------------
	 * Retorna o tempo médio por chamada em nanossegundos.
	 * Retorna 0.0 se a operação nunca foi chamada.
	 *
	 * retorno: média de tempo em nanossegundos, ou 0.0 se contagem == 0.
	 */
	double tempo_medio_ns() const {
		if (contagem == 0)
			return 0.0;
		return (double)tempo_total_ns / contagem;
	}
};

/**
 * enum OperacaoGrafo
 * ---------------------------------------------------
 * Identifica cada operação pública do TAD Grafo para fins de
 * indexação do array de medições interno ao Grafo. O valor
 * NUM_OP_GRAFO indica o tamanho do array e não corresponde
 * a nenhuma operação real.
 */
enum OperacaoGrafo {
	OP_G_CRIAR_NO,
	OP_G_CRIAR_ARESTA,
	OP_G_REMOVER_ARESTA,
	OP_G_VIZINHOS_APONTADOS,
	OP_G_VIZINHOS_APONTAM_PARA,
	OP_G_VIZINHOS_BIDIRECIONAL,
	OP_G_CHECAR_ARESTA,
	OP_G_TROCAR_TIPO,
	NUM_OP_GRAFO
};

/**
 * enum OperacaoSistema
 * ---------------------------------------------------
 * Identifica cada comando do protocolo de entrada mapeado para um
 * método público do TAD Sistema, permitindo medir o tempo médio
 * por tipo de operação ao longo de toda a execução. O valor
 * NUM_OP_SISTEMA indica o tamanho do array e não corresponde
 * a nenhuma operação real.
 */
enum OperacaoSistema {
	OP_S_ALTERAR_ARMAZENAMENTO,
	OP_S_ADICIONAR_USUARIO,
	OP_S_ADICIONAR_TEMA,
	OP_S_SEGUIR_USUARIO,
	OP_S_REMOVER_SEGUIMENTO,
	OP_S_CONSULTAR_TEMAS,
	OP_S_CONSULTAR_SEGUIDORES,
	OP_S_CONSULTAR_SEGUIDOS,
	OP_S_CONSULTAR_AMIGOS,
	OP_S_CONSULTAR_RELACAO,
	OP_S_CONSULTAR_INTERESSE,
	OP_S_CONSULTAR_POPULARIDADE,
	OP_S_CONSULTAR_RECOMENDACAO,
	NUM_OP_SISTEMA
};

#endif
