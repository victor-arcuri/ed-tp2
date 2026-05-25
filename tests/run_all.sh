#!/bin/bash
# run_all.sh — Executa todos os casos de teste e coleta métricas de tempo.
#
# Pré-requisitos:
#   1. make all          (compila bin/tp2.out)
#   2. python3 generate.py  (gera os casos em tests/cases/)
#
# Saída:
#   tests/results/{nome}.metrics  — relatório bruto de cada caso
#   tests/results/summary.txt     — todos os relatórios concatenados com cabeçalho

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BINARY="$PROJECT_DIR/bin/tp2.out"
CASES_DIR="$SCRIPT_DIR/cases"
RESULTS_DIR="$SCRIPT_DIR/results"
SUMMARY="$RESULTS_DIR/summary.txt"

# --- Verificações de pré-condição ---
if [ ! -f "$BINARY" ]; then
    echo "Erro: executável não encontrado em '$BINARY'."
    echo "       Execute 'make all' na raiz do projeto."
    exit 1
fi

if [ ! -d "$CASES_DIR" ] || [ -z "$(ls -A "$CASES_DIR" 2>/dev/null)" ]; then
    echo "Erro: nenhum caso de teste em '$CASES_DIR'."
    echo "       Execute 'python3 tests/generate.py' na raiz do projeto."
    exit 1
fi

mkdir -p "$RESULTS_DIR"
> "$SUMMARY"

# --- Execução dos casos ---
count=0
failed=0

for caso in "$CASES_DIR"/*.txt; do
    nome=$(basename "$caso" .txt)
    metrics_file="$RESULTS_DIR/${nome}.metrics"

    if "$BINARY" < "$caso" > /dev/null 2> "$metrics_file"; then
        echo "=== $nome ===" >> "$SUMMARY"
        cat "$metrics_file"  >> "$SUMMARY"
        printf "\n"          >> "$SUMMARY"
        count=$((count + 1))
        printf "  [OK]  %s\n" "$nome"
    else
        echo "=== $nome === FALHOU ===" >> "$SUMMARY"
        failed=$((failed + 1))
        printf "  [FALHOU]  %s\n" "$nome"
    fi
done

# --- Resumo final ---
echo ""
echo "----------------------------------------"
echo "Concluído: $count casos OK, $failed falhas."
echo "Métricas individuais : $RESULTS_DIR/*.metrics"
echo "Resumo consolidado   : $SUMMARY"
