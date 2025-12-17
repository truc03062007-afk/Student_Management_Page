#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ================= CONSTANTS ================= */
#define MAX_USERS 100
#define MAX_UNITS 50
#define MAX_STUDENT_UNITS 3
#define MAX_LINE 512

/* ================= STRUCTS ================= */
typedef struct {
    char unit_code[10];
    int score;
} EnrolledUnit;

typedef struct {
    int user_id;
    char username[50];
    char password[50];
    char role[4];
    char status[10];
    EnrolledUnit enrolled_units[MAX_STUDENT_UNITS];
    int num_enrolled_units;
} User;

typedef struct {
    int unit_id;
    char unit_code[10];
    char unit_name[100];
    int capacity;
    int current_enrollment;
} Unit;

/* ================= GLOBAL ================= */
User users[MAX_USERS];
Unit units[MAX_UNITS];
int user_count = 0;
int unit_count = 0;

/* ================= UTIL ================= */
void pause_screen(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

void to_uppercase(char *s) {
    for (int i = 0; s[i]; i++) s[i] = toupper(s[i]);
}

int find_unit_by_code(const char *code) {
    for (int i = 0; i < unit_count; i++)
        if (strcmp(units[i].unit_code, code) == 0)
            return i;
    return -1;
}

/* ================= CSV LOAD ================= */
int load_units(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char line[MAX_LINE];

    if (!fp) return -1;

    fgets(line, sizeof(line), fp); // header

    while (fgets(line, sizeof(line), fp)) {
        Unit u;
        sscanf(line, "%d,%9[^,],%99[^,],%d,%d",
               &u.unit_id,
               u.unit_code,
               u.unit_name,
               &u.capacity,
               &u.current_enrollment);
        units[unit_count++] = u;
    }
    fclose(fp);
    return unit_count;
}

int load_users(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char line[MAX_LINE];

    if (!fp) return -1;

    fgets(line, sizeof(line), fp); // header

    while (fgets(line, sizeof(line), fp)) {
        User u;
        char extra[200] = "";

        sscanf(line, "%d,%49[^,],%49[^,],%3[^,],%9[^,],%199[^\n]",
               &u.user_id,
               u.username,
               u.password,
               u.role,
               u.status,
               extra);

        u.num_enrolled_units = 0;

        if (strcmp(u.role, "ST") == 0 && strlen(extra) > 0) {
            char *token = strtok(extra, ";");
            while (token && u.num_enrolled_units < MAX_STUDENT_UNITS) {
                sscanf(token, "%9[^:]:%d",
                       u.enrolled_units[u.num_enrolled_units].unit_code,
                       &u.enrolled_units[u.num_enrolled_units].score);
                u.num_enrolled_units++;
                token = strtok(NULL, ";");
            }
        }
        users[user_count++] = u;
    }
    fclose(fp);
    return user_count;
}

/* ================= CSV SAVE ================= */
int save_units(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;

    fprintf(fp, "unit_id,unit_code,unit_name,capacity,current_enrollment\n");
    for (int i = 0; i < unit_count; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d\n",
                units[i].unit_id,
                units[i].unit_code,
                units[i].unit_name,
                units[i].capacity,
                units[i].current_enrollment);
    }
    fclose(fp);
    return unit_count;
}

