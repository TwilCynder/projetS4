#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "block.h"
#include "list.h"
#include "transaction.h"
#include "printf_colors.h"
#include "sha256_utils.h"

typedef unsigned char BYTE;

#define HASH_SIZE 65
#define MAX_TRANSAC_BLOCK 10
#define BLOCK_STRING_SIZE 193 //Enough space for 2 hashes (64 chars each), a timestamp (24 chars at most) and a few ints

struct s_block {
    char* timestamp;
    int index;
    List* transactions;
    char* hash_merkle_transactions;
    char* hash_current;
    char* hash_previous;
    int nonce;
};

char * getTimeStamp(){
    time_t ltime;
    time(&ltime);
    return ctime(&ltime);
}

/**
 * @brief Returns a random int between min and max
 * Very low quality random
 * @param min 
 * @param max 
 * @return int 
 */
int random_int(int min, int max){
    return (rand() % (max - min)) + min; 
}

/**
 * @brief Create a block object.
 * Does not allocate memory for the hash and merkle_hash fields.
 * @param index the index in the blockchain
 * @param previous the previous block in the blockchain
 * @return Block* the created block.
 */
Block* create_block(int index, Block* previous){
    Block* block = malloc(sizeof(Block));
    block->timestamp = getTimeStamp();
    block->nonce = 0;
    block->index = index;
    block->hash_current = malloc(HASH_SIZE);
    block->hash_previous = malloc(HASH_SIZE);
    block->hash_merkle_transactions = malloc(HASH_SIZE);
    block->transactions = create_list();

    if (previous){
        sprintf(block->hash_previous, "%s", previous->hash_current);
    }

    return block;
};

/**
 * @brief Generates the hash of a block.
 * Returns a pointer to an mallocated string that must be freed.
 * Takes account of the timestamp, the index, the nonce, the hash of the previous block, the merkle root of the transactions.
 * @param block 
 * @param output buffer where the resulting hash will be written
 * @return char* 
 */
void hash_block(Block* block, char output[HASH_SIZE]){
    char block_string[BLOCK_STRING_SIZE];
    snprintf(block_string, BLOCK_STRING_SIZE, "%s%d%s%d%s", block->timestamp, block->index, block->hash_previous, block->nonce, block->hash_merkle_transactions);
    sha256ofString((BYTE*)block_string, output);
}

/**
 * @brief Calculates the hash of a block and store it in the corresponding field of that block
 * 
 * @param block 
 */
void update_block_hash(Block* block){
    hash_block(block, block->hash_current);
}

/**
 * @brief Generate the merkle root of the transaction list of a block.
 * 
 * @param block 
 * @param output rhe buffer where the result will be stored
 * @return char* 
 */
void merkle(Block* block, char* output){
    //Omitting the use of "sizeof(char)"
    int nb_hashes = list_size(block->transactions);
    
    char** hashes = malloc(nb_hashes);
    hashes[0] = output;
    for (int i = 1; i < nb_hashes; i++){
        hashes[i] = malloc(HASH_SIZE);  //i'm not very happy with this method, as i could have made just one big malloc(nb_hashes * HASH_SIZE)), but handling it was complicated and led to ugly code
    }
    char* concat = malloc(HASH_SIZE * 2);

    Iterator* it = create_iterator(block->transactions);
    int current = 0;

    while (!iterator_end(it)){
        sha256ofString((BYTE*)iterator_value(it), hashes[current]);
        current++;
        iterator_next(it);
    }

    current = 0;

    while (nb_hashes > 1){
        if (current * 2 >= nb_hashes){
            nb_hashes = current;
            current = 0;
        } else if (nb_hashes - (current * 2) == 1){ //There is a "lone" hash
            hashes[current] = hashes[current * 2]; //We simply pull it up
            current++;
        } else {
            sprintf(concat, "%s%s", hashes[current * 2], hashes[(current * 2) + 1]);
            sha256ofString((BYTE*)concat, hashes[current]);
            current++;
        }
    }

    free(concat);
    for (int i = 1; i < nb_hashes; i++){
        free(hashes[i]);
    }
    free(hashes);
}

