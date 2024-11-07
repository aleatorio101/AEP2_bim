#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME 100  
#define MAX_EMAIL 100 
#define MAX_PASSWORD 100 
#define FILENAME "users.txt"
#define ENCRYPTION_KEY "SimpleKey"

typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char password[MAX_PASSWORD];
    int active;
} User;

void encryptDecrypt(char *text) {
    int keyLen = strlen(ENCRYPTION_KEY);
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] ^= ENCRYPTION_KEY[i % keyLen];
    }
}

void listUsers(User users[], int count) {
    printf("\n=== Lista de Usuários ===\n");
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (users[i].active) {
            printf("ID: %d\n", users[i].id);
            printf("Nome: %s\n", users[i].name);
            printf("Email: %s\n", users[i].email);
            printf("------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("Nenhum usuário cadastrado.\n");
    }
}

void saveUsers(User users[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (users[i].active) {
            char encName[MAX_NAME];
            char encEmail[MAX_EMAIL];
            char encPassword[MAX_PASSWORD];
            
            strncpy(encName, users[i].name, MAX_NAME - 1);
            strncpy(encEmail, users[i].email, MAX_EMAIL - 1);
            strncpy(encPassword, users[i].password, MAX_PASSWORD - 1);
            
            encryptDecrypt(encName);
            encryptDecrypt(encEmail);
            encryptDecrypt(encPassword);
            
            fprintf(file, "%d|%s|%s|%s|%d\n", 
                    users[i].id,
                    encName, encEmail, encPassword,
                    users[i].active);
        }
    }
    fclose(file);
}

int loadUsers(User users[]) {
    FILE *file = fopen(FILENAME, "r");
    int count = 0;
    
    if (file == NULL) {
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) && count < MAX_USERS) {
        line[strcspn(line, "\n")] = 0;
        
        int id, active;
        char encName[MAX_NAME], encEmail[MAX_EMAIL], encPassword[MAX_PASSWORD];
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%d",
                   &id, encName, encEmail, encPassword, &active) == 5) {
                   
            users[count].id = id;
            users[count].active = active;

            encryptDecrypt(encName);
            encryptDecrypt(encEmail);
            encryptDecrypt(encPassword);
            
            strncpy(users[count].name, encName, MAX_NAME - 1);
            strncpy(users[count].email, encEmail, MAX_EMAIL - 1);
            strncpy(users[count].password, encPassword, MAX_PASSWORD - 1);
            
            users[count].name[MAX_NAME - 1] = '\0';
            users[count].email[MAX_EMAIL - 1] = '\0';
            users[count].password[MAX_PASSWORD - 1] = '\0';
            
            count++;
        }
    }
    fclose(file);
    return count;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addUser(User users[], int *count) {
    if (*count >= MAX_USERS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    User newUser;
    newUser.id = *count + 1;
    newUser.active = 1;

    printf("\nNome: ");
    clearInputBuffer();
    fgets(newUser.name, MAX_NAME, stdin);
    newUser.name[strcspn(newUser.name, "\n")] = 0;
    
    printf("\nEmail: ");
    fgets(newUser.email, MAX_EMAIL, stdin);
    newUser.email[strcspn(newUser.email, "\n")] = 0;

    printf("\nSenha: ");
    fgets(newUser.password, MAX_PASSWORD, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;

    users[*count] = newUser;
    (*count)++;
    
    saveUsers(users, *count);
    printf("Usuário adicionado com sucesso!\n");
}

void updateUser(User users[], int count) {
    int id;
    printf("\nDigite o ID do usuário que deseja alterar: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].active) {
            printf("Digite o novo nome (atual: %s): ", users[i].name);
            clearInputBuffer();
            fgets(users[i].name, MAX_NAME, stdin);
            users[i].name[strcspn(users[i].name, "\n")] = 0;

            printf("Digite o novo email (atual: %s): ", users[i].email);
            fgets(users[i].email, MAX_EMAIL, stdin);
            users[i].email[strcspn(users[i].email, "\n")] = 0;

            printf("Digite a nova senha: ");
            fgets(users[i].password, MAX_PASSWORD, stdin);
            users[i].password[strcspn(users[i].password, "\n")] = 0;

            saveUsers(users, count);
            printf("Usuário atualizado com sucesso!\n");
            return;
        }
    }
    printf("Usuário com ID %d não encontrado ou inativo.\n", id);
}

void deleteUser(User users[], int count) {
    int id;
    printf("\nDigite o ID do usuário que deseja excluir: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].active) {
            users[i].active = 0;
            saveUsers(users, count);
            printf("Usuário excluído com sucesso!\n");
            return;
        }
    }
    printf("Usuário com ID %d não encontrado ou já inativo.\n", id);
}

int main() {
    setlocale(LC_ALL, "");

    User users[MAX_USERS];
    int userCount = loadUsers(users);

    int option;
    do {
        printf("\nMenu:\n");
        printf("1. Listar Usuários\n");
        printf("2. Adicionar Usuário\n");
        printf("3. Alterar Usuário\n");
        printf("4. Excluir Usuário\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                listUsers(users, userCount);
                break;
            case 2:
                addUser(users, &userCount);
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
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (option != 0);

    return 0;
}

