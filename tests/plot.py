#!/usr/bin/env python3
"""
plot.py — Gera gráficos comparativos a partir das métricas coletadas em
tests/results/*.metrics (geradas por tests/run_all.sh).

Gráficos produzidos em tests/plots/:
  01_modos_grafo_social.png  — tempo médio por operação do Grafo Social (L/M/E)
  02_escalonamento_n.png     — escalonamento do tempo médio com n (L/M/E)
  03_perfil_carga.png        — tempo total de workload por modo e perfil
  04_impacto_densidade.png   — impacto esparso vs denso por modo
  05_modos_sistema.png       — tempo médio por operação do Sistema (L/M/E)
"""

import os
import glob
import matplotlib
matplotlib.use('Agg')   # sem display; salva apenas em arquivo
import matplotlib.pyplot as plt
import numpy as np

# ---------------------------------------------------------------------------
# Caminhos
# ---------------------------------------------------------------------------
SCRIPT_DIR  = os.path.dirname(os.path.abspath(__file__))
RESULTS_DIR = os.path.join(SCRIPT_DIR, 'results')
PLOTS_DIR   = os.path.join(SCRIPT_DIR, 'plots')

# ---------------------------------------------------------------------------
# Parser
# ---------------------------------------------------------------------------

def parse_metrics_file(filepath):
    """
    Lê um arquivo .metrics e retorna:
    {
      'Sistema':        { op: {'chamadas': int, 'total_ns': float, 'media_ns': float} },
      'Grafo Social':   { ... },
      'Grafo de Temas': { ... }
    }
    """
    result = {}
    secao  = None

    with open(filepath) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            if '--- Sistema'        in line:  secao = 'Sistema';        result[secao] = {}; continue
            if '--- Grafo Social'   in line:  secao = 'Grafo Social';   result[secao] = {}; continue
            if '--- Grafo de Temas' in line:  secao = 'Grafo de Temas'; result[secao] = {}; continue
            if line.startswith('===') or line.startswith('Operacao'):
                continue
            if secao is None:
                continue
            parts = line.split()
            if len(parts) < 4:
                continue
            op       = parts[0]
            chamadas = int(parts[1])
            total_ns = float(parts[2])
            media_ns = float(parts[3])
            result[secao][op] = {
                'chamadas': chamadas,
                'total_ns': total_ns,
                'media_ns': media_ns,
            }
    return result


def carregar_dados():
    """
    Carrega todos os .metrics e retorna:
    data[modo][perfil][tamanho][densidade] = parsed_dict
    """
    data = {}
    padrao = os.path.join(RESULTS_DIR, '*.metrics')
    for filepath in sorted(glob.glob(padrao)):
        nome   = os.path.splitext(os.path.basename(filepath))[0]
        partes = nome.split('_')
        if len(partes) != 4:
            continue
        modo, perfil, tamanho, densidade = partes
        parsed = parse_metrics_file(filepath)
        data.setdefault(modo, {}).setdefault(perfil, {}).setdefault(tamanho, {})[densidade] = parsed
    return data


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

CORES_MODO     = {'L': '#1f77b4', 'M': '#ff7f0e', 'E': '#2ca02c'}
CORES_DENS     = {'esparso': '#5aade0', 'denso': '#d62728'}
CORES_PERFIL   = {'atualizacoes': '#e67e22', 'consultas': '#2980b9'}
MODOS          = ['L', 'M', 'E']
N_MAP          = {'pequeno': 50, 'medio': 300, 'grande': 1000}
TAMANHOS_ORD   = ['pequeno', 'medio', 'grande']

LABEL_OP_GRAFO = {
    'criar_no':              'criar_no',
    'criar_aresta':          'criar_aresta',
    'remover_aresta':        'remover_aresta',
    'vizinhos_apontados_por':'viz.saída',
    'vizinhos_apontam_para': 'viz.entrada',
    'vizinhos_bidirecional': 'viz.mútuos',
    'checar_aresta':         'checar_aresta',
    'trocar_tipo':           'trocar_tipo',
}

LABEL_OP_SISTEMA = {
    'alterar_armazenamento':  'alterar_arm.',
    'adicionar_usuario':      'add_usuario',
    'adicionar_tema':         'add_tema',
    'seguir_usuario':         'seguir',
    'remover_seguimento':     'remover_seg.',
    'consultar_temas':        'cons.temas',
    'consultar_seguidores':   'cons.seguid.',
    'consultar_seguidos':     'cons.seguidos',
    'consultar_amigos':       'cons.amigos',
    'consultar_relacao':      'cons.relação',
    'consultar_interesse':    'cons.interes.',
    'consultar_popularidade': 'cons.popular.',
    'consultar_recomendacao': 'recomendação',
}