/**
 * @brief Generates the merkle root of a block and store it in the corresponding field of that block
 * 
 */

void update_merkle_hash(Block* block){
    merkle(block, block->hash_merkle_transactions);
}

/**
 * @brief Returns a hash respects the requirement
 * The said requirements are "the hash must start with a certain number of zeros"
 * @param block the tested block
 * @param difficulty the number of zeros required
 * @return int 
 */
int check_hash_condition(char* hash, int difficulty){
    if (difficulty > HASH_SIZE - 1) difficulty = HASH_SIZE - 1;
    for (int i = 0; i < difficulty; i++){
        if (hash[i] != '0'){
            return 0;
        }
    }
    return 1;
}

void print_transaction_operator(void* transac){
    printf("    %s\n", (char*)transac);
}

void print_block(Block* block){
    printf("%sBlock %d\n%s", COLOR_YELLOW, block->index, COLOR_RESET);
    printf("Nonce : %d ; Timestamp : %s", block->nonce, block->timestamp);
    int nb_transac = list_size(block->transactions);
    printf("%d transactions : \n", nb_transac);
    list_reduce(block->transactions, &print_transaction_operator);
    printf("Merkle : %s\nHash   : %s\nPrevious Hash : %s\n", block->hash_merkle_transactions, block->hash_current, block->hash_previous);
}

void block_add_transaction(Block* block, char* transac){
    list_push(block->transactions, (void*)transac);
}

int block_nb_transactions(Block* block){
    return list_size(block->transactions);
}

Block* create_genesis(){
    Block* block = create_block(0, NULL);
    block_add_transaction(block, "Genesis");
    sprintf(block->hash_previous, "%064d", 0); //Filling the previous hash string with zero chars
    update_merkle_hash(block);
    update_block_hash(block);
    return block;
}

void mine_block(Block* block, Queue* transac_q, int difficulty){
    int nb_transac = random_int(1, MAX_TRANSAC_BLOCK);
    if (nb_transac > queue_size(transac_q)){
        nb_transac = queue_size(transac_q);  //Concretly useless since we are never gonna mine() when there are less than MAX_TRANSAC_BLOCK transactions in the queue
    }

    for (int i = 0; i < nb_transac; i++){
        list_push(block->transactions, queue_top(transac_q));
        queue_pop(transac_q);
    }

    update_merkle_hash(block);

    //The block is ready, we now only have to calculate the hash, verify the condition, and increase the nonce then repeat if it wasn't satisfied
    update_block_hash(block);
    while (!check_hash_condition(block->hash_current, difficulty)){
        block->nonce++;
        update_block_hash(block);
    }
}

bool is_genesis(Block* block){
    return (
        block->nonce == 0 &&
        list_size(block->transactions) == 1 &&
        strcmp(list_front(block->transactions), "Genesis") == 0
    );
}

/**
 * @brief Check is a block is coherent
 * Checks the merkle hash, the hash of the block, and whether the previous hash field is the hash of the previous block
 * @param block 
 * @param previous the previous block
 * @return true if everything if coherent
 * @return false if there is at least one incoherence
 */
bool check_block_coherence(Block* block, Block* previous){
    if (previous){  //Check the previous hash only is there is a previous hash (since Genesis will be tested)
        if (strcmp(previous->hash_current, block->hash_previous)) return false;
    }

    char hash[HASH_SIZE];   //Buffer used to store the calculated hash that wil be compared to the stored hashes (merkle root, block hash)
    
    merkle(block, hash);
    if (strcmp(hash, block->hash_merkle_transactions)) return false;

    hash_block(block, hash);
    if (strcmp(hash, block->hash_current)) return false;

    return true;
}