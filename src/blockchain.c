#include "blockchain.h"
#include "sha256_utils.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "transaction.h"
#include "list.h"
#include "queue.h"

#include "printf_colors.h"

struct s_blockchain{
    int difficulty;
    List* blocks;
    Queue* transac_q;
};

void block_add_transaction(Block* block, char* transac);

Blockchain* create_blockchain(int difficulty){
    Blockchain* bc = malloc(sizeof(Blockchain));
    bc->difficulty = difficulty;
    bc->blocks = create_list();
    bc->transac_q = create_queue();
    list_push(bc->blocks, (void*)create_genesis());
    return bc;
}

int blockchain_size(Blockchain* bc){
    return list_size(bc->blocks);
}

Block* add_block(Blockchain* bc){
    Block* block = create_block(blockchain_size(bc), list_front(bc->blocks));
    list_push(bc->blocks, (void*)block);
    return block;
}

void blockchain_add_transaction(Blockchain* bc, char* user1, char* user2, double amount){
    queue_push(bc->transac_q, (void*)create_transaction(user1, user2, amount));
}

void mine(Blockchain* bc){
    Block* block = add_block(bc);

    mine_block(block, bc->transac_q, bc->difficulty);
}

void print_block_operator(void* block){
    print_block((Block*)block);
}

void print_blockchain(Blockchain* bc){
    printf("%sBlockchain information : %s\n", COLOR_CYAN, COLOR_RESET);
    printf("Difficulty : %d\n", bc->difficulty);
    printf("%d transactions are currently stored.\n", queue_size(bc->transac_q));
    list_reduce(bc->blocks, &print_block_operator);
}

bool check_blockchain_coherence(Blockchain* bc){
    printf("%sStarting blockchain verification%s\n", COLOR_MAG, COLOR_RESET);

    if (list_size(bc->blocks) == 0) return false;   //A blockchain is always supposed to contain at least one block (the Genesis)

    Iterator* it = create_iterator(bc->blocks);     //No need to check iterator_end(), thanks to the previous check

    Block* previous = NULL;
    Block* current = iterator_value(it);

    if (!is_genesis(current)) return false;

    do {
        if (!check_block_coherence(current, previous)){
            return false;
        }
        iterator_next(it);
        previous = current;
        current = iterator_value(it);
    } while (!iterator_end(it));

    return true;
}