
#ifndef POKEMON_H_
#define POKEMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"



typedef struct PokemonTypeP {
    char* ptype;
    int pokenum;
    int countMe;
    int countOther;
    struct PokemonTypeP** effective_against_me;
    struct PokemonTypeP** effective_against_other;
}PokemonType;




typedef struct Pokemonbio{
	double height;
	double weight;
	int attack;
}Pokemonbioinf;

typedef struct Pokemonnew{
	char* name;
	char* species;
	PokemonType* type;
	Pokemonbioinf* bioinf;
}Pokemon;





/*##################################################################################################################*/




Pokemon* pokemon(char* name, char* species, double height, double weight, int attack, PokemonType* type);
Pokemonbioinf* pokemonInfo(double height, double weight, int attack);
PokemonType* pokemonType(char* name);
status addto_against_me(PokemonType** A, PokemonType* B);
status addto_against_others(PokemonType** A, PokemonType* B);
status deletefrom_against_me(PokemonType** A, char* ntype);
status deletefrom_against_others(PokemonType** A, char* ntype);
status print_Pokemon(Pokemon* pokemon);
status print_Type(PokemonType* type);
void freePokemon(Pokemon* tmp);
void freeType(PokemonType* tmp);
PokemonType* searchForType(PokemonType** allTypes, char* typeName, int numberOfTypes);
Pokemon* searchForPokemon(Pokemon** allPokemons, char* pokemonName, int numberOfPokemons);


#endif /* POKEMON_H_ */