def val(data, modo, perfil, tamanho, densidade, secao, op, campo='media_ns'):
    try:
        return data[modo][perfil][tamanho][densidade][secao][op][campo]
    except KeyError:
        return 0.0


# ---------------------------------------------------------------------------
# Gráfico 1 — Operações do Grafo Social: tempo médio por modo
# ---------------------------------------------------------------------------

def plot_01_modos_grafo_social(data):
    OPS = ['criar_no', 'criar_aresta', 'remover_aresta',
           'vizinhos_apontados_por', 'vizinhos_apontam_para',
           'vizinhos_bidirecional', 'checar_aresta']

    labels = [LABEL_OP_GRAFO[o] for o in OPS]
    x      = np.arange(len(OPS))
    w      = 0.25

    fig, ax = plt.subplots(figsize=(13, 6))
    for i, modo in enumerate(MODOS):
        valores = [val(data, modo, 'atualizacoes', 'medio', 'esparso',
                       'Grafo Social', op) for op in OPS]
        bars = ax.bar(x + i * w, valores, w,
                      label=f'Modo {modo}', color=CORES_MODO[modo], alpha=0.85)

    ax.set_yscale('log')
    ax.set_ylabel('Tempo médio (ns) — escala log')
    ax.set_title('Operações do Grafo Social — Tempo Médio por Modo\n'
                 '(rede média, esparso, perfil atualizações)')
    ax.set_xticks(x + w)
    ax.set_xticklabels(labels, rotation=30, ha='right', fontsize=9)
    ax.legend()
    ax.grid(axis='y', which='both', linestyle='--', alpha=0.4)

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# Gráfico 2 — Escalonamento com n (4 subplots)
# ---------------------------------------------------------------------------

