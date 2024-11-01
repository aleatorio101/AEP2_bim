#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_USERS 100
#define MAX_NAME 50
#define MAX_EMAIL 50
#define MAX_PASSWORD 50
#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char password[MAX_PASSWORD];
    int active;
} User;

void encrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] + 3;
    }
}

void decrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] - 3;
    }
}

void saveUsers(User users[], int count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (users[i].active) {
            char name[MAX_NAME];
            char email[MAX_EMAIL];
            char password[MAX_PASSWORD];
            
            strcpy(name, users[i].name);
            strcpy(email, users[i].email);
            strcpy(password, users[i].password);
            
            encrypt(name);
            encrypt(email);
            encrypt(password);
            
            fprintf(file, "%d|%s|%s|%s|1\n", users[i].id, name, email, password);
        }
    }
    fclose(file);
}

int loadUsers(User users[]) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    FILE *file = fopen(FILENAME, "r");
    int count = 0;
    
    if (file == NULL) {
        return 0;
    }

    char line[MAX_NAME + MAX_EMAIL + MAX_PASSWORD + 20];
    while (fgets(line, sizeof(line), file) && count < MAX_USERS) {
        char name[MAX_NAME];
        char email[MAX_EMAIL];
        char password[MAX_PASSWORD];
        
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%d", 
               &users[count].id, 
               name, 
               email, 
               password,
               &users[count].active);
        
        decrypt(name);
        decrypt(email);
        decrypt(password);
        
        strcpy(users[count].name, name);
        strcpy(users[count].email, email);
        strcpy(users[count].password, password);
        
        count++;
    }
    
    fclose(file);
    return count;
}

int isValidEmail(const char *email) {
    return strchr(email, '@') != NULL;
}

void addUser(User users[], int *count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    if (*count >= MAX_USERS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    User newUser;
    newUser.id = *count + 1;
    newUser.active = 1;

    printf("Nome: ");
    scanf(" %[^\n]", newUser.name);
    
    do {
        printf("Email: ");
        scanf(" %[^\n]", newUser.email);
        if (!isValidEmail(newUser.email)) {
            printf("Email inválido! Por favor, insira um email válido.\n");
        }
    } while (!isValidEmail(newUser.email));

    printf("Senha: ");
    scanf(" %[^\n]", newUser.password);

    encrypt(newUser.password);
    users[*count] = newUser;
    (*count)++;
    
    saveUsers(users, *count);
    printf("Usuário adicionado com sucesso!\n");
}

void listUsers(User users[], int count) {
    printf("\n=== Lista de Usuários ===\n");
    for (int i = 0; i < count; i++) {
        if (users[i].active) {
            printf("ID: %d\n", users[i].id);
            printf("Nome: %s\n", users[i].name);
            printf("Email: %s\n", users[i].email);
            printf("------------------------\n");
        }
    }
}

void updateUser(User users[], int count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int id;
    printf("Digite o ID do usuário para atualizar: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].active) {
            printf("Novo nome (%s): ", users[i].name);
            scanf(" %[^\n]", users[i].name);
            
            printf("Novo email (%s): ", users[i].email);
            scanf(" %[^\n]", users[i].email);
            
            printf("Nova senha (deixe em branco para manter a atual): ");
            char newPassword[MAX_PASSWORD];
            scanf(" %[^\n]", newPassword);

            if (strlen(newPassword) > 0) {
                encrypt(newPassword);
                strcpy(users[i].password, newPassword);
            }
            
            saveUsers(users, count);
            printf("Usuário atualizado com sucesso!\n");
            return;
        }
    }
    printf("Usuário não encontrado!\n");
}

void deleteUser(User users[], int count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int id;
    printf("Digite o ID do usuário para excluir: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].active) {
            users[i].active = 0;
            saveUsers(users, count);
            printf("Usuário excluído com sucesso!\n");
            return;
        }
    }
    printf("Usuário não encontrado!\n");
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    User users[MAX_USERS];
    int userCount = loadUsers(users);
    int option;
    

    do {
        printf("\n=== Sistema de Gerenciamento de Usuários ===\n");
        printf("1. Adicionar Usuário\n");
        printf("2. Listar Usuários\n");
        printf("3. Atualizar Usuário\n");
        printf("4. Excluir Usuário\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch(option) {
            case 1:
                addUser(users, &userCount);
                break;
            case 2:
                listUsers(users, userCount);
                break;
            case 3:
                updateUser(users, userCount);
                break;
            case 4:
                deleteUser(users, userCount);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(option != 0);

    return 0;
}


