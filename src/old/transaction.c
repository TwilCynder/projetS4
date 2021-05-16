#include "transaction.h"
#include "string.h"

#define TRANSAC_S 128

/*
typedef struct _Transaction
{
	char *emetteur;
	char *recepteur;
	long int montant;
} Transaction;
*/

void transaction_print(Transaction transaction)
{
	printf("%s", transaction);
}

Transaction *transaction_create(long int montant, char *emetteur,char *recepteur)
{
	
	Transaction *transaction = malloc();
	
	transaction->montant = montant;
	transaction->emetteur=emetteur;
	transaction->recepteur=recepteur;

	printf("Transaction aleatoire : ");
	transaction_print(transaction);
	
	return transaction;
}

Transaction *generate_random_transaction()
{
	
	int num_uti1,num_uti2;
	//char name1[7]={ };
	//char name2[7]={ };
	

	num_uti1=rand()%NB_UTILISATEURS;
	num_uti2=rand()%NB_UTILISATEURS;
	while (num_uti1==num_uti2)
	{
		num_uti2=rand()%NB_UTILISATEURS;
	}

	
	sprintf(name1,"User%d",num_uti1);
	sprintf(name2,"User%d",num_uti2);
	


	return transaction_create(rand()%MAX_TRANSACTIONS_MONTANT,name1,name2);
}



Transaction **generate_random_transaction_list( long int *transaction_nb)
{
	*transaction_nb = (rand()%(MAX_TRANSACTIONS-MIN_TRANSACTIONS))+MIN_TRANSACTIONS;
	Transaction **transaction_list = malloc((*transaction_nb)*sizeof(struct _Transaction));
	
	for (long int i=0;i<*transaction_nb; i++ )
	{
		transaction_list[i] = generate_random_transaction();	
	}
	return transaction_list;
}




void transaction_to_string(Transaction *transaction, char *string)
{
	sprintf(string, "%s envoie %ld BnB a %s",transaction->emetteur ,transaction->montant,transaction->recepteur);
}


int main(){
	srand(time(NULL));
	long int nb;
	generate_random_transaction_list(&nb);
	printf("Nb de tx = %ld\n",nb);
	return(0);
}
