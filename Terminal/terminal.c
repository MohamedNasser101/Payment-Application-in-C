#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Card/card.h"
#include "terminal.h"



EN_terminalError_t getTransaction_Date(ST_terminalData_t *termData)
{
    // __DATE__ >> "MMM DD YYYY"
    // termData->transactionDate[0] >> "DD/MM/YYYY" Required
   	if(__DATE__[4] == ' ')
	{
		termData->transactionDate[0] = '0';
	}
	else
    {
        termData->transactionDate[0] = __DATE__[4];
    }
	termData->transactionDate[1] = __DATE__[5];
	termData->transactionDate[2] = '/';
    termData->transactionDate[5] = '/';
	termData->transactionDate[6] = __DATE__[7];
	termData->transactionDate[7] = __DATE__[8];
	termData->transactionDate[8] = __DATE__[9];
	termData->transactionDate[9] = __DATE__[10];
	termData->transactionDate[10] = '\0';
	switch(__DATE__[0])
	{
		case 'J':
		{
			switch(__DATE__[1])
			{
				case 'a':
				{
					termData->transactionDate[3] = '0';
					termData->transactionDate[4] = '1';
					break;
				}
				case 'u':
				{
					switch(__DATE__[2])
					{
						case 'n':
						{
							termData->transactionDate[3] = '0';
							termData->transactionDate[4] = '6';
							break;
						}
						case 'l':
						{
							termData->transactionDate[3] = '0';
							termData->transactionDate[4] = '7';
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 'F':
		{
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '2';
			break;
		}
		case 'M':
		{
			switch(__DATE__[2])
			{
				case 'r':
				{
					termData->transactionDate[3] = '0';
					termData->transactionDate[4] = '3';
					break;
				}
				case 'y':
				{
					termData->transactionDate[3] = '0';
					termData->transactionDate[4] = '5';
					break;
				}
			}
			break;
		}
		case 'A':
		{
			switch(__DATE__[1])
			{
				case 'p':
				{
					termData->transactionDate[3] = '0';
					termData->transactionDate[4] = '4';
					break;
				}
				case 'u':
				{
					termData->transactionDate[3] = '0';
					termData->transactionDate[4] = '8';
					break;
				}
			}
			break;
		}
		case 'S':
		{
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '9';
			break;
		}
		case 'O':
		{
			termData->transactionDate[3] = '1';
			termData->transactionDate[4] = '0';
			break;
		}
		case 'N':
		{
			termData->transactionDate[3] = '1';
			termData->transactionDate[4] = '1';
			break;
		}
		case 'D':
		{
			termData->transactionDate[3] = '1';
			termData->transactionDate[4] = '2';
			break;
		}
	}

	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	// Comparing the card expiry date which exist in cardData structure "MM/YY"
	//with the transaction date which is taken automatically in structure terminalData"DD/MM/YYYY"
	EN_terminalError_t r = TERMINAL_OK;
	int currentYear, expiryYear;
	int currentMonth, expiryMonth;
	currentYear = atoi(termData->transactionDate+8);
	currentMonth = atoi(termData->transactionDate+3);

	expiryYear = atoi(cardData->cardExpirationDate+3);
	expiryMonth = atoi(cardData->cardExpirationDate);

	//printf("%d\n", expiryYear);
	//printf("%d\n", expiryMonth);
	//printf("%d\n", currentYear);
	//printf("%d\n", currentMonth);

	if(currentYear > expiryYear)
	{
		r = EXPIRED_CARD;
	}
	else if(currentYear == expiryYear)
	{
		if(currentMonth > expiryMonth)
		{
			r = EXPIRED_CARD;
		}
	}
	else
	{
		r = TERMINAL_OK;
	}


	return r;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
	EN_terminalError_t r = CARD_OK;
	uint8_t length = strlen((int8_t*)cardData->primaryAccountNumber);

	uint8_t checksum = 0, oddIndex = 0, evenIndex = 0;
	for(uint8_t i = length-1; i>=0; i--)
	{
		if(i%2)
		{
			if(2*(cardData->primaryAccountNumber[i] - 48) > 9)
			{
				oddIndex += 2*(cardData->primaryAccountNumber[i] - 48) - 9;
			}
			else
			{
				oddIndex += 2*(cardData->primaryAccountNumber[i] - 48);
			}
		}
		else
		{
			evenIndex += (cardData->primaryAccountNumber[i] - 48);
		}
	}
	checksum = oddIndex+evenIndex;
	if((checksum%10))
	{
		r = INVALID_CARD;
	}

	return r;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	EN_terminalError_t r = TERMINAL_OK;
	scanf("%f", &termData->transAmount);
	fflush(stdin);
	if(termData->transAmount <= 0)
	{
		r = INVALID_AMOUNT;
	}

	return r;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	EN_terminalError_t r = TERMINAL_OK;
	if(termData->transAmount > termData->maxTransAmount)
	{
		r = EXCEED_MAX_AMOUNT;
	}

	return r;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
	EN_terminalError_t r = TERMINAL_OK;
	termData->maxTransAmount = 10000.0;
	// If the Bank have set a maximum amount less than or equal Zero, an issue will be occurred
	if(termData->maxTransAmount <= 0)
	{
		r = INVALID_MAX_AMOUNT;
	}

	return r;
}


