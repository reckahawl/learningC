/*
C language starts with a header file (library) that has pre defined functions
for use in your code use in you
*/
#include <ctypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_DATA 512
#define MAX_ROW 100

// Everyting in C is initialized before use
int main(int argc, char *argv[]){
    // integer
    int distance = 100;
    // float 
    float power = 2.345f;
    // double
    double super_power = 5678.9876;
    // character
    char first_name[] = "Lewis";
    // long
    long universe_defects = 1L * 1024L * 1234L;
    
    // print formated is a function in stdio.h
    printf("my name is %s and I have %d long to go.\n",first_name, distance);
    printf("universal defect is %ld", universe_defects);
    return 0;

}

int
main(int argc, char *argv[]){
    int i = 0;
    while (i < 20 && i > 0 || i != '\0'){
        if (i % 2 == 0){
            printf("%d is an even number", i);
        }else{
            printf("%d is an odd number", i);
        }
        i ++ ;
    }
    return 0;
}
/*
int
main(int argc, char *argv[]){
    // initializing array
    char vowel[] = {"a", "e", "i", "o", "u", "A", "E","I","O","U"};
    char *vowels = "a,e,i,o,u"

    int i = 0;
    for (i=0; i < argc && argv[1] != '\0'; i++){
        if (argv != '\0'){
            char Args[] = {argv[i]};        
        }else{
            break;
        }

    }
}
*/

// Print letters

int can_print(char ch);
void print_letters(char arg[]);
void print_args(int argc, char *argv[]){
    int i = 0;
    for (i=0; i < argc; i++){
        print_letters();
    }
}

void print_letters(char arg[]){
    int i = 0;
    for (i = 0; arg[i] != '\0'; i++){
        char ch = arg[i];
        if (can_print(ch)){
            printf("%s == %d", ch, ch);
        }
    }
    printf("\n")
}
int can_print(char ch){
    return isalpha(ch) || isblank(ch);
}
int main(int argc, char *argv[]){
    print_args(argv);
    return 0;
}

// Pointers is what to be explained below

int main(int argc, char *argv[]){
    int ages[]={23,43,12,89,2};
    char *names[]={"Deelaw","Coder","Lens","Ajey","Lewis"};
    int count = sizeof(ages)/sizeof(int);
    int i = 0;
    for (i =0; i<count; i++){
        printf("%s has %d years alive.\n", names[i],ages[i]);
    }
    printf("--------------\n");
    // Pointer setup
    int *cur_age = ages;
    char **cur_name = names;
    for (i=0, i<count; i++){
        printf("%s is %d years old.\n",*(cur_name + i), (cur_age + i));
    }
    printf("--------------\n");
    // Pointers are just array
    for (i=0; i<count; i++){
        printf("%s is %d years old.\n", cur_name[i], cur_age[i]);
    }
    printf("--------------\n");

    // Pointers as stupid complex 
    for (cur_name = names, cur_age = ages;(cur_name - names, cur_age - ages)<count; cur_name++, cur_age++){
         printf("%s is %d years old.\n", *cur_name, *cur_age);
    }
    return 0; 
}


// Struct and its manipulation

struct Person{
    char *name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight){
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != '\0');
    who -> name = strdup(name);
    who -> age = age;
    who -> height = height;
    who -> weight = weight;
    return who;
}
void Person_destroy(struct Person *who){
    assert(who != NULL);
    free(who -> name);
    free(who);
}
void Person_print(struct Person *who){
    printf("Name %s\n", who->name);
    printf("Age %d\n", who->age);
    printf("Height %d\n", who->height);
    printf("Weight %d\n", who->weight);
}
int main(int argc, char *argv[]){
    //Make two people structure
    struct Person *lens = Person_create("Lens Ajey", 32, 64, 140);
    struct Person *law = Person_create("Dee Law", 20, 72, 180);

    printf("Lens is at memory location %p:\n", lens);
    printf("Deeis at memory location %p:\n", law);

    Person_print(lens->age += 20;lens->height-=2;lens->weight+=40);

    Person_destroy(lens);
    Person_destroy(law);

    return 0; 
}




/*
// Struct and Stack

struct Address{
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};
struct Database{
    struct Address rows[MAX_ROW];
};
struct Connection{
    FILE *file;
    struct Database *db;
};

void die(const char *massage){
    if (errno){
        perror(1);
    }else{
        printf("%s\n", message);
    }
    exit(1);
}

void Address_print(struct Address *addr){
    printf("%d %s %s\n", addr->id, addr->name, addr->email); 
}

void Database_loads(struct Connection conn){
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc !=1) die("loads database.");
    struct Connection *Database_open(const char *filename, char mode){
        struct Connection *conn = malloc(sizeof(struct Connection));
        if (!conn) die("error"); 
        conn->db = malloc(sizeof(struct Database));
        if (!conn->db) die("Mem error");
        if (mode == 'c'){
            conn->file = fopen(filename, "w");
        }else{
            conn->file = fopen(filename, "r+");
        }
    }
}

*/

