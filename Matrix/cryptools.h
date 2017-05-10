/*
Cryptools.h made by Vincent vs38190@gmail.com
Programme en acces libre mais priere de me demander si utilisation professionel
Program with free acces but you need to ask me before professional use
*/
#pragma once
#include <string>
#include <random>
#include <vector>
#include <cwchar>

#define elif else if
#define FOR(var,min,max) for(uint var = min;var < max;++var)
#define FOR(var,max) for(uint var = 0;var<max;++var)


typedef enum Alphabet //type permettant de choisir quel alphabet utilisé
{
	ALL, NORMAL, NUMUPPER, NUMLOWER,UPPER,LOWER,SPACEALL
}ALPHABET,*PALPHABET;
typedef enum Mode //type permettant de choisir les mode
{
	CRYPT,UNCRYPT
}MODE,*PMODE;

//differents alphabet;
std::wstring sALL(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345789é#~î{^¨$£ù%µ<>[|èç@à]},?;_'\".:/!*-+\\=()");
std::wstring sSPACEALL(sALL + L" "); //pareil que celui d'avant sauf que les espaces sont eu aussi chiffré
std::wstring sNORMAL(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
std::wstring sNUMUPPER(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
std::wstring sNUMLOWER(L"abcdefghijklmnopqrstuvwxyz0123456789");
std::wstring sUPPER(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
std::wstring sLOWER(L"abcdefghijklmnopqrstuvwxyz");

//si le caractere n'appartient pas a l'alphabet utilisé
const wchar_t unknowcar = 1;


typedef unsigned char byte;
typedef unsigned char *pbyte;
typedef unsigned int uint;
typedef unsigned int *puint;

class Cryptools
{
public:
	static inline std::wstring Vignere(std::wstring const &wstring, std::wstring const &key, ALPHABET const &alphabet = ALPHABET::ALL,MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring; //pour le ret
		std::wstring *ptr = chooseAlphabet(alphabet); //permet de faire pointé ptr vers le bonne alphabet
		size_t i = 0; //compteur
		for (auto &car : ret)
		{
			car = to(car, alphabet); // permet de mettre les lettre en min/maj selon le choix de l'alphabet
			if (testcharacter(car, ptr)) //si le caractere appartient a l'alphabet
			{
				if(mode == MODE::CRYPT) //si on crypt
					car =(*ptr)[(getpos(car, ptr) + getpos(to(key[i % key.size()], alphabet), ptr)) % ptr->size()]; //gl&hf
				else // sinon
					car = (*ptr)[(ptr->size() + (getpos(car, ptr)) - getpos(to(key[i % key.size()], alphabet), ptr)) % ptr->size()]; //gl&hf

				i++; // on incremente i
			}
			elif(car != L' ' && car != L'\n') //sinon si le caratcere n'appartient pas mais n'est pas un escpace
			{
				car = unknowcar; //on le change
				i++; // on incremente i
			}
		}
		return ret; // on retourn la wstring
	}

	//POUR LES AUTRE MEME FONCTIONEMENT MAIS IL Y A JUSTE LA LIGNE PERMETTANT D'APPLIQUER L'ALGO QUI CHANGE correspond aux gl&hf d'au dessus
	static inline std::wstring PMS(std::wstring const &wstring, std::wstring const &key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring;
		std::wstring *ptr = chooseAlphabet(alphabet);
		size_t i = 0;
		for (auto &car : ret)
		{
			car = to(car, alphabet);
			if (testcharacter(car, ptr))
			{
				if (mode == MODE::CRYPT)
					car = (*ptr)[(getpos(car, ptr) + (getpos(to(key[i % key.size()], alphabet), ptr)*(i+1))) % ptr->size()];
				else
					car = (*ptr)[( (ptr->size() * (i+1)) + (getpos(car, ptr)) - (getpos(to(key[i % key.size()], alphabet), ptr)*(i + 1))) % ptr->size()];

				i++;
			}
			elif(car != L' ' && car != L'\n')
			{
				car = unknowcar;
				i++;
			}
		}
		return ret;
	}
	static inline std::wstring Cesar(std::wstring const &wstring, size_t key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring;
		std::wstring *ptr = chooseAlphabet(alphabet);
		size_t i = 0;
		for (auto &car : ret)
		{
			car = to(car, alphabet);
			if (testcharacter(car, ptr))
			{
				if (mode == MODE::CRYPT)
					car = (*ptr)[(getpos(car, ptr) + key) % ptr->size()];
				else
					car = (*ptr)[(ptr->size() + (getpos(car, ptr)) - key ) % ptr->size()];
			}
			elif(car != L' ' && car != L'\n')
			{
				car = unknowcar;
			}
			i++;
		}
		return ret;
	}
	static inline std::wstring XOR(std::wstring const &wstring, std::wstring const &key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring;
		std::wstring *ptr = chooseAlphabet(alphabet);
		size_t i = 0;
		for (auto &car : ret)
		{
			car = to(car, alphabet);
			if (testcharacter(car, ptr))
			{
				car = (*ptr)[(getpos(car, ptr) ^ getpos(to(key[i % key.size()], alphabet), ptr)) % ptr->size()];
				i++;
			}
			elif(car != L' ' && car != L'\n')
			{
				car = unknowcar;
				i++;
			}
		}
		return ret;
	}
	static inline std::wstring VignereNumericalKey(std::wstring const &wstring, std::wstring const &numerical_key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring key = fixkey(numerical_key);
		std::wstring ret = wstring; //pour le ret
		std::wstring *ptr = chooseAlphabet(alphabet); //permet de faire pointé ptr vers le bonne alphabet
		size_t i = 0; //compteur
		for (auto &car : ret)
		{
			car = to(car, alphabet); // permet de mettre les lettre en min/maj selon le choix de l'alphabet
			if (testcharacter(car, ptr)) //si le caractere appartient a l'alphabet
			{
				if (mode == MODE::CRYPT) //si on crypt
					car = (*ptr)[(getpos(car, ptr) + (key[i % key.size()] - '0')) % ptr->size()]; //gl&hf
				else // sinon
					car = (*ptr)[(ptr->size() + (getpos(car, ptr)) - (key[i % key.size()] - '0')) % ptr->size()]; //gl&hf

				i++; // on incremente i
			}
			elif(car != L' ' && car != L'\n') //sinon si le caratcere n'appartient pas mais n'est pas un escpace
			{
				car = unknowcar; //on le change
				i++; // on incremente i
			}
		}
		return ret; // on retourn la wstring
	}
	
	//algo maison
	static inline std::wstring LinearCongruentialGenerator(std::wstring const &wstring, std::wstring const &key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring;
		std::wstring *ptr = chooseAlphabet(alphabet);
		std::vector<size_t> mixer = randomNoRep(sum(key, ptr), wstring.size());
		for (size_t i = 0; i < wstring.size(); i++)
		{
			if (mode == CRYPT)
			{
				ret[i] = wstring[mixer[i]];
			}
			else
			{
				ret[i] = wstring[search<size_t>(mixer, i)];
			}
		}
		return ret;
	}
	static inline std::wstring AtmCongruentialPMS(std::wstring const &string, std::wstring const &key, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = string;
		std::wstring *ptr = chooseAlphabet(alphabet);
		std::vector<size_t> mixer = randomNoRep(sum(key, ptr), string.size());
		for (size_t i = 0; i < string.size(); i++)
		{
			if (mode == CRYPT)
			{
				ret[i] = string[mixer[i]];
			}
			else
			{
				ret[i] = string[search<size_t>(mixer, i)];
			}
		}
		ret = Cryptools::PMS(string, key, alphabet, mode);
		return ret;
	}
	
	//test de la gestion de l'alphabet
	static inline std::wstring test(std::wstring const &wstring, ALPHABET const &alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		std::wstring ret = wstring;
		std::wstring *ptr = chooseAlphabet(alphabet);
		size_t i = 0;
		for (auto &car : ret)
		{
			car = to(car, alphabet);
			if (testcharacter(car, ptr))
			{
				if (mode == MODE::CRYPT)
					car = (*ptr)[getpos(car, ptr) % ptr->size()];
				else
					car = (*ptr)[getpos(car, ptr) % ptr->size()];

				i++;
			}
			elif(car != L' ' && car != L'\n')
			{
				car = unknowcar;
				i++;
			}
		}
		return ret;
	}

	//voir chiffrement de Bazeries sur google :)
	static inline std::wstring Bazeries(std::wstring const &wstring, std::wstring const &numerical_key, ALPHABET alphabet = ALPHABET::ALL, MODE const &mode = MODE::CRYPT)
	{
		if (alphabet == ALPHABET::SPACEALL)
			alphabet = ALPHABET::ALL;
		std::wstring key = fixkey(numerical_key);
		std::wstring *ptr = chooseAlphabet(alphabet);
		std::vector<std::wstring> wstring_reverse_vector;
		std::vector<size_t> fixed_wstring_space_pos;
		std::wstring fixed_wstring = fixwstring(wstring, &fixed_wstring_space_pos);
		if(mode == MODE::UNCRYPT)
			fixed_wstring = Cryptools::VignereNumericalKey(fixed_wstring,key, alphabet, mode);
		{
			int i = 0, gapfixer = 0;
			bool cont = true;
			while (cont)
			{
				wstring_reverse_vector.push_back(std::wstring());
				for (int j = 0; j < (key[i%key.size()] - '0'); ++j)
				{

					if ((i + j + gapfixer) < fixed_wstring.size())
					{
						fixed_wstring[i + j + gapfixer] = to(fixed_wstring[i + j + gapfixer], alphabet);
						if (testcharacter(fixed_wstring[i + j + gapfixer], ptr))
							wstring_reverse_vector[i] += fixed_wstring[i + j + gapfixer];
						else
						{
							wstring_reverse_vector[i] += unknowcar;
						}
					}
					else
					{
						cont = false;	
						break;
					}
				}
				gapfixer += (key[i%key.size()] - '0' - 1 );
				i++;
			}
		} // partie du code modifiant les grouppes de mot pour la transpostion
		if (mode == MODE::CRYPT)
			return Cryptools::VignereNumericalKey(reverse(wstring_reverse_vector, fixed_wstring_space_pos), key, alphabet, mode); //simple codage avec le chiffrement de vignere mais adapté pour une clé uniquement numerique
		else
			return reverse(wstring_reverse_vector, fixed_wstring_space_pos);
	}
	static inline size_t getSum(std::wstring const &string, ALPHABET const &alphabet)
	{
		return sum(string, chooseAlphabet(alphabet));
	}

private:
	static inline std::wstring* chooseAlphabet(ALPHABET const &alphabet)
	{
		switch (alphabet)
		{
		case ALPHABET::ALL:
			return &sALL;
			break;
		case ALPHABET::SPACEALL:
			return &sSPACEALL;
			break;
		case ALPHABET::UPPER:
			return &sUPPER;
			break;
		case ALPHABET::LOWER:
			return &sLOWER;
			break;
		case ALPHABET::NORMAL:
			return &sNORMAL;
			break;
		case ALPHABET::NUMLOWER:
			return &sNUMLOWER;
			break;
		case ALPHABET::NUMUPPER:
			return &sNUMUPPER;
			break;
		default:
			return &sALL;
			break;
		}
	}//permet de faire pointé ptr vers le bonne alphabet
	static inline wchar_t to(const wchar_t &car,ALPHABET type)
	{
		switch (type)
		{
		case ALPHABET::NUMLOWER:
			if (car <= L'0' && car >= L'9' && car >= L'A' && car <= L'Z')
				return (car - L'A') + L'a';
			else
				return car;
			break;
		case ALPHABET::NUMUPPER:
			if (car <= L'0' && car >= L'9' && car >= L'a' && car <= L'z')
				return (car - L'a') + L'A';
			else
				return car;
			break;
		case ALPHABET::LOWER:
			if(car >= L'A' && car <= L'Z')
				return (car - L'A') + L'a';
			break;
		case ALPHABET::UPPER:
			if(car >= L'a' && car <= L'z')
				return (car - L'a') + L'A';
			break;
		default:
			return car;
			break;
		return car;
		}
	}// permet de mettre les lettre en min/maj selon le choix de l'alphabet
	static inline bool testcharacter(wchar_t const &car,std::wstring const *alphabet) //test le caratctere
	{
		//on boucle dans l'alphabet et on regarde si le caractere appartient a celui ci alors on retourne true
		for (auto &str : *alphabet)
		{
			if (car == str) return true; 
		}
		return false;
	}
	static inline size_t getpos(wchar_t car,std::wstring const *alphabet) //permet de chopper la position du caractere dans l'alphabet
	{
		for (size_t i = 0; i < alphabet->size(); i++)
		{
			if (car == (*alphabet)[i]) return i;
		}
		return 0;
	}
	static inline std::wstring fixkey(std::wstring const &key)
	{
		std::wstring rep;
		bool test = false;
		for (auto &i : key)
		{
			if (i <= '9' && i >= '0')
				rep += i;
			if (i != '0')
				test = true;
		}
		if (rep == L"" || !test)
			return L"1";
		return rep;
	}
	static inline std::wstring fixwstring(std::wstring const &str, std::vector<size_t> *vector)
	{
		std::wstring rep;
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] == L' ')
			{
				vector->push_back(i);
			}
			else
			{
				rep += str[i];
			}
		}
		return rep;
	}
	static inline std::wstring reverse(std::vector<std::wstring> &vector,std::vector<size_t> space)
	{
		std::wstring rep;
		for (auto &i : vector)
		{
			rep += reverse_wstring(i);
		}
		for (auto &i : space)
		{
			rep.insert(rep.begin() + i, L' ');
		}
		return rep;
	}
	static inline std::wstring reverse_wstring(std::wstring const &str)
	{
		std::wstring rep(str);
		std::reverse(rep.begin(), rep.end());
		return rep;
	}
	static inline std::vector<size_t> randomNoRep(size_t const &seed, size_t const &max, size_t const &min = 0)
	{
		std::linear_congruential_engine<size_t, 48271, 0, 2147483647> random_engine(seed);

		std::vector<size_t> rep;
		std::vector<bool> isUsed;
		isUsed.resize(max - min, false);
		size_t temp;
		while (rep.size() < (max - min))
		{
			temp = (random_engine() % (max - min)) + min;
			bool test = true;
			if (!isUsed[temp])
			{
				isUsed[temp] = true;
				rep.push_back(temp);
			}
		}

		return rep;
	}
	static inline size_t sum(std::wstring const &str,std::wstring const *alphabet)
	{
		size_t rep = 0;
		for (auto &i : str)
		{
			rep += getpos(i, alphabet);
		}
		return rep;
	}
	template<typename T>
	static inline T search(std::vector<T> const &vector, T const &value,T ignore_count = 0)
	{
		for (T i = 0; i < vector.size(); i++)
		{
			if (vector[i] == value && ignore_count == 0)
			{
				return i;
			}
			elif(vector[i] == value)
			{
				ignore_count--;
			}
		}
		return T(0);
	}

};

class RawDataCryptools
{
public:

	static inline void XOR(pbyte const Input, pbyte Output, const uint &len, pbyte const key, const uint &keylen)
	{
		FOR(i, len)
		{
			Output[i] = Input[i%len] ^ key[i%keylen];
		}
		return;
	}



private:




};


#define RawDataXOR(input,output,len,key,keylen) RawDataCryptools::XOR((pbyte)input,(pbyte)output,(uint)len,(pbyte)key,(uint)keylen)