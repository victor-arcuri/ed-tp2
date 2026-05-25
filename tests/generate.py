#!/usr/bin/env python3
"""
Gerador de casos de teste para análise experimental do ConectaDCC (TP2 DCC221).

Gera 36 arquivos de entrada varrendo as dimensões:
  - Modo de armazenamento : L (lista), M (matriz), E (arestas)
  - Perfil de carga       : atualizacoes (75% S/R), consultas (80% LT/LC/LS/LA/Q/G/F)
  - Tamanho da rede       : pequeno (n=50), medio (n=300), grande (n=1000)
  - Densidade social      : esparso (grau≈2), denso (grau≈10)

Convenção de nome: {modo}_{perfil}_{tamanho}_{densidade}.txt
Casos com o mesmo (perfil, tamanho, densidade) usam a mesma semente para que
L, M e E recebam exatamente o mesmo workload — garantindo comparação justa.

Saída: tests/cases/
"""

import os
import random

# ---------------------------------------------------------------------------
# Configuração dos parâmetros
# ---------------------------------------------------------------------------

MODOS          = ['L', 'M', 'E']
PERFIL_NAMES   = ['atualizacoes', 'consultas']
TAMANHO_NAMES  = ['pequeno', 'medio', 'grande']
DENSIDADE_NAMES = ['esparso', 'denso']

# Parâmetros por tamanho: (n_users, n_themes, themes_per_user, n_workload)
TAMANHOS = {
    'pequeno': (50,   10, 2, 500),
    'medio':   (300,  30, 3, 2000),
    'grande':  (1000, 50, 4, 5000),
}

# Fator de densidade: n_follows_setup = n_users * fator
DENSIDADES = {
    'esparso': 2,
    'denso':   10,
}

# Probabilidade de gerar operação de atualização (S/R) no workload
PERFIS = {
    'atualizacoes': 0.75,
    'consultas':    0.20,
}

CASES_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'cases')


# ---------------------------------------------------------------------------
# Geração de um caso de teste
# ---------------------------------------------------------------------------

def gerar_caso(modo, perfil, tamanho, densidade, seed):
    """
    Gera as linhas de um caso de teste ConectaDCC válido.

    Retorna lista de strings (uma por linha de entrada).
    A semente é independente do modo para que L/M/E compartilhem o mesmo
    workload e possam ser comparados diretamente.
    """
    n_users, n_themes, themes_per_user, n_workload = TAMANHOS[tamanho]
    fator       = DENSIDADES[densidade]
    n_follows_s = n_users * fator
    p_update    = PERFIS[perfil]

    rng   = random.Random(seed)
    linhas = []

    # ---- Modo inicial -------------------------------------------------
    linhas.append(f"A {modo}")

    # ---- Setup: temas -------------------------------------------------
    for i in range(n_themes):
        tipo = rng.choice(['A', 'P'])
        linhas.append(f"T Tema{i} {tipo}")

    # ---- Setup: usuários ----------------------------------------------
    for i in range(n_users):
        idade = rng.randint(18, 65)
        k = min(themes_per_user, n_themes)
        ids_temas = rng.sample(range(n_themes), k)
        linhas.append(f"U User{i} {idade} {' '.join(map(str, ids_temas))}")

    # ---- Setup: arestas iniciais (grafo social base) ------------------
    follow_set  = set()
    follow_list = []      # espelho de follow_set para acesso por índice em R
    tentativas  = 0
    max_tent    = n_follows_s * 30

    while len(follow_set) < n_follows_s and tentativas < max_tent:
        u = rng.randint(0, n_users - 1)
        v = rng.randint(0, n_users - 1)
        if u != v and (u, v) not in follow_set:
            follow_set.add((u, v))
            follow_list.append((u, v))
            linhas.append(f"S {u} {v}")
        tentativas += 1

    # ---- Workload -----------------------------------------------------
    for _ in range(n_workload):
        if rng.random() < p_update:
            # Operação de atualização
            if follow_list and rng.random() < 0.4:
                # R: remove aresta existente (swap-com-último para O(1))
                idx  = rng.randrange(len(follow_list))
                pair = follow_list[idx]
                follow_list[idx] = follow_list[-1]
                follow_list.pop()
                follow_set.discard(pair)
                linhas.append(f"R {pair[0]} {pair[1]}")
            else:
                # S: insere nova aresta
                inserido = False
                for _ in range(20):
                    u = rng.randint(0, n_users - 1)
                    v = rng.randint(0, n_users - 1)
                    if u != v and (u, v) not in follow_set:
                        follow_set.add((u, v))
                        follow_list.append((u, v))
                        linhas.append(f"S {u} {v}")
                        inserido = True
                        break
                if not inserido:
                    # Grafo saturado: cai para consulta leve
                    linhas.append(f"LT {rng.randint(0, n_users - 1)}")
        else:
            # Operação de consulta
            op = rng.choice(['LT', 'LC', 'LS', 'LA', 'Q', 'G', 'F'])
            if op in ('LT', 'LC', 'LS', 'LA'):
                linhas.append(f"{op} {rng.randint(0, n_users - 1)}")
            elif op == 'Q':
                u = rng.randint(0, n_users - 1)
                v = rng.randint(0, n_users - 1)
                while v == u:
                    v = rng.randint(0, n_users - 1)
                linhas.append(f"Q {u} {v}")
            elif op == 'G':
                linhas.append(
                    f"G {rng.randint(0, n_users - 1)} {rng.randint(0, n_themes - 1)}"
                )
            elif op == 'F':
                linhas.append(f"F {rng.randint(0, n_themes - 1)}")

    return linhas


# ---------------------------------------------------------------------------
# Ponto de entrada
# ---------------------------------------------------------------------------

def main():
    os.makedirs(CASES_DIR, exist_ok=True)
    total = 0

    for perfil_idx, perfil in enumerate(PERFIL_NAMES):
        for tamanho_idx, tamanho in enumerate(TAMANHO_NAMES):
            for densidade_idx, densidade in enumerate(DENSIDADE_NAMES):
                # Semente idêntica para L/M/E do mesmo cenário → workload igual
                seed = perfil_idx * 100 + tamanho_idx * 10 + densidade_idx

                # Pré-gera as linhas uma vez e ajusta apenas a primeira para cada modo
                linhas_base = gerar_caso('L', perfil, tamanho, densidade, seed)

                for modo in MODOS:
                    nome    = f"{modo}_{perfil}_{tamanho}_{densidade}"
                    caminho = os.path.join(CASES_DIR, f"{nome}.txt")

                    # Primeira linha muda de modo; resto é idêntico
                    linhas = [f"A {modo}"] + linhas_base[1:]

                    with open(caminho, 'w') as fp:
                        fp.write('\n'.join(linhas) + '\n')

                    total += 1
                    print(f"  {nome}.txt  ({len(linhas)} linhas)")

    print(f"\n{total} casos gerados em '{CASES_DIR}/'")


if __name__ == '__main__':
    main()
