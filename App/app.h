#ifndef APP_H
#define APP_H
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#define MAX_ACCOUNTS 10
#define MAX_TRANSACTIONS 10

extern ST_accountsDB_t serverSideAccounts[MAX_ACCOUNTS];
extern ST_transaction_t serverSideTransactions[MAX_TRANSACTIONS];

ST_cardData_t cardData;
ST_terminalData_t termData;
ST_accountsDB_t accountRefrence;
int8_t pointedAccount;

//uint32_t transSequenNumber = 0;

enum
{
	STOPPED,
	MAKE_TRANSACTION,
	EXIT
};

void appStart(void);


#endif
