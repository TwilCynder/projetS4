#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    Blockchain* bc = create_blockchain(2);
    blockchain_add_transaction(bc, "User1", "User2", 999);
    blockchain_add_transaction(bc, "User2", "User3", 900);
    blockchain_add_transaction(bc, "User3", "User4", 810);
    mine(bc);

    blockchain_add_transaction(bc, "User1", "User2", 999);
    blockchain_add_transaction(bc, "User2", "User3", 900);
    blockchain_add_transaction(bc, "User3", "User4", 810);
    mine(bc);

    print_blockchain(bc);

    printf("%d\n", check_blockchain_coherence(bc));

    return 0;
}
