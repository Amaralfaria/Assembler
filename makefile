all:
	g++ -o montador compilador.cpp
	g++ -o ligador Ligador/ligador.cpp Ligador/ligador.h Ligador/funcoesAuxiliaresLigador.h

ligador:
	g++ -o ligador Ligador/ligador.cpp Ligador/ligador.h Ligador/funcoesAuxiliaresLigador.h

compilador:
	g++ -o montador compilador.cpp