#include <bits/stdc++.h>
#include <fstream>

using namespace std;

map<string, int> definicoesGlobais;
vector<int> codigoLigado;
vector<int> bitsMapa;

/**
    Responsável por ligar dois módulos de um programa e gerar um arquivo de saída contendo o código objeto ligado.
    @param caminho1 String com o caminho do primeiro arquivo.
    @param caminho2 String com o caminho do segundo arquivo.
*/
void ligador(string caminho1, string caminho2) {
    auto arq1 = carregarArquivo(caminho1);
    auto arq2 = carregarArquivo(caminho2);

    Estruturas tabela1 = processarArquivo(arq1);
    Estruturas tabela2 = processarArquivo(arq2);

    // Concatenando código objeto e mapa de bits
    codigoLigado = unirVetores(tabela1.codigoObjeto, tabela2.codigoObjeto);
    bitsMapa = unirVetores(tabela1.mapaBits, tabela2.mapaBits);

    int correcao = tabela1.codigoObjeto.size();

    // Aplicando fator de correção no segundo módulo
    tabela2 = ajustarEnderecos(tabela2, correcao);

    // Criando a tabela de definições global
    definicoesGlobais = combinarDefinicoes(tabela1.definicoes, tabela2.definicoes);

    // Resolvendo referências do primeiro módulo
    for (unsigned int i = 0; i < tabela1.uso.size(); i++) {
        auto simbolo = tabela1.uso[i].first;
        auto posicao = tabela1.uso[i].second;
        codigoLigado[posicao] = definicoesGlobais[simbolo];
    }

    // Ajustando referências do segundo módulo
    for (unsigned int i = 0; i < tabela2.codigoObjeto.size(); ) {
        int endereco = tabela2.codigoObjeto[i];
        int tamanhoInstrucao = obterOpcode(endereco);
        if (tamanhoInstrucao == -1) break;

        i++;
        for (int j = 0; j < tamanhoInstrucao; j++) {
            int posicao = i + correcao;
            bool referenciaCruzada = false;

            if ((int)bitsMapa.size() > posicao && bitsMapa[posicao] == '0') break;

            for (const auto& par : tabela2.uso) {
                if (posicao == par.second) {
                    codigoLigado[par.second] = definicoesGlobais[par.first];
                    referenciaCruzada = true;
                }
            }

            if (!referenciaCruzada) {
                codigoLigado[posicao] += correcao;
            }

            i++;
        }
    }

    // Escrevendo o código objeto ligado em um arquivo de saída
    ofstream saida;
    saida.open("prog1.e");
    if (!saida) {
        cout << "Erro: não foi possível criar o arquivo de saída\n";
        saida.close();
        return;
    }
    for (unsigned int i = 0; i < codigoLigado.size(); i++) {
        saida << codigoLigado[i] << " ";
    }
    saida.close();
}

