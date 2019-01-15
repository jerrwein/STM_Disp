/******************************************************************************
* text.c
*
* Module for handling text display messages
*
* Copyright (c) 2013 Nilfisk-Advance, Inc. All rights reserved.
*
* Version History
*------------------------------------------------------------------------------
* 0.01, 25Sep2013, CParker
*	- file created
*
******************************************************************************/
#include "stdint.h"
#include "stdbool.h"

#include "system.h"
#include "text.h"


/*
*	Defines
**********************************************************/

/*
*	Variables
**********************************************************/
static const char **m_textList = gk_textListEnglish;

/*
*	Private Function Prototypes
**********************************************************/
//none


/******************************************************************************
* Text_SetLanguage
*	Set the desired language.
******************************************************************************/
void Text_SetLanguage (text_language_t language)
{
	if (language == TEXT_LANGUAGE_SPANISH)
	{
		m_textList = gk_textListSpanish;
	}
	else if (language == TEXT_LANGUAGE_FRENCH)
	{
		m_textList = gk_textListFrench;
	}
	else if (language == TEXT_LANGUAGE_PORTUGUESE)
	{
		m_textList = gk_textListPortuguese;
	}
	else if (language == TEXT_LANGUAGE_GERMAN)
	{
		m_textList = gk_textListGerman;
	}
	else if (language == TEXT_LANGUAGE_ITALIAN)
	{
		m_textList = gk_textListItalian;
	}
	else
	{
		m_textList = gk_textListEnglish;
	}
}

/******************************************************************************
* Text_Get
*	Return a text string from the list of whichever language is set in
*	configuration settings.
******************************************************************************/
inline const char* Text_Get (text_id_t id)
{
	return (m_textList[id]);
}
