#include <iostream>
#include <sstream>
#include <string>

#include "Sistema.hpp"

using namespace std;

int main() {
	char comando_inicial;
	char tipo_armazenamento;
	if (!(cin >> comando_inicial >> tipo_armazenamento)) {
		return 0;
	}

	TipoGrafo tipo_inicial;
	if (tipo_armazenamento == 'M') {
		tipo_inicial = GRAFO_MATRIZ;
	} else {
		tipo_inicial = GRAFO_LISTA;
	}
	Sistema sistema(tipo_inicial);
	std::cout << "A " << tipo_armazenamento << "\n";
	string comando;
	while (cin >> comando) {
		if (comando == "A") {
			TipoGrafo tipo;
			char tipo_ch;

			cin >> tipo_ch;

			if (tipo_ch == 'L') {
				tipo = GRAFO_LISTA;
			} else if (tipo_ch == 'M') {
				tipo = GRAFO_MATRIZ;
			}
			sistema.alterar_armazenamento(tipo);
		} else if (comando == "T") {
			std::string nome_tema;
			char tipo_tema_ch;
			cin >> nome_tema >> tipo_tema_ch;

			TipoTema tipo_tema;

			if (tipo_tema_ch == 'A') {
				tipo_tema = ACADEMICO;
			} else if (tipo_tema_ch == 'P') {
				tipo_tema = PESSOAL;
			}
			sistema.adicionar_tema(nome_tema, tipo_tema);
		} else if (comando == "U") {
			string nome;
			int idade;
			cin >> nome >> idade;

			string resto_da_linha;
			getline(cin >> ws, resto_da_linha);

			stringstream ss(resto_da_linha);
			Lista<int> id_temas;
			int id_temp;

			while (ss >> id_temp) {
				id_temas.inserir(id_temp);
			}

			sistema.adicionar_usuario(nome, idade, id_temas);
		} else if (comando == "S") {
			int id_1, id_2;
			cin >> id_1 >> id_2;
			sistema.seguir_usuario(id_1, id_2);
		} else if (comando == "R") {
			int id_1, id_2;
			cin >> id_1 >> id_2;
			sistema.remover_seguimento_usuario(id_1, id_2);
		} else if (comando == "LT") {
			int id;
			cin >> id;
			sistema.consultar_temas(id);
		} else if (comando == "LC") {
			int id;
			cin >> id;
			sistema.consultar_seguidores(id);
		} else if (comando == "LS") {
			int id;
			cin >> id;
			sistema.consultar_seguidos(id);
		} else if (comando == "LA") {
			int id;
			cin >> id;
			sistema.consultar_amigos(id);
		} else if (comando == "Q") {
			int id_1, id_2;
			cin >> id_1 >> id_2;
			sistema.consultar_relacao(id_1, id_2);
		} else if (comando == "G") {
			int id_usuario, id_tema;
			cin >> id_usuario >> id_tema;
			sistema.consultar_interesse(id_usuario, id_tema);
		} else if (comando == "F") {
			int id_tema;
			cin >> id_tema;
			sistema.consultar_popularidade(id_tema);
		}
	}
	return 0;
}