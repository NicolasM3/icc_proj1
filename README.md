# icc_proj1

## Como rodar

```gcc -o Main main.c | ./Main```

## Pseudocódigo

```
tipo Usuario
    inteiro id
    caracter nome[100]
    real quantidade
fim_tipo

procedimento escreverNovaLinha(caracter arquivo[], caracter linha[])
    Arquivo arquivo <- abrirArquivo(arquivo, "anexar")

    se (arquivo == nulo) entao
        escreva("Erro ao abrir o arquivo: ", arquivo)
        retorne
    fim_se

    escreverArquivo(arquivo, linha + "\n")

    fecharArquivo(arquivo)
fim_procedimento

funcao Usuario lerUsuario(arquivoNome, id)
    Arquivo arquivo <- abrirArquivo(arquivoNome, "leitura")

    Usuario usuario

    se (arquivo == nulo) entao
        escreva("Erro: Não foi possível abrir o arquivo.")
        retorne usuario
    fim_se

    enquanto (naoFimDoArquivo(arquivo))
        usuario <- lerDados(arquivo)

        se (usuario.id == id) entao
            fecharArquivo(arquivo)
            retorne usuario
        fim_se
    fim_enquanto

    fecharArquivo(arquivo)
    retorne usuario
fim_funcao
```
