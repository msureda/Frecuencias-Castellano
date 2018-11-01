/*
 * calculo_frecuencias.hpp
 *
 *  Created on: 28/10/2018
 *      Author: marcelo
 */

#ifndef CALCULO_FRECUENCIAS_HPP_
#define CALCULO_FRECUENCIAS_HPP_

#include <string>

enum constantes
{
	CANTIDAD_SIMBOLOS			= 27,

	// Constantes para identificar letras con tildes
	A_MINUSCULA_ACENTO_AGUDO	= 0xffffffe1,	// á
	A_MINUSCULA_ACENTO_GRAVE	= 0xffffffe0,	// à
	A_MAYUSCULA_ACENTO_AGUDO	= 0xffffffc1,	// Á
	A_MAYUSCULA_ACENTO_GRAVE	= 0xffffffc0,	// À

	E_MINUSCULA_ACENTO_AGUDO	= 0xffffffe9,	// é
	E_MINUSCULA_ACENTO_GRAVE	= 0xffffffe8,	// è
	E_MAYUSCULA_ACENTO_AGUDO	= 0xffffffc9,	// É
	E_MAYUSCULA_ACENTO_GRAVE	= 0xffffffc8,	// È

	I_MINUSCULA_ACENTO_AGUDO	= 0xffffffed,	// í
	I_MINUSCULA_ACENTO_GRAVE	= 0xffffffec,	// ì
	I_MAYUSCULA_ACENTO_AGUDO	= 0xffffffcd,	// Í
	I_MAYUSCULA_ACENTO_GRAVE	= 0xffffffcc,	// Ì

	O_MINUSCULA_ACENTO_AGUDO	= 0xfffffff3,	// ó
	O_MINUSCULA_ACENTO_GRAVE	= 0xfffffff2,	// ò
	O_MAYUSCULA_ACENTO_AGUDO	= 0xffffffd3,	// Ó
	O_MAYUSCULA_ACENTO_GRAVE	= 0xffffffd2,	// Ò

	U_MINUSCULA_ACENTO_AGUDO	= 0xfffffffa,	// ú
	U_MINUSCULA_ACENTO_GRAVE	= 0xfffffff9,	// ù
	U_MAYUSCULA_ACENTO_AGUDO	= 0xffffffda,	// Ú
	U_MAYUSCULA_ACENTO_GRAVE	= 0xffffffd9,	// Ù

	ENIE_MINUSCULA				= 0xf1,			// ñ
	ENIE_MAYUSCULA				= 0xffffffd1	// Ñ
};


// Estructura con frecuencias de palabras
struct frecuencias_t
{
	std::string 		palabra;
	unsigned long long	frec_absoluta;
	double				frec_normalizada;
};

char alfabeto[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
					'j', 'k', 'l', 'm', 'n', ENIE_MINUSCULA, 'o', 'p', 'q',
					'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

// Declara e inicializa matriz para simbolos
unsigned long long int ocurr_sim[CANTIDAD_SIMBOLOS] = { 0ull };

// Declara e inicializa matriz para digramas
unsigned long long int ocurr_di[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

// Declara e inicializa matriz para trigramas
unsigned long long int ocurr_tri[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

// Contadores de cantidad total de simbolos, digramas y trigramas
unsigned long long int cnt_sim = 0ull, cnt_di = 0ull, cnt_tri = 0ull;

#endif /* CALCULO_FRECUENCIAS_HPP_ */
