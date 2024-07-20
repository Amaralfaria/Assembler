# Projeto de Compilador e Ligador

Este projeto contém um compilador e um ligador simples. Você pode usar o `make` para automatizar a construção e geração dos arquivos

## Como Usar

O make irá gerar dois executaveis no diretorio raiz, lig referente ao ligador e comp referente ao compilador.

Ao executar o compilador deve ser especificado se será pre-processamento com a flag -p e geração do codigo objeto com a flag -o.

Ao executar o ligador devem ser especificados os modulos como argumentos.

### Comando `make`

O comando `make` compila o projeto inteiro, gerando os arquivos necessários para o compilador e o ligador.

```sh
make
