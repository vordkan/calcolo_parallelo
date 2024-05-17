cmake_minimum_required(VERSION 3.28)
project(calcolo_parallelo C)

set(CMAKE_C_STANDARD 11)

add_executable(calcolo_parallelo
        calcolo_pi_parallelo.c
        decomposizione_blocchi.c
        decomposizione_colonne.c
        decomposizione_righe.c
        id_thread.c
        norma2.c
        prodotto_scalare.c
        prodotto_vettoriale.c
        scheduling.c
        somma_vettori_2.c)
