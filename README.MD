# Projeto de Compilador e Ligador

Este projeto contém um compilador e um ligador simples, permitindo a automação da construção e geração dos arquivos utilizando o `make`.

## Alunos

- Lucas Amaral de Faria
- Wesley Taveira

## Sistema Operacional

O projeto foi desenvolvido e testado em Linux.

## Como Usar

Após a execução do `make`, dois executáveis serão gerados no diretório raiz:
- `lig`: ligador
- `comp`: compilador

### Executando o Compilador

Para utilizar o compilador, você deve especificar se deseja realizar o pré-processamento ou a geração do código objeto, utilizando as flags correspondentes:
- `-p`: pré-processamento
- `-o`: geração do código objeto

Exemplos de uso:
```sh
./comp -p caminho_para_o_arquivo.txt
./comp -o caminho_para_o_arquivo.pre

### Executando o Ligador

Para utilizar o ligador, forneça como argumentos os caminhos para os arquivos objeto:
```sh
./lig caminho_para_arquivo1.o caminho_para_arquivo2.o

## Comando `make`

O comando `make` compila todo o projeto, gerando os executáveis necessários para o compilador e o ligador.

```sh
make
