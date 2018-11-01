/*
 * calculo_frecuencias.cpp
 *
 *  Created on: 27/10/2018
 *      Author: Marcelo Sureda
 */
#include "calculo_frecuencias.hpp"

#include <cctype>
#include <cstdlib>
#include <clocale>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
using namespace std;

/*
 * elimina_tildes
 * Traduce un string de entrada en uno
 * sin tildes y con minusculas.
 */
void procesa_palabra(string &entrada)
{
	for (string::iterator it=entrada.begin(); it!=entrada.end(); ++it)
	{
		switch (*it)
		{
			case A_MINUSCULA_ACENTO_AGUDO:
			case A_MINUSCULA_ACENTO_GRAVE:
			case A_MAYUSCULA_ACENTO_AGUDO:
			case A_MAYUSCULA_ACENTO_GRAVE:
				*it = 'a';
				break;

			case E_MINUSCULA_ACENTO_AGUDO:
			case E_MINUSCULA_ACENTO_GRAVE:
			case E_MAYUSCULA_ACENTO_AGUDO:
			case E_MAYUSCULA_ACENTO_GRAVE:
				*it = 'e';
				break;

			case I_MINUSCULA_ACENTO_AGUDO:
			case I_MINUSCULA_ACENTO_GRAVE:
			case I_MAYUSCULA_ACENTO_AGUDO:
			case I_MAYUSCULA_ACENTO_GRAVE:
				*it = 'i';
				break;

			case O_MINUSCULA_ACENTO_AGUDO:
			case O_MINUSCULA_ACENTO_GRAVE:
			case O_MAYUSCULA_ACENTO_AGUDO:
			case O_MAYUSCULA_ACENTO_GRAVE:
				*it = 'o';
				break;

			case U_MINUSCULA_ACENTO_AGUDO:
			case U_MINUSCULA_ACENTO_GRAVE:
			case U_MAYUSCULA_ACENTO_AGUDO:
			case U_MAYUSCULA_ACENTO_GRAVE:
				*it = 'u';
				break;

			case ENIE_MAYUSCULA:
				*it = ENIE_MINUSCULA;
				break;

			default:
				*it = tolower(*it);
				break;
		}
	}
}

void elimina_comas_numero(string &str_double)
{
	size_t pos = 0;
	while ((pos = str_double.find(',', pos)) &&
			(pos < str_double.length()))
	{
		str_double.erase(pos, 1);
	}
}


/*
 * carga_lista_frecuencias
 * Carga en memoria la lista de frecuencias del archivo
 * con el formato:
 * Orden	Palabra	Frec.absoluta	Frec.normalizada
 */
list<frecuencias_t> carga_lista_frecuencias(char *archivo_frecuencias)
{
	// Abre Archivo
	ifstream ifrecuencias(archivo_frecuencias, ifstream::in);
	if (ifrecuencias.fail())
	{
		cerr << "Error: Fallo abriendo " << archivo_frecuencias << endl;
		exit(EXIT_FAILURE);
	}

	string ifrec_linea;
	// Lee 1ra linea de titulo
	getline(ifrecuencias, ifrec_linea);

	// Carga frecuencias a lista en memoria
	list<frecuencias_t> lista_frecuencias;
	while (getline(ifrecuencias, ifrec_linea))
	{
		// Lee stream de archivo de entrada
		istringstream ifrec_stream(ifrec_linea);
		frecuencias_t frecuencia;
		string ind, str_frec_absoluta, str_frec_normalizada;
		ifrec_stream >> ind >> frecuencia.palabra >> str_frec_absoluta >> str_frec_normalizada;
		// Procesa palabra entrada
		procesa_palabra(frecuencia.palabra);
		elimina_comas_numero(str_frec_absoluta);
		// Convierte datos a tipo numerico
		frecuencia.frec_absoluta = strtoull(str_frec_absoluta.c_str(), NULL, 10);
		frecuencia.frec_normalizada = strtold(str_frec_normalizada.c_str(), NULL);
		// Agrega a lista de frecuencias
		lista_frecuencias.push_back(frecuencia);
	}

	if (!ifrecuencias.eof())
	{
		cerr << "Error: Leyendo archivo " << archivo_frecuencias << endl;
		ifrecuencias.close();
		exit(EXIT_FAILURE);
	}

	ifrecuencias.close();
	return lista_frecuencias;
}