int save_users(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;

    fprintf(fp, "user_id,username,password,role,status,additional_data\n");

    for (int i = 0; i < user_count; i++) {
        User *u = &users[i];
        fprintf(fp, "%d,%s,%s,%s,%s,",
                u->user_id,
                u->username,
                u->password,
                u->role,
                u->status);

        if (strcmp(u->role, "ST") == 0) {
            for (int j = 0; j < u->num_enrolled_units; j++) {
                fprintf(fp, "%s:%d",
                        u->enrolled_units[j].unit_code,
                        u->enrolled_units[j].score);
                if (j < u->num_enrolled_units - 1)
                    fprintf(fp, ";");
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return user_count;
}
int find_user_name(const char *user){
     for (int i = 0; i < user_count; i++){
        if (strcmp(users[i].username, user) == 0){
            return i;
        }
     }
     printf("Error: Username not found!");
     return -1;
    
}
int find_user_pass(const char *pass){
     for (int i = 0; i < user_count; i++){
        if(strcmp(users[i].password, pass) == 0){
            return i;
        }
    }
    printf("Error: Incorrect password!");
    return -1;
}
int is_user_enabled(const char *user){
    int user_index=find_user_name(user);
        if(strcmp(users[user_index].status, "enabled") == 0){
            return user_index;
        }
        printf("Error: Account is disabled. Contact admin.");
    return -1;
}


/* ================= AUTH ================= */
int login(const char *user, const char *pass) {
int idx=find_user_name(user);
    if(idx !=-1){
        idx=find_user_pass(pass);
        if(idx !=-1){
            idx=is_user_enabled(user);
           if(idx !=-1){
            return idx;
           } 
        }
    }
    return -1;
}
int find_user_by_id(int user_id){
    for (int i = 0; i < user_count; i++){
        if(strcmp(users[i].id, user_id) == 0){
            return i;
        }
    }
    return -1;
}
int find_unit_by_code(const char* unit_code){
    for (int i = 0; i < user_count; i++){
        if(strcmp(units[i].unit_code, unit_code) == 0){
            return i;
        }
    }
    return -1;
}
int get_unit_enrollment_count(const char* unit_code){
    int j=0;
    User *u = &users[i];

    for (int i = 0; i < user_count; i++){
        if(strcmp(users[i].role, "ST") == 0){
            for(int q=0;q< u->num_enrolled_units;q++){
                if(strcmp(u->enrolled_units[q].unit_code, unit_code) == 0){
            j++;
                }
            }
        }
    }
    return j;
}




/* ================= STUDENT ================= */
void list_available_units(void) {
    printf("\n=== AVAILABLE UNITS ===\n");
    for (int i = 0; i < unit_count; i++)
        printf("%s - %s (%d/%d)\n",
               units[i].unit_code,
               units[i].unit_name,
               units[i].current_enrollment,
               units[i].capacity);
}

void enroll_unit(int idx) {
    char code[10];
    User *st = &users[idx];

    if (st->num_enrolled_units >= MAX_STUDENT_UNITS) {
        printf("Max units reached!\n");
        return;
    }

    printf("Unit code: ");
    fgets(code, sizeof(code), stdin);
    code[strcspn(code, "\n")] = 0;
    to_uppercase(code);

    int u = find_unit_by_code(code);
    if (u == -1 || units[u].current_enrollment >= units[u].capacity) {
        printf("Cannot enroll!\n");
        return;
    }

    strcpy(st->enrolled_units[st->num_enrolled_units].unit_code, code);
    st->enrolled_units[st->num_enrolled_units].score = -1;
    st->num_enrolled_units++;
    units[u].current_enrollment++;

    printf("Enrolled successfully!\n");
}

/* ================= MENU ================= */
void student_menu(int idx) {
    int c;
    do {
        printf("\n1.List units\n2.Enroll\n3.Logout\nChoice: ");
        scanf("%d", &c); getchar();
        if (c == 1) list_available_units();
        else if (c == 2) enroll_unit(idx);
    } while (c != 3);
}

/* ================= MAIN ================= */
int main(void) {
    srand(time(NULL));

    if (load_users("users.csv") == -1 || load_units("units.csv") == -1) {
        printf("CSV load failed!\n");
        return 1;
    }

    char u[50], p[50];
    printf("Username: ");
    fgets(u, sizeof(u), stdin);
    u[strcspn(u, "\n")] = 0;

    printf("Password: ");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;


    int idx = login(u, p);
    if (idx == -1) {
        printf("Login failed!\n");
        return 0;
    }

    student_menu(idx);

    save_users("users.csv");
    save_units("units.csv");

    printf("Data saved. Goodbye!\n");
    return 0;
}
