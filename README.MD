# Projeto de Montador e Ligador

Este projeto contém um montador e um ligador simples, permitindo a automação da construção e geração dos arquivos utilizando o `make`.

## Alunos

- Lucas Amaral de Faria
- Wesley Taveira

## Sistema Operacional

O projeto foi desenvolvido e testado em Linux.

## Como Usar

Após a execução do `make`, dois executáveis serão gerados no diretório raiz:
- `ligador`: ligador
- `montador`: montador

### Executando o Montador

Para utilizar o Montador, você deve especificar se deseja realizar o pré-processamento ou a geração do código objeto, utilizando as flags correspondentes:
- `-p`: pré-processamento
- `-o`: geração do código objeto

Exemplos de uso:
```sh
./montador -p caminho_para_o_arquivo.asm
./montador -o caminho_para_o_arquivo.pre
```

Será gerado, respectivamente:
```
caminho_para_o_arquivo.pre
caminho_para_o_arquivo.obj
```

### Executando o Ligador

Para utilizar o ligador, forneça como argumentos os caminhos para os arquivos objeto:
```sh
./ligador caminho_para_arquivo1.obj caminho_para_arquivo2.obj
```

Será gerado:
```
caminho_para_arquivo1.e
```

## Comando `make`

O comando `make` compila todo o projeto, gerando os executáveis necessários para o Montador e o ligador.

```sh
make
```