/*
 * indice
 * Devuelve el indice del alfabeto a usar
 * en el array de simbolos.
 */
int indice(unsigned char c)
{
	if (c >= 'a' && c <= 'n')
	{
		return (int)(c - 'a');
	}
	else if (c == ENIE_MINUSCULA)
	{
		return (int)('n' - 'a' + 1);
	}
	else if (c >= 'o' && c <= 'z')
	{
		return (int)(c - 'a' + 1);
	}
	else
	{
		return -1;
	}
}

/*
 * calcula_frecuencias
 * Cuenta la cantidad de ocurrencias de simbolos,
 * digramas y trigramas
 */
void calcula_frecuencias(list<frecuencias_t> lista_frecuencias)
{
	// Recorre lista de palabras contando ocurrencias
	unsigned long long int cnt_sim = 0ull, cnt_di = 0ull, cnt_tri = 0ull;
	//long double cnt_sim = 0, cnt_di = 0, cnt_tri = 0;
	for (list<frecuencias_t>::iterator f = lista_frecuencias.begin(); f != lista_frecuencias.end(); ++f)
	{
		int ind_0 = -1, ind_1 = -1, ind_2 = -1;
		for (int i = 0; i < f->palabra.length(); ++i)
		{
			ind_2 = i > 1 ? ind_1 : -1;
			ind_1 = i > 0 ? ind_0 : -1;
			ind_0 = indice(f->palabra[i]);

			// Simbolos
			if (ind_0 != -1)
			{
				ocurr_sim[ind_0] += f->frec_absoluta;
				cnt_sim += f->frec_absoluta;
			}

			// Digramas
			if (ind_0 != -1 && ind_1 != -1)
			{
				ocurr_di[ind_1][ind_0] += f->frec_absoluta;
				cnt_di += f->frec_absoluta;
			}

			// Trigramas
			if (ind_0 != -1 && ind_1 != -1 && ind_2 != -1)
			{
				ocurr_tri[ind_2][ind_1][ind_0] += f->frec_absoluta;
				cnt_tri += f->frec_absoluta;
			}
		}
//		cout << "Palabra: " << f->palabra << "\t" << "cnt_sim=" << cnt_sim << endl;
	}

	cout << "Simbolo\tOcurrencias" << endl;
	for(int i = 0; i < CANTIDAD_SIMBOLOS; i++)
	{
		cout << alfabeto[i] << "\t" << ocurr_sim[i] << endl;
	}
	cout << "Simbolo\tFrecuencia" << endl;
	for(int i = 0; i < CANTIDAD_SIMBOLOS; i++)
	{
		cout << alfabeto[i] << "\t" << (double)ocurr_sim[i]/cnt_sim << endl;
	}

}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << "Error: Falta nombre de archivo de entrada." << endl;
		exit(EXIT_FAILURE);
	}

	ios_base::sync_with_stdio(false);
	cout.imbue(locale("es_UY.UTF-8"));
	list<frecuencias_t> lista_frecuencias = carga_lista_frecuencias(argv[1]);
	for (list<frecuencias_t>::iterator it=lista_frecuencias.begin(); it != lista_frecuencias.end(); ++it)
	{
		cout << "Palabra: " << it->palabra << endl <<
				"Frecuencia absoluta: " << it->frec_absoluta << endl <<
				"Frecuencia normalizada: " << it->frec_normalizada << endl;
	}

	calcula_frecuencias(lista_frecuencias);
	return 0;
}
