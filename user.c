#include <stdio.h>

#include "user.h"

void printUser(struct User user) {
    printf("%d %s %d %.2f", user.id, user.name, user.age, user.amount);
}

void printUserFromPointer(struct User* user) {
    printf("%d %s %d %.2f", user->id, user->name, user->age, user->amount);
}