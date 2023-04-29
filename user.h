#ifndef ICC_PROJ1_USER_H
#define ICC_PROJ1_USER_H

struct User {
    int id;
    char name[100];
    int age;
    float amount;
};

struct UserArray {
    struct User *users;
    int size;
};

#endif //ICC_PROJ1_USER_H
