#!/bin/bash
# run_plots.sh — Cria venv, instala matplotlib e gera os gráficos comparativos.
#
# Uso: bash tests/run_plots.sh
# Saída: tests/plots/01_*.png ... 05_*.png

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="$SCRIPT_DIR/venv"

# --- Cria venv e instala dependências (idempotente) ---
if [ ! -f "$VENV_DIR/bin/python" ]; then
    echo "Criando ambiente virtual em '$VENV_DIR'..."
    python3 -m venv "$VENV_DIR"
    "$VENV_DIR/bin/pip" install --quiet --upgrade pip
    "$VENV_DIR/bin/pip" install --quiet matplotlib
    echo "Dependências instaladas."
else
    echo "Ambiente virtual já existe. Reutilizando."
fi

# --- Executa o script de plotagem ---
"$VENV_DIR/bin/python" "$SCRIPT_DIR/plot.py"
