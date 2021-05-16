#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_TRANSACTIONS 10
#define MIN_TRANSACTIONS 1
#define MAX_TRANSACTIONS_MONTANT 100
#define NB_UTILISATEURS 10

typedef char* Transaction;

void transaction_print(Transaction);
Transaction generate_random_transaction();