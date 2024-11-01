# AEP2_bim
# Sistema de Gerenciamento de Usuários

Este sistema permite gerenciar usuários com criptografia básica de dados sensíveis. Ele armazena e carrega informações dos usuários em um arquivo de texto, utilizando uma cifra de substituição para proteger os dados. 

## Funcionalidades

- **Adicionar Usuário**: Adiciona um novo usuário ao sistema, solicitando nome e e-mail. O e-mail é validado para garantir que possui "@" e algum texto após este símbolo.
- **Listar Usuários**: Exibe uma lista de todos os usuários ativos com seu ID, nome e e-mail.
- **Atualizar Usuário**: Atualiza o nome e/ou e-mail de um usuário existente.
- **Excluir Usuário**: Marca um usuário como inativo, o que o remove da lista de usuários ativos.

## Estrutura dos Dados

- **ID**: Número único que identifica cada usuário.
- **Nome**: Nome completo do usuário.
- **E-mail**: E-mail do usuário, que é validado para incluir "@" e algum texto após o símbolo.
- **Ativo**: Indica se o usuário está ativo (1) ou inativo (0).

## Criptografia

O sistema utiliza uma criptografia simples de substituição, somando 3 a cada caractere para encriptar dados de nome e e-mail ao salvá-los. Ao carregar os dados, o sistema subtrai 3 de cada caractere para descriptografá-los.

## Estrutura do Arquivo `users.txt`

Cada linha no arquivo segue o formato:

- **ID**: Identificador do usuário.
- **Nome (criptografado)**: Nome do usuário após ser criptografado.
- **Email (criptografado)**: E-mail do usuário após ser criptografado.
- **Ativo**: Status do usuário (1 para ativo, 0 para inativo).

## Funções Principais

- `addUser`: Adiciona um novo usuário com validação de e-mail.
- `listUsers`: Exibe todos os usuários ativos.
- `updateUser`: Atualiza informações de um usuário existente.
- `deleteUser`: Marca um usuário como inativo.
- `saveUsers`: Salva todos os usuários ativos em `users.txt`.
- `loadUsers`: Carrega os usuários ativos de `users.txt`.
- `encrypt` e `decrypt`: Funções de criptografia e descriptografia para nome e e-mail.

## Requisitos

- **Compilador C** (como `gcc`)
- **Locale PT-BR UTF-8** para exibir mensagens corretamente
