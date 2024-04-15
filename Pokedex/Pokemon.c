#include "Pokemon.h"


Pokemon* pokemon(char* name, char* species, double height, double weight, int attack, PokemonType* type){
	Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
	if (pokemon == NULL)
		return NULL;
	pokemon->name = (char*)malloc(strlen(name) + 1);
	if (pokemon->name == NULL){
		free(pokemon);
		return NULL;
	}

	pokemon->species = (char*)malloc(strlen(species) + 1);
	if (pokemon->species == NULL){
		free(pokemon->name);
		free(pokemon);
		return NULL;
	}

	strcpy(pokemon->name, name);
	strcpy(pokemon->species, species);
	pokemon->bioinf = pokemonInfo(height, weight, attack);
	pokemon->type = type;
	pokemon->type->pokenum++;

	return pokemon;
}


Pokemonbioinf* pokemonInfo(double height, double weight, int attack){
	Pokemonbioinf* bioinf = (Pokemonbioinf*)malloc(sizeof(Pokemonbioinf));
	if (bioinf == NULL)
		return NULL;

	bioinf->height = height;
	bioinf->weight = weight;
	bioinf->attack = attack;

	return bioinf;
}

PokemonType* pokemonType(char* Typename){
	PokemonType* type = (PokemonType*)malloc(sizeof(PokemonType));
	if (type == NULL)
		return NULL;

	type->ptype = (char*)malloc(strlen(Typename) + 1);
	if (type->ptype == NULL){
		free(type);
		return NULL;
	}

	strcpy(type->ptype, Typename);
	type->pokenum = 0;
	type->countMe = 0;
	type->countOther = 0;
	type->effective_against_me = (PokemonType**)malloc(sizeof(PokemonType));
	if (type->effective_against_me == NULL){
		free(type->ptype);
		free(type);
		return NULL;
	}

	type->effective_against_other = (PokemonType**)malloc(sizeof(PokemonType));
	if (type->effective_against_other == NULL){
		free(type->effective_against_me);
		free(type->ptype);
		free(type);
		return NULL;
	}

	return type;
}





status addto_against_me(PokemonType** A, PokemonType* B){
	(*A)->effective_against_me = (PokemonType**)realloc((*A)->effective_against_me, sizeof(PokemonType*) * (((*A)->countMe) + 1));
	if ((*A)->effective_against_me == NULL){
		return failure;}
	(*A)->effective_against_me[(*A)->countMe] = B;
	(*A)->countMe++;
	return success;

}

status addto_against_others(PokemonType** A, PokemonType* B){
	(*A)->effective_against_other = (PokemonType**)realloc((*A)->effective_against_other, sizeof(PokemonType*) * (((*A)->countOther) + 1));
	if ((*A)->effective_against_other == NULL){
		return failure;}
	(*A)->effective_against_other[(*A)->countOther] = B;
	(*A)->countOther++;
	return success;
}


status deletefrom_against_me(PokemonType** A, char* typename) {
    int i;
    int j;
    int index = -1;
    PokemonType* tmp;

    for (i = 0; i < (*A)->countMe; i++) {
    	if (strcmp((*A)->effective_against_me[i]->ptype, typename) == 0){
            (*A)->effective_against_me[i] = NULL;
            index = i;

            for (j = index; j < (*A)->countMe - 1; j++) {
                tmp = (*A)->effective_against_me[j];
                (*A)->effective_against_me[j] = (*A)->effective_against_me[j + 1];
                (*A)->effective_against_me[j + 1] = tmp;
            }

            (*A)->countMe--;
    	}
    }

    if (index == -1) {
        return failure;
    }

    (*A)->effective_against_me = (PokemonType**)realloc((*A)->effective_against_me, sizeof(PokemonType) * (*A)->countMe);

    return success;
}



status deletefrom_against_others(PokemonType** A, char* typename) {
    int i;
	int j;
	int index = -1;
    PokemonType* tmp;

    for (i = 0; i < (*A)->countOther; i++) {
        if (strcmp((*A)->effective_against_other[i]->ptype, typename) == 0) {
        	(*A)->effective_against_other[i] = NULL;
            index = i;

            for (j = index; j < (*A)->countOther - 1; j++) {
                tmp = (*A)->effective_against_other[j];
                (*A)->effective_against_other[j] = (*A)->effective_against_other[j + 1];
                (*A)->effective_against_other[j + 1] = tmp;
            }

            (*A)->countOther--;
        }
    }

    if (index == -1) {
        return failure;
    }

    (*A)->effective_against_other = (PokemonType**)realloc((*A)->effective_against_other, (sizeof(PokemonType*) * (*A)->countOther));

    return success;
}


status print_Pokemon(Pokemon* pokemon){
	if (pokemon == NULL)
		return failure;

	printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n", pokemon->name, pokemon->species, pokemon->type->ptype, pokemon->bioinf->height, pokemon->bioinf->weight, pokemon->bioinf->attack);
	return success;
}

status print_Type(PokemonType* type){
	if(type == NULL)
		return failure;
	printf("Type %s -- %d pokemons\n", type->ptype, type->pokenum);
	if(type->countMe != 0){
		printf("\tThese types are super-effective against %s:", type->ptype);
		for(int i = 0; i < type->countMe - 1; i++){
			printf("%s ,", type->effective_against_me[i]->ptype);
		}
		printf("%s\n", type->effective_against_me[type->countMe - 1]->ptype);

	}
	if (type->countOther != 0){
			printf("\t%s moves are super-effective against:", type->ptype);
			for (int i = 0; i < type->countOther - 1; i++){
				printf("%s ,", type->effective_against_other[i]->ptype);
			}
			printf("%s\n", type->effective_against_other[type->countOther - 1]->ptype);
		}

		printf("\n");

		return success;
}
PokemonType* searchForType(PokemonType** allTypes, char* typeName, int numberOfTypes){
	int i;
	for (i = 0; i < numberOfTypes; i++){
		if (allTypes[i] == NULL)
			return NULL;

		if (strcmp(allTypes[i]->ptype, typeName) == 0)
			return allTypes[i];
	}

	return NULL;
}

Pokemon* searchForPokemon(Pokemon** allPokemons, char* pokemonName, int numberOfPokemons){
	int i;
	for (i = 0; i < numberOfPokemons; i++){
		if (allPokemons[i] == NULL)
			return NULL;

		if (strcmp(allPokemons[i]->name, pokemonName) == 0)
			return allPokemons[i];
	}

	return NULL;
}

void freePokemon(Pokemon* tmp){
	if (tmp == NULL)
		return;
	if (tmp->name != NULL)
		free(tmp->name);

	if (tmp->species != NULL)
		free(tmp->species);

	if (tmp->bioinf != NULL)
		free(tmp->bioinf);

	free(tmp);
}

void freeType(PokemonType* tmp){
	if (tmp == NULL)
		return;

	if (tmp->ptype != NULL)
		free(tmp->ptype);

	if (tmp->effective_against_me != NULL)
		free(tmp->effective_against_me);

	if (tmp->effective_against_other != NULL)
		free(tmp->effective_against_other);

	free(tmp);
}
