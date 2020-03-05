#!/bin/bash -f

INPUT=$1
OUTPUT=$2

EMSDKPATH="/home/pratikbhandari/Documents/emscripten/emsdk-portable/emscripten/1.38.31/system/include/"
OPT=-O2

time emcc \
   -I ${EMSDKPATH} \
   ${OPT} \
   ${INPUT} \
   -o ${OUTPUT} \
   -s WASM=1 \
   -s ALLOW_MEMORY_GROWTH=1 \
   -s AGGRESSIVE_VARIABLE_ELIMINATION=1 \
   -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
   -s USE_WEBGL2=1 \
   -s USE_GLFW=3 \
   -s FULL_ES3=1 \
   -s ASSERTIONS=1 \
   --js-library src/library_websocket.js \
   -s SOCKET_DEBUG=1 \
   -fmacro-backtrace-limit=0 \