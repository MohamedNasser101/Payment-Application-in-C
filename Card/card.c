#include <stdio.h>
#include <string.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	gets(cardData->cardHolderName);
	fflush(stdin);
	EN_cardError_t r = CARD_OK;
	uint8_t length = strlen(cardData->cardHolderName);
	if(cardData->cardHolderName[0] == '\0')
	{
		r = WRONG_NAME;
	}
	if(length>24 ^ length<20)
	{
		r = WRONG_NAME;
	}

	return r;
}


EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	gets(cardData->cardExpirationDate);
	fflush(stdin);
	EN_cardError_t r = CARD_OK;
	uint8_t length = strlen(cardData->cardExpirationDate);
	for(uint8_t i = 0; i<5; i++)
	{
		if(i == 2)
		{
			if(cardData->cardExpirationDate[2] != '/')
			{
				r = WRONG_EXP_DATE;
			}
			continue;
		}
		if(cardData->cardExpirationDate[i] < '0' && cardData->cardExpirationDate[i] > '9')
		{
			r = WRONG_EXP_DATE;
		}
		if(cardData->cardExpirationDate[0] == '\0' && length!=5)
		{
			r = WRONG_EXP_DATE;
		}
	}


	return r;
}


EN_cardError_t getCard_PAN(ST_cardData_t *cardData)
{
	gets(cardData->primaryAccountNumber);
	fflush(stdin);
	EN_cardError_t r = CARD_OK;
	uint8_t length = strlen(cardData->primaryAccountNumber);
	if(cardData->primaryAccountNumber[0] == '\0' || length <16 || length>19)
	{
		r = WRONG_PAN;
	}

	return r;
}