def plot_02_escalonamento_n(data):
    SUBPLOTS = [
        ('vizinhos_apontam_para',  'Grafo Social', 'viz.entrada — O(n) vs O(n²)'),
        ('vizinhos_apontados_por', 'Grafo Social', 'viz.saída'),
        ('checar_aresta',          'Grafo Social', 'checar_aresta'),
        ('criar_no',               'Grafo Social', 'criar_no'),
    ]

    xs = [N_MAP[t] for t in TAMANHOS_ORD]

    fig, axes = plt.subplots(2, 2, figsize=(12, 9))
    fig.suptitle('Escalonamento do Tempo Médio com n\n'
                 '(esparso, perfil consultas)', fontsize=13)

    for idx, (op, secao, titulo) in enumerate(SUBPLOTS):
        ax = axes[idx // 2][idx % 2]
        for modo in MODOS:
            ys = [val(data, modo, 'consultas', t, 'esparso', secao, op)
                  for t in TAMANHOS_ORD]
            ax.plot(xs, ys, marker='o', label=f'Modo {modo}',
                    color=CORES_MODO[modo], linewidth=1.8, markersize=6)
        ax.set_title(titulo, fontsize=10)
        ax.set_xlabel('n (usuários)')
        ax.set_ylabel('Tempo médio (ns)')
        ax.set_xticks(xs)
        ax.legend(fontsize=8)
        ax.grid(linestyle='--', alpha=0.4)

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# Gráfico 3 — Perfil de carga: tempo total do workload
# ---------------------------------------------------------------------------

def plot_03_perfil_carga(data):
    OPS_WORKLOAD = [
        'seguir_usuario', 'remover_seguimento',
        'consultar_temas', 'consultar_seguidores', 'consultar_seguidos',
        'consultar_amigos', 'consultar_relacao', 'consultar_interesse',
        'consultar_popularidade',
    ]
    PERFIS = ['atualizacoes', 'consultas']
    w      = 0.3
    x      = np.arange(len(MODOS))

    fig, ax = plt.subplots(figsize=(8, 6))
    for j, perfil in enumerate(PERFIS):
        totais = []
        for modo in MODOS:
            total = sum(
                val(data, modo, perfil, 'medio', 'esparso', 'Sistema', op, 'total_ns')
                for op in OPS_WORKLOAD
            ) / 1e3   # ns → µs
            totais.append(total)
        bars = ax.bar(x + j * w, totais, w,
                      label=perfil, color=CORES_PERFIL[perfil], alpha=0.85)
        ax.bar_label(bars, fmt='%.0f µs', padding=3, fontsize=8)

    ax.set_ylabel('Tempo total de workload (µs)')
    ax.set_title('Perfil de Carga — Tempo Total por Modo e Perfil\n'
                 '(rede média, esparso)')
    ax.set_xticks(x + w / 2)
    ax.set_xticklabels([f'Modo {m}' for m in MODOS])
    ax.legend()
    ax.grid(axis='y', linestyle='--', alpha=0.4)

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# Gráfico 4 — Impacto da densidade (subplots por modo)
# ---------------------------------------------------------------------------

def plot_04_impacto_densidade(data):
    OPS    = ['vizinhos_apontam_para', 'vizinhos_apontados_por',
              'vizinhos_bidirecional', 'checar_aresta']
    labels = [LABEL_OP_GRAFO[o] for o in OPS]
    DENS   = ['esparso', 'denso']
    w      = 0.35
    x      = np.arange(len(OPS))

    fig, axes = plt.subplots(1, 3, figsize=(15, 5), sharey=False)
    fig.suptitle('Impacto da Densidade nas Operações de Vizinhança\n'
                 '(rede média, perfil consultas)', fontsize=13)

    for col, modo in enumerate(MODOS):
        ax = axes[col]
        for j, dens in enumerate(DENS):
            valores = [val(data, modo, 'consultas', 'medio', dens,
                           'Grafo Social', op) for op in OPS]
            bars = ax.bar(x + j * w, valores, w,
                          label=dens, color=CORES_DENS[dens], alpha=0.85)

        ax.set_title(f'Modo {modo}', fontsize=11)
        ax.set_xticks(x + w / 2)
        ax.set_xticklabels(labels, rotation=25, ha='right', fontsize=8)
        ax.set_ylabel('Tempo médio (ns)' if col == 0 else '')
        ax.legend(fontsize=8)
        ax.grid(axis='y', linestyle='--', alpha=0.4)

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# Gráfico 5 — Operações do Sistema: tempo médio por modo
# ---------------------------------------------------------------------------

def plot_05_modos_sistema(data):
    OPS_TODOS = [
        'adicionar_usuario', 'adicionar_tema',
        'seguir_usuario', 'remover_seguimento',
        'consultar_temas', 'consultar_seguidores', 'consultar_seguidos',
        'consultar_amigos', 'consultar_relacao', 'consultar_interesse',
        'consultar_popularidade',
    ]
    # filtra operações que tiveram chamadas em pelo menos um modo
    OPS = [o for o in OPS_TODOS
           if any(val(data, m, 'consultas', 'medio', 'esparso', 'Sistema', o) > 0
                  for m in MODOS)]

    labels = [LABEL_OP_SISTEMA.get(o, o) for o in OPS]
    x      = np.arange(len(OPS))
    w      = 0.25

    fig, ax = plt.subplots(figsize=(14, 6))
    for i, modo in enumerate(MODOS):
        valores = [val(data, modo, 'consultas', 'medio', 'esparso',
                       'Sistema', op) for op in OPS]
        ax.bar(x + i * w, valores, w,
               label=f'Modo {modo}', color=CORES_MODO[modo], alpha=0.85)

    ax.set_yscale('log')
    ax.set_ylabel('Tempo médio (ns) — escala log')
    ax.set_title('Operações do Sistema — Tempo Médio por Modo\n'
                 '(rede média, esparso, perfil consultas)')
    ax.set_xticks(x + w)
    ax.set_xticklabels(labels, rotation=30, ha='right', fontsize=9)
    ax.legend()
    ax.grid(axis='y', which='both', linestyle='--', alpha=0.4)

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    os.makedirs(PLOTS_DIR, exist_ok=True)

    print('Carregando métricas...')
    data = carregar_dados()
    if not data:
        print(f'Erro: nenhum .metrics encontrado em {RESULTS_DIR}/')
        print('Execute "bash tests/run_all.sh" primeiro.')
        return

    graficos = [
        ('01_modos_grafo_social.png',  plot_01_modos_grafo_social),
        ('02_escalonamento_n.png',     plot_02_escalonamento_n),
        ('03_perfil_carga.png',        plot_03_perfil_carga),
        ('04_impacto_densidade.png',   plot_04_impacto_densidade),
        ('05_modos_sistema.png',       plot_05_modos_sistema),
    ]

    for nome_arquivo, func in graficos:
        caminho = os.path.join(PLOTS_DIR, nome_arquivo)
        fig = func(data)
        fig.savefig(caminho, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f'  Salvo: {caminho}')

    print(f'\n5 gráficos gerados em {PLOTS_DIR}/')


if __name__ == '__main__':
    main()
