#include "queue.h"
#include <stdbool.h>

typedef struct s_block Block;

void print_block(Block* block);
void merkle(Block* block, char* output);
void hash_block(Block* block, char* output);
Block* create_block(int index, Block* previous);
Block* create_genesis();
bool is_genesis(Block* block);
bool check_block_coherence(Block* block, Block* previous);

void mine_block(Block* block, Queue* transac_q, int difficulty);