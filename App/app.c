#include <stdio.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include "app.h"

extern uint32_t transSequenNumber;
void appStart(void)
{
	while(!STOPPED)
	{
		int choice;
		printf("1) Make Transaction \n2) Exit\nYour choice: ");
		fflush(stdout);
		scanf("%d", &choice);
		getchar();
		if(choice == MAKE_TRANSACTION)
		{
			// This is the card region.

			EN_cardError_t CardHolderName;
			EN_cardError_t Card_PAN;
			EN_cardError_t CardExpiryDate;

			while(1)	// Ensuring that the entered information is valid as information of Card
			{
				printf("Enter the cardholder's name: ");
				CardHolderName = getCardHolderName(&cardData);

				if(CardHolderName == WRONG_NAME)
				{
					printf("Please enter a valid name from (20 to 24 character).\n");
				}
				else
				{
					break; // Go to the next step
				}
			}

			while(1)	// Ensuring that the entered information is valid as information of Card
			{
				printf("What is the card's primary account number: ");
				Card_PAN = getCard_PAN(&cardData);

				if(Card_PAN == WRONG_PAN)
				{
					printf("Please enter a valid Card PAN (From 16 to 19 digit).\n");
				}
				else
				{
					break; // Go to the next step
				}
			}

			while(1)	// Ensuring that the entered information is valid as information of Card
			{
				printf("What is the card expiry date as form (MM/YY): ");
				CardExpiryDate = getCardExpiryDate(&cardData);

				if(CardExpiryDate == WRONG_EXP_DATE)
				{
					printf("Please enter a valid Card expiry date.\n");
				}
				else
				{
					break; // Go to the next step
				}
			}

			//	This is the terminal region.

			printf("The date of the transaction is automatically have taken.\n");
			getTransaction_Date(&termData);

			EN_terminalError_t isExpired;
			isExpired = isCardExpired(&cardData, &termData);
			if(isExpired == EXPIRED_CARD)
			{
				printf("\n--------------------------------------------------------------------------------------------------\n");
				printf("Transaction declined: Your card has expired. Please use a different card or contact your bank.\n");
				printf("----------------------------------------------------------------------------------------------------\n\n");
				continue;
			}

			EN_terminalError_t maxAmount;
			maxAmount = setMaxAmount(&termData);
			if(maxAmount == INVALID_MAX_AMOUNT)
			{	// If the Bank have set a maximum amount less than or equal Zero, an issue will be occurred
				printf("The maximum amount is invalid.\n");
				continue;
			}


			EN_terminalError_t isValidAmount;
			while(1)
			{
				printf("Enter the transaction amount: ");
				isValidAmount = getTransactionAmount(&termData);
				if(isValidAmount == INVALID_AMOUNT)
				{
					printf("Please Enter a valid amount.\n");
				}
				else
				{
					break; // Go to the next step
				}
			}

			EN_terminalError_t isBelowMax;
			isBelowMax = isBelowMaxAmount(&termData);
			if(isBelowMax == EXCEED_MAX_AMOUNT)
			{
				printf("\n------------------------------------------------------------------\n");
				printf("The transaction amount exceeds the maximum transaction amount\n");
				printf("Where the maximum amount is %.3f $\n", termData.maxTransAmount);
				printf("-------------------------------------------------------------------\n\n");
				continue;
			}

			EN_serverError_t isTransactionFound;
			isTransactionFound = getTransaction(transSequenNumber, &serverSideTransactions[transSequenNumber]);
			if(isTransactionFound == TRANSACTION_NOT_FOUND)
			{
				printf("\n------------------------------------------------------------------\n");
				printf("Error: Transaction not found. Please try again or contact customer support for assistance.\n");
				printf("--------------------------------------------------------------------\n\n");
			}
			else
			{
				switch(serverSideTransactions[transSequenNumber-1].transState)
				{
					case FRAUD_CARD:
					{
						printf("\nTransaction declined: The account entered does not exist.\n");
						printf("\n------------------------------------------------------------------\n");
						printf("The cardholder's name: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardHolderName);
						printf("The account primary number: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.primaryAccountNumber);
						printf("The card expiration date: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardExpirationDate);
						printf("--------------------------------------------------------------------\n\n");
						break;
					}
					case DECLINED_STOLEN_CARD:
					{
						printf("\n------------------------------------------------------------------\n");
						printf("Transaction declined: Stolen card reported.\n");
						printf("--------------------------------------------------------------------\n\n");
						break;
					}
					case DECLINED_INSUFFECIENT_FUND:
					{
						printf("\nTransaction declined: Insufficient funds.\n");
						printf("\n------------------------------------------------------------------\n");
						printf("The cardholder's name: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardHolderName);
						printf("The account primary number: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.primaryAccountNumber);
						printf("The card expiration date: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardExpirationDate);
						printf("Your Current balance is: %.3f\n", serverSideAccounts[(int)pointedAccount].balance);
						printf("The transaction date is: %s\n", serverSideTransactions[transSequenNumber-1].terminalData.transactionDate);
						printf("--------------------------------------------------------------------\n\n");
						break;
					}
					case APPROVED:
					{
						printf("\nTransaction approved: Succeeded Process.\n");
						printf("\n------------------------------------------------------------------\n");
						printf("The cardholder's name: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardHolderName);
						printf("The account primary number: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.primaryAccountNumber);
						printf("The card expiration date: %s\n", serverSideTransactions[transSequenNumber-1].cardHolderData.cardExpirationDate);
						printf("Your balance before the transaction is: %.3f\n", termData.transAmount + serverSideAccounts[(int)pointedAccount].balance);
						printf("Your Current balance is: %.3f\n", serverSideAccounts[(int)pointedAccount].balance);
						printf("The transaction date is: %s\n", serverSideTransactions[transSequenNumber-1].terminalData.transactionDate);
						printf("--------------------------------------------------------------------\n\n");
						break;
					}
					// Consider adding a default case to handle unexpected values
					default:
					{
						printf("\n------------------------------------------------------------------\n");
						printf("Transaction declined: Unknown reason.\n");
						printf("------------------------------------------------------------------\n\n");
						break;
					}
				}
			}

		}
		else if(choice == EXIT)
		{
			break;	// Stop the program
		}
		else
		{
			printf("Invalid choice.\n");
		}
	}
}
