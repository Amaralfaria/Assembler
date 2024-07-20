#include <bits/stdc++.h>

#include "ligador.h"
#ifndef _funcoesAuxiliaresLigador_h_
#include "funcoesAuxiliaresLigador.h"
#define _funcoesAuxiliaresLigador_h_
#endif

using namespace std;

int main(int argc, char *argv[]){    
    string caminho_arq1 = argv[1];
    string caminho_arq2 = argv[2];
    
    ligador(caminho_arq1, caminho_arq2);

    return 0;
}