all:
	g++ -o comp compilador.cpp
	g++ -o lig Ligador/ligador.cpp Ligador/ligador.h Ligador/funcoesAuxiliaresLigador.h

ligador:
	g++ -o lig Ligador/ligador.cpp Ligador/ligador.h Ligador/funcoesAuxiliaresLigador.h

compilador:
	g++ -o comp compilador.cpp