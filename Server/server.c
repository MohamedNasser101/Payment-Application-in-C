
//#include "../App/app.h"
#include <string.h>
#include "server.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"

ST_accountsDB_t serverSideAccounts[MAX_ACCOUNTS] = {
    // Balance, State, Primary account number
    {20000.0, RUNNING, "2785015975290250"},
    {30500.0, RUNNING, "8865250866359282"},
    {2070000.0, BLOCKED, "8645904435536336"},
    {8031.0, RUNNING, "8646298960113671"},
    {4050700.0, BLOCKED, "8648771432257316"},
    {758700.0, RUNNING, "8640026078434150"},
    {250.5, RUNNING, "8649413241577316"},
    {90600407.0, BLOCKED, "8646273039575234"},
    {60000.0, RUNNING, "8646984130894833"},
    {95050.0, RUNNING, "8647949957924947"}
};

ST_transaction_t serverSideTransactions[MAX_TRANSACTIONS] = {0};

extern ST_cardData_t cardData;
extern ST_terminalData_t termData;
extern int8_t pointedAccount;
extern uint32_t transSequenNumber;

typedef char int8_t;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	EN_transState_t r;
	if(isValidAccount(cardData, serverSideAccounts[10]) == ACCOUNT_NOT_FOUND)
	{
		r = FRAUD_CARD;
	}
	else if(isBlockedAccount(&serverSideAccounts[(int)pointedAccount]) == BLOCKED_ACCOUNT)
	{
		r = DECLINED_STOLEN_CARD;
	}
	else if(isAmountAvailable(&termData) == LOW_BALANCE)
	{
		r = DECLINED_INSUFFECIENT_FUND;
	}
	else
	{
		r = APPROVED;
		serverSideAccounts[(int)pointedAccount].balance -= transData->terminalData.transAmount;

	}

	return r;
}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence)
{ // it will pass PAN card and the DB of server accounts
	EN_serverError_t r = ACCOUNT_NOT_FOUND;

	for(int i = 0; i<MAX_ACCOUNTS;i++)
	{
		if(strcmp((int8_t*)(cardData.primaryAccountNumber), (int8_t*)(serverSideAccounts[i].primaryAccountNumber)) == 0)
		{
			r = SERVER_OK;
			pointedAccount = i;
		}
	}

	return r;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t r = SERVER_OK;
	if(accountRefrence->state == BLOCKED)
	{
		r = BLOCKED_ACCOUNT;
	}

	return r;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
	EN_serverError_t r = SERVER_OK;
	if(termData->transAmount > serverSideAccounts[(int)pointedAccount].balance)
	{
		r = LOW_BALANCE;
	}

	return r;
}


EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	EN_serverError_t r = APPROVED;
	strcpy((int8_t*)transData->cardHolderData.cardHolderName, (int8_t*)cardData.cardHolderName);
	strcpy((int8_t*)transData->cardHolderData.primaryAccountNumber, (int8_t*)cardData.primaryAccountNumber);
	strcpy((int8_t*)transData->cardHolderData.cardExpirationDate, (int8_t*)cardData.cardExpirationDate);
	transData->terminalData.transAmount = termData.transAmount;
	transData->terminalData.maxTransAmount = termData.maxTransAmount;
	strcpy((int8_t*)transData->terminalData.transactionDate, (int8_t*)termData.transactionDate);
	transData->transState = recieveTransactionData(transData); // continue
	transSequenNumber++;
	transData[transSequenNumber].transactionSequenceNumber = (transData[transSequenNumber-1].transactionSequenceNumber)++;

	return r;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
	EN_serverError_t r = SERVER_OK;
	if(transactionSequenceNumber == MAX_TRANSACTIONS)
	{
		r = TRANSACTION_NOT_FOUND;
	}
	else
	{
		r = saveTransaction(transData);
	}

	return r;
}
