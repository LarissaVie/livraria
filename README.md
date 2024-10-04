# 📚 Livraria Elegante 📚

Bem-vindo ao **Livraria Elegante**! Esta aplicação permite que você gerencie uma lista de livros de forma simples e eficiente. 

## 🔧 Funcionalidades

- **Adicionar Livros** 🆕: Insira detalhes como título, autor, preço e quantidade.
- **Remover Livros** ❌: Selecione um livro e remova-o rapidamente.
- **Buscar Livros** 🔍: Procure livros pelo título e veja suas informações.
- **Visualização Atraente** 🎨: Exibição dos livros em formato de "cards" para uma experiência visual agradável.

## ⚙️ Estrutura do Código

A aplicação utiliza uma lista encadeada dupla para armazenar os livros, garantindo uma manipulação eficiente dos dados.

## 🖥️ Video no youtube explicando informações: https://www.youtube.com/watch?v=ltJZ5JVjo0k

## Pré-requisitos

Antes de começar, você precisará das seguintes ferramentas:

- **MSYS2**: Ambiente Unix-like para Windows.
- **GTK**: Biblioteca para criação de interfaces gráficas.

### 1. Instalar MSYS2

Baixe e instale o [MSYS2](https://www.msys2.org/). Após a instalação, siga os passos abaixo:

1. Abra o terminal MSYS2 e atualize os pacotes:

   ```bash
   pacman -Syu

 - Reinicie o terminal e continue a atualização:
   
   ```bashh
   pacman -Su

2. Instalar GTK e Ferramentas de Desenvolvimento
No terminal MSYS2, instale o GTK3 e o conjunto de ferramentas de desenvolvimento:
   ```bash
   pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-toolchain
   
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-pkg-config


3. Compilação e Execução

   Navegue até o seu diretorio ex:cd /c/Users/Aluno/Desktop/atividade

   1. Compile o código usando o GCC no terminal MSYS2:
    ```bash
    gcc -o livraria livraria.c `pkg-config --cflags --libs gtk+-3.0`

2. Execute o programa:
    ```bash
    ./livraria.exe

### Verificar o PATH

Se após a instalação houver problemas com os comandos `gcc` ou `pkg-config`, você pode verificar se o caminho `/mingw64/bin` foi adicionado ao `PATH` do MSYS2. Isso garante que os comandos sejam encontrados corretamente.

Você pode adicionar o caminho manualmente executando o seguinte comando no terminal do MSYS2:

    ```bash
    export PATH=/mingw64/bin:$PATH

    
## 👩‍💻 Desenvolvedores [José Jhonata Vieira de Oliveira] [Larissa Vieira de Oliveira]
