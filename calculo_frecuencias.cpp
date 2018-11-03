//
//  calculo_frecuencias.cpp
//
//   Created on: 27/10/2018
//       Author: Marcelo Sureda
//
#include "calculo_frecuencias.hpp"

#include <cctype>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
using namespace std;

//
//  elimina_tildes
//  Traduce un string de entrada en uno
//  sin tildes y con minusculas.
//
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

//
//  elimina_comas_numero
//  Procesa el string numérico eliminando las comas
//
void elimina_comas_numero(string &str_double)
{
	size_t pos = 0;
	while ((pos = str_double.find(',', pos)) &&
			(pos < str_double.length()))
	{
		str_double.erase(pos, 1);
	}
}

//
//  carga_lista_frecuencias
//  Carga en memoria la lista de frecuencias del archivo
//  con el formato:
//  Orden	Palabra	Frec.absoluta	Frec.normalizada
//
list<Frecuencia_t> carga_lista_frecuencias(char *archivo_frecuencias)
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
	list<Frecuencia_t> lista_frecuencias;
	while (getline(ifrecuencias, ifrec_linea))
	{
		// Lee stream de archivo de entrada
		istringstream ifrec_stream(ifrec_linea);
		Frecuencia_t frecuencia;
		string ind, str_frec_absoluta, str_frec_normalizada;
		ifrec_stream >> ind >> frecuencia.palabra >>
				str_frec_absoluta >> str_frec_normalizada;
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

//
//  indice
//  Devuelve el indice del alfabeto a usar
//  en el array de simbolos.
//
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

//
//  calcula_frecuencias
//  Cuenta la cantidad de ocurrencias de simbolos,
//  digramas y trigramas
//
void calcula_frecuencias(list<Frecuencia_t> lista_frecuencias)
{
	// Recorre lista de palabras contando ocurrencias
	for (list<Frecuencia_t>::iterator f = lista_frecuencias.begin(); f != lista_frecuencias.end(); ++f)
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
	}
}

//
//  guarda_frecuencias
//  Guarda en archivo de salida cantidad de ocurrencias
//  y frecuencias obtenidas
//
void guarda_frecuencias(char *archivo_salida, list<Frecuencia_t>lista_frecuencias)
{
	// Abre Archivo
	ofstream ofrecuencias(archivo_salida, ofstream::out);
	if (ofrecuencias.fail())
	{
		cerr << "Error: Fallo abriendo " << archivo_salida << endl;
		exit(EXIT_FAILURE);
	}

	// Unigramas
	ofrecuencias << "Simbolo\tOcurrencias\tFrecuencia\n";
	for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		ofrecuencias << alfabeto[i] << "\t" <<
					ocurr_sim[i] << "\t" <<
					(double)ocurr_sim[i]/cnt_sim << "\n";
	}

	// Digramas
	ofrecuencias << "Digrama\tOcurrencias\tFrecuencia\n";
	for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			if (ocurr_di[i][j] > 0)
			{
				ofrecuencias << alfabeto[i] << alfabeto[j] << "\t" <<
							ocurr_di[i][j] << "\t" <<
							(double)ocurr_di[i][j]/cnt_di << "\n";
			}
		}
	}

	// Trigramas
	ofrecuencias << "Trigrama\tOcurrencias\tFrecuencia\n";
	for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			for(int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
			{
				if (ocurr_tri[i][j][k] > 0)
				{
					ofrecuencias << alfabeto[i] << alfabeto[j] << alfabeto[k] << "\t" <<
								ocurr_tri[i][j][k] << "\t" <<
								(double)ocurr_tri[i][j][k]/cnt_tri << "\n";
				}
			}
		}
	}
	ofrecuencias.close();
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cerr << "Error: Faltan parametros.\n\nUso:\n";
		cerr << "\t" << argv[0] << " <archivo-frecuencias> <archivo-salida>\n" << endl;
		exit(EXIT_FAILURE);
	}

	list<Frecuencia_t> lista_frecuencias = carga_lista_frecuencias(argv[1]);
	calcula_frecuencias(lista_frecuencias);
	guarda_frecuencias(argv[2], lista_frecuencias);
	return 0;
}
