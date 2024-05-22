#include <bits/stdc++.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

map<int, int> instrucoes = {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 2}, {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 0}};

struct Estruturas {
    map<string, int> definicoes;
    vector<pair<string, int>> uso;
    vector<int> mapaBits;
    vector<int> codigoObjeto;
};

/**
    Ajusta os endereços nas tabelas de definições e de uso aplicando um fator de correção.
    @param tabela Uma estrutura contendo as tabelas.
    @param correcao O fator de correção a ser aplicado.
    @return Uma estrutura contendo as tabelas corrigidas.
*/
Estruturas ajustarEnderecos(Estruturas tabela, int correcao) {
    Estruturas resultado = tabela;

    // Corrige endereços na tabela de definições
    for (auto &par : resultado.definicoes) {
        par.second += correcao;
    }

    // Corrige endereços na tabela de uso
    for (auto &uso : resultado.uso) {
        uso.second += correcao;
    }

    return resultado;
}

/**
    Concatena dois vetores de inteiros.
    @param vetorA Primeiro vetor de inteiros.
    @param vetorB Segundo vetor de inteiros.
    @return Um vetor contendo a concatenação dos dois vetores.
*/
vector<int> unirVetores(const vector<int>& vetorA, const vector<int>& vetorB) {
    vector<int> vetorResultante = vetorA;
    vetorResultante.insert(vetorResultante.end(), vetorB.begin(), vetorB.end());
    return vetorResultante;
}

/**
    Combina duas tabelas de definições em uma única tabela global.
    @param tabelaA Primeira tabela de definições.
    @param tabelaB Segunda tabela de definições.
    @return Uma tabela global contendo as definições de ambas as tabelas.
*/
map<string, int> combinarDefinicoes(const map<string, int>& tabelaA, const map<string, int>& tabelaB) {
    map<string, int> definicoesGlobais;

    // Adiciona as definições da primeira tabela
    for (const auto &par : tabelaA) {
        definicoesGlobais[par.first] = par.second;
    }

    // Adiciona as definições da segunda tabela
    for (const auto &par : tabelaB) {
        definicoesGlobais[par.first] = par.second;
    }

    return definicoesGlobais;
}

/**
    Exibe uma matriz de strings na tela.
    @param dados Uma matriz de strings a ser exibida.
*/
void mostrarArquivo(const vector<vector<string>>& dados) {
    for (const auto& linha : dados) {
        for (const auto& coluna : linha) {
            printf("%s ", coluna.c_str());
        }
        printf("\n");
    }
}

/**
    Verifica se uma string representa um número.
    @param str A string a ser verificada.
    @return Verdadeiro se a string representa um número, falso caso contrário.
*/
bool ehNumero(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

/**
    Lê um arquivo e retorna seu conteúdo como uma matriz de strings.
    @param caminho O caminho do arquivo a ser lido.
    @return Uma matriz de strings representando o conteúdo do arquivo.
*/
vector<vector<string>> carregarArquivo(const string& caminho) {
    ifstream arquivo(caminho);
    string linhaTexto;
    vector<vector<string>> conteudo;
    vector<string> linha;

    while (getline(arquivo, linhaTexto)) {
        istringstream iss(linhaTexto);
        string token;
        while (getline(iss, token, ' ')) {
            linha.push_back(token);
        }
        if (!linha.empty()) {
            conteudo.push_back(linha);
            linha.clear();
        }
    }
    return conteudo;
}

/**
    Processa um arquivo e retorna as tabelas de definições, de uso, o mapa de bits e o código objeto.
    @param conteudo Uma matriz de strings representando o arquivo.
    @return Uma estrutura contendo as tabelas e o código objeto.
*/
Estruturas processarArquivo(const vector<vector<string>>& conteudo) {
    Estruturas resultado;
    bool lendoUso = true, lendoDef = false, lendoMapa = false, lendoCodigo = false;

    for (size_t i = 1; i < conteudo.size(); ++i) {
        if (conteudo[i][0] == "TABELA") {
            lendoDef = true;
            lendoUso = false;
            continue;
        } else if (conteudo[i][0] == "MAPA") {
            lendoMapa = true;
            lendoDef = false;
            continue;
        }

        if (lendoUso) {
            resultado.uso.push_back({conteudo[i][0], stoi(conteudo[i][1])});
        } else if (lendoDef) {
            resultado.definicoes[conteudo[i][0]] = stoi(conteudo[i][1]);
        } else if (lendoMapa) {
            for (const auto& token : conteudo[i]) {
                resultado.mapaBits.push_back(stoi(token));
            }
            lendoCodigo = true;
            lendoMapa = false;
        } else if (lendoCodigo) {
            for (const auto& token : conteudo[i]) {
                resultado.codigoObjeto.push_back(stoi(token));
            }
        }
    }

    return resultado;
}

/**
    Verifica se um endereço é um opcode e retorna o número de endereços que a instrução ocupa.
    @param endereco O endereço a ser verificado.
    @return O número de endereços que a instrução ocupa ou -1 se não for uma instrução.
*/
int obterOpcode(int endereco) {
    return (instrucoes.count(endereco) ? instrucoes[endereco] : -1);
}
