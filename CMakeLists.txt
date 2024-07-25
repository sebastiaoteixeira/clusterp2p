cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(clusterp2p)

# Configura o diretório de saída para os binários e bibliotecas
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adiciona o diretório src/ como include
include_directories(${CMAKE_SOURCE_DIR}/src)

# Lista de fontes para a biblioteca
set(CLUSTERP2P_LIB_SOURCES
    src/network/node.c
    src/identity.c
    src/shared.c
    src/utils.c
    src/logger.c
)

# Cria a biblioteca compartilhada libclusterp2p.so
add_library(libclusterp2p SHARED ${CLUSTERP2P_LIB_SOURCES})

# Lista de fontes para o executável
set(CLUSTERP2P_EXEC_SOURCES
    src/network/node_listener.c
)

# Cria o executável clusterp2p
add_executable(clusterp2p ${CLUSTERP2P_EXEC_SOURCES})

# Linka a biblioteca clusterp2p com o executável clusterp2p
target_link_libraries(clusterp2p libclusterp2p)
