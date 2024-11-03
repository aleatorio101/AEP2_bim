#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME 100  
#define MAX_EMAIL 100 
#define MAX_PASSWORD 100 
#define FILENAME "users.txt"
#define ENCRYPTION_KEY "S3cr3tK3y2024"
#define SALT_LENGTH 8

typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char password[MAX_PASSWORD];
    int active;
} User;


void generateSalt(char *salt) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < SALT_LENGTH; i++) {
        int index = rand() % (sizeof(charset) - 1);
        salt[i] = charset[index];
    }
    salt[SALT_LENGTH] = '\0';
}


void reverseString(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}


void encrypt(char *text) {
    char salt[SALT_LENGTH + 1];
    generateSalt(salt);
    
    int textLen = strlen(text);
    int keyLen = strlen(ENCRYPTION_KEY);
    int saltLen = strlen(salt);
    

    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] ^ ENCRYPTION_KEY[i % keyLen];
    }
    

    for (int i = 0; text[i] != '\0'; i++) {
        int shift = (salt[i % saltLen] + i) % 26;
        text[i] = ((text[i] + shift) % 128);
    }
    
    
    reverseString(text);
    
    
    char temp[256];
    strcpy(temp, text);
    strcpy(text, salt);
    strcat(text, temp);
}


void decrypt(char *text) {
    if (strlen(text) <= SALT_LENGTH) {
        return;
    }

    char salt[SALT_LENGTH + 1];
    
    strncpy(salt, text, SALT_LENGTH);
    salt[SALT_LENGTH] = '\0';
    
    memmove(text, text + SALT_LENGTH, strlen(text) - SALT_LENGTH + 1);
    

    reverseString(text);
    
    
    for (int i = 0; text[i] != '\0'; i++) {
        int shift = (salt[i % SALT_LENGTH] + i) % 26;
        text[i] = ((text[i] - shift + 128) % 128);
    }
    
    
    int keyLen = strlen(ENCRYPTION_KEY);
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] ^ ENCRYPTION_KEY[i % keyLen];
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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

    printf("\nNome: ");
    clearInputBuffer();
    fgets(newUser.name, MAX_NAME, stdin);
    newUser.name[strcspn(newUser.name, "\n")] = 0;
    
    do {
        printf("\nEmail: ");
        fgets(newUser.email, MAX_EMAIL, stdin);
        newUser.email[strcspn(newUser.email, "\n")] = 0;
        if (!isValidEmail(newUser.email)) {
            printf("Email inválido! Por favor, insira um email válido.\n");
        }
    } while (!isValidEmail(newUser.email));

    printf("\nSenha: ");
    fgets(newUser.password, MAX_PASSWORD, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;

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
    printf("\nDigite o ID do usuário para atualizar: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].active) {
            printf("\nNovo nome (%s): ", users[i].name);
            fgets(users[i].name, MAX_NAME, stdin);
            users[i].name[strcspn(users[i].name, "\n")] = 0;
            
            do {
                printf("\nNovo email (%s): ", users[i].email);
                fgets(users[i].email, MAX_EMAIL, stdin);
                users[i].email[strcspn(users[i].email, "\n")] = 0;
                if (!isValidEmail(users[i].email)) {
                    printf("Email inválido! Por favor, insira um email válido.\n");
                }
            } while (!isValidEmail(users[i].email));
            
            printf("\nNova senha (pressione Enter para manter a atual): ");
            char newPassword[MAX_PASSWORD];
            fgets(newPassword, MAX_PASSWORD, stdin);
            newPassword[strcspn(newPassword, "\n")] = 0;

            if (strlen(newPassword) > 0) {
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
    clearInputBuffer();

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
    setlocale(LC_ALL, "");
    srand(time(NULL));
    
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
        printf("\nEscolha uma opção: ");
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
