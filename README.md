# AEP2_bim

## Sistema de Gerenciamento de Usuários e Senhas

Este sistema permite gerenciar usuários e suas senhas com criptografia de dados sensíveis. Ele armazena e carrega informações dos usuários e senhas em arquivos de texto separados, utilizando um algoritmo de criptografia para proteger os dados.

## Funcionalidades

### Gerenciamento de Usuários
* **Adicionar Usuário:** Adiciona um novo usuário ao sistema, solicitando nome, e-mail e senha
* **Listar Usuários:** Exibe uma lista de todos os usuários ativos com seu ID, nome e e-mail
* **Atualizar Usuário:** Atualiza o nome, e-mail e/ou senha de um usuário existente
* **Excluir Usuário:** Marca um usuário como inativo, o que o remove da lista de usuários ativos

### Gerenciamento de Senhas
* **Login:** Sistema de autenticação para acessar o gerenciador de senhas
* **Listar Senhas:** Exibe todas as senhas cadastradas do usuário logado
* **Adicionar Senha:** Permite cadastrar uma nova senha para uma plataforma

## Estrutura dos Dados

### Usuários (`users.txt`)
* **ID:** Número único que identifica cada usuário
* **Nome:** Nome completo do usuário
* **E-mail:** E-mail do usuário
* **Senha:** Senha de acesso ao sistema
* **Ativo:** Indica se o usuário está ativo (1) ou inativo (0)

### Senhas (`passwords.txt`)
* **UserID:** ID do usuário dono da senha
* **Plataforma:** Nome da plataforma/serviço
* **Senha:** Senha armazenada

## Segurança

### Criptografia
O sistema utiliza um algoritmo de criptografia simétrica para proteger dados sensíveis:
* Todas as senhas são criptografadas antes de serem salvas
* Dados pessoais (nome, email) são criptografados
* Uma chave de criptografia única é utilizada (`ENCRYPTION_KEY`)
* Implementação separada para encriptação e decriptação

## Funções Principais

### Gerenciamento de Usuários
* `addUser`: Adiciona um novo usuário
* `listUsers`: Exibe todos os usuários ativos
* `updateUser`: Atualiza informações de um usuário existente
* `deleteUser`: Marca um usuário como inativo
* `saveUsers`: Salva todos os usuários em `users.txt`
* `loadUsers`: Carrega os usuários de `users.txt`

### Gerenciamento de Senhas
* `loginUser`: Autentica o usuário no sistema
* `passwordManager`: Interface do gerenciador de senhas
* `addNewPassword`: Adiciona nova senha
* `listUserPasswords`: Lista senhas do usuário
* `savePasswordEntry`: Salva nova senha no arquivo

### Criptografia
* `encrypt`: Encripta dados antes de salvar
* `decrypt`: Decripta dados após carregar

## Limites do Sistema
* Máximo de 500 usuários
* Máximo de 1000 senhas por usuário
* Tamanho máximo para campos de texto: 500 caracteres

## Arquivos do Sistema
* `users.txt`: Armazena dados dos usuários
* `passwords.txt`: Armazena senhas dos usuários

## Requisitos
* Compilador C (GCC recomendado)
* Sistema operacional com suporte a locale PT-BR
* Mínimo de 1MB de espaço em disco
* Permissões de leitura/escrita na pasta do programa

## Como Compilar e Executar
```bash
gcc -o gerenciador_senhas main.c
./gerenciador_senhas
```

## Considerações de Segurança
* Senhas são sempre criptografadas antes de serem salvas
* Dados sensíveis nunca são armazenados em texto puro
* Sistema de login protege acesso às senhas
* Cada usuário só tem acesso às suas próprias senhas

## Limitações
* A criptografia implementada é básica e serve para fins educacionais
* Não há proteção contra força bruta
* Arquivos de dados não são compactados
* Não há backup automático dos dados
