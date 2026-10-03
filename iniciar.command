#!/bin/bash
set -e
cd "$(dirname "$0")"
./compilar.sh grafico
./build/catalogo
