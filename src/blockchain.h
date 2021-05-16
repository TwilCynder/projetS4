#include "block.h"
#include <stdbool.h>

typedef struct s_blockchain Blockchain;

Blockchain* create_blockchain(int difficulty);
int blockchain_size(Blockchain* bc);
void blockchain_add_transaction(Blockchain* bc, char* user1, char* user2, double amount);
Block* add_block(Blockchain* bc);
char* create_transaction(char* user1, char* user2, double amount);
void mine(Blockchain* bc);
bool check_blockchain_coherence(Blockchain* bc);

void print_blockchain(Blockchain* bc);
