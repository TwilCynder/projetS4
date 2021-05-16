#include <stdlib.h>
#include <stdio.h>
#include "transaction.h"

#define TRANSAC_SIZE 128
#define SATO 10000000000

Transaction create_transaction(char* user1, char* user2, double amount){
    char* t = (char*)malloc(TRANSAC_SIZE);
    //The last %d is a random number, to make transactions more realistic.
    //We are aware that rand() % m produces an very low-quality random output, but it doesn't seem too important here.
    sprintf(t, "%s envoie %lld satoBnB à %s | %d", user1, (long long)amount * SATO , user2, rand() % 99);
    return t;
}