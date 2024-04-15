#include "Pokemon.h"
#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static Pokemon** getPokemons(char* fileName, int size);
static PokemonType** getTypes(char* fileName, int size);
static void printallPokemons(Pokemon** arr, int size);
static void printTypes(PokemonType** arr, int size);
static status addType_to_me();
static status addType_to_others();
static status deleteType_from_me();
static status deleteType_from_others();
static status printByName();
static status printByType();
static void freeTypeArr(PokemonType** arr, int size);
static void freePokArr(Pokemon** arr, int size);
static status ExitNow();

int numberOfTypes;
int numberOfPokemons;
Pokemon** PokemonsArr;
PokemonType** TypesArr;
PokemonType* addType = NULL;




int main(int argc, char* argv[]) {
	if (argc != 4)
			exit(1);

		if (argv[1]!= NULL)
			numberOfTypes = atoi(argv[1]);

		if (argv[2] != NULL)
			numberOfPokemons = atoi(argv[2]);

		TypesArr = getTypes(argv[3], numberOfTypes);
		PokemonsArr = getPokemons(argv[3], numberOfPokemons);

    char choice;

    while (true) {
		printf("Please choose one of the following numbers:\n");
		printf("1 : Print all Pokemons\n");
		printf("2 : Print all Pokemons types\n");
		printf("3 : Add type to effective against me list\n");
		printf("4 : Add type to effective against others list\n");
		printf("5 : Remove type from effective against me list\n");
		printf("6 : Remove type from effective against others list\n");
		printf("7 : Print Pokemon by name\n");
		printf("8 : Print Pokemons by type\n");
		printf("9 : Exit\n");

        scanf(" %c", &choice);

        switch (choice) {
            case '1':
            	printallPokemons(PokemonsArr, numberOfPokemons);
                break;

            case '2':
            	printTypes(TypesArr, numberOfTypes);
                break;

            case '3':
                addType_to_me();
                break;

            case '4':
                addType_to_others();
                break;

            case '5':
                deleteType_from_me();
                break;

            case '6':
                deleteType_from_others();
                break;

            case '7':
                printByName();
                break;

            case '8':
                printByType();
                break;

            case '9':
                ExitNow();
                printf("All the memory cleaned and the program is safely closed.\n");
                return 0;

            default:
                printf("Please choose a valid number.\n");
                break;
        }
    }

    return 0;
}


PokemonType** getTypes(char* datafile, int size){
	TypesArr = (PokemonType**)malloc(sizeof(PokemonType)  * size);
	if (TypesArr == NULL){
		printf("Memory Problem.\n");
		ExitNow();
		exit(1);
	}
	FILE* file = fopen(datafile, "r");
    PokemonType* newType = NULL;

    char data[300];
    char tmp[300];
    fgets(data, 300, file);
    fgets(data, 300, file);
    strcpy(tmp, data);

    char read[5] = ",\r\n";
    char* till;
    char* stop;
    int index = 0;
    till = strtok(data, "\r\n");
    stop = strtok(tmp, ",");

    while (stop != NULL) {
        TypesArr[index] = pokemonType(stop);
        if (TypesArr[index] == NULL) {
            freeTypeArr(TypesArr, index);
            return NULL;
        }
        stop = strtok(NULL, read);
        index++;
    }

    fgets(data, 300, file);

    char start[300];
    strcpy(start, "Pokemons\r\n");

    char me[300];
    strcpy(me, "effective-against-me");

    char others[300];
    strcpy(others, "effective-against-other");

    stop = strtok(data, "\t");
    stop = strtok(stop, " ");

    while ((strcmp(data, start)) != 0) {
        addType = searchForType(TypesArr, stop, numberOfTypes);
        if (addType == NULL) {
            fgets(data, 300, file);
            stop = strtok(data, "\t");
            stop = strtok(stop, " ");
            continue;
        }
        till = strtok(NULL, ":");

        if (strcmp(till, me) == 0) {
            till = strtok(NULL, read);
            while (till != NULL) {
                newType = searchForType(TypesArr, till, index);
                if (newType == NULL) {
                    till = strtok(NULL, read);
                    continue;
                }
                status st = addto_against_me(&addType, newType);
                if (st == failure) {
                    printf("Memory Problem.\n");
                    ExitNow();
                    exit(1);
                }
                till = strtok(NULL, read);
            }
        } else if (strcmp(till, others) == 0) {
            till = strtok(NULL, read);
            while (till != NULL) {
                newType = searchForType(TypesArr, till, index);
                if (newType == NULL) {
                    till = strtok(NULL, read);
                    continue;
                }
                status st = addto_against_others(&addType, newType);
                if (st == failure) {
                    printf("Memory Problem.\n");
                    ExitNow();
                    exit(1);
                }
                till = strtok(NULL, read);
            }
        }

        fgets(data, 300, file);
        stop = strtok(data, "\t");
        stop = strtok(stop, " ");
    }

    fclose(file);

    return TypesArr;
}



static Pokemon** getPokemons(char* datafile, int size){
	Pokemon** arr;
	arr = (Pokemon**)malloc(sizeof(Pokemon*) * size);
	if (arr == NULL){
		printf("Memory Problem.\n");
		ExitNow();
		exit(1);
	}


	FILE* file = fopen(datafile, "r");
	int index = 0;
	char start[300];
	char* stop;
	char name[300];
	char species[300];
	double height;
	double weight;
	int attack;
	char read[5] = ",\r\n";
	char type[300];
	strcpy(start,"Pokemons\r\n");
	char data[300];

	do {
		fgets(data,300,file);
	}while(strcmp(data,start) != 0);

	fgets(data,300,file);
	stop = strtok(data, ",");

	while (strcmp(stop, start) != 0){
		while(stop != NULL){
			strcpy(name, stop);
			strcpy(start, stop);
			stop = strtok(NULL,",");
			strcpy(species, stop);
			stop = strtok(NULL,",");
			height = atof(stop);
			stop = strtok(NULL,",");
			weight = atof(stop);
			stop = strtok(NULL,",");
			attack = atoi(stop);
			stop = strtok(NULL, read);
			strcpy(type, stop);

			addType = searchForType(TypesArr, type, numberOfTypes);
			if (addType == NULL){
				stop = strtok(NULL, read);
				continue;
			}

			arr[index] = pokemon(name, species, height, weight, attack, addType);
			if (arr[index] == NULL){
				freePokArr(PokemonsArr,numberOfPokemons);
				return NULL;
			}
			index++;

			stop = strtok(NULL,read);

		}

		fgets(data, 300, file);

		stop = strtok(data, ",");
	}

	fclose(file);

	return arr;
}



static void printallPokemons(Pokemon** arr, int size){

	if (arr == NULL)
		return;
	int i;
	for (i = 0; i < size; i++)
		print_Pokemon(arr[i]);
}

static void printTypes(PokemonType** arr, int size){
	if (arr == NULL)
		return;
	int i;
	for (i = 0; i < size; i++)
		print_Type(arr[i]);
}



status addType_to_me() {
    char source_type_name[30];
    char new_type_name[30];
    PokemonType* source;
    PokemonType* add;

    printf("Please enter type name:\n");
    scanf("%s", source_type_name);

    // Search for the source type directly
    int source_type_index = -1;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, source_type_name) == 0) {
            source = TypesArr[i];
            source_type_index = i;
            break;
        }
    }

    if (source_type_index == -1) {
        printf("Type name doesn't exist.\n");
        return failure;
    }

    printf("Please enter type name to add to %s effective against me list:\n", source_type_name);
    scanf("%s", new_type_name);

    // Search for the type to be added directly
    int addIndex = -1;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, new_type_name) == 0) {
            add = TypesArr[i];
            addIndex = i;
            break;
        }
    }

    if (addIndex == -1) {
        printf("Type name doesn't exist.\n");
        return failure;
    }

    // Check if the type already exists in the effective against me list
    for (int i = 0; i < source->countMe; i++) {
        if (strcmp(source->effective_against_me[i]->ptype, new_type_name) == 0) {
            printf("This type already exist in the list.\n");
            return failure;
        }
    }

    // Add to the effective against me list
    status st = addto_against_me(&source, add);
    if (st == failure) {
        printf("Memory Problem.\n");
        ExitNow();
        exit(1);
    }

    print_Type(source);

    return success;
}




static status addType_to_others() {
    char source_type_name[30];
    char new_type_name[30];
    PokemonType* source;
    PokemonType* add;

    printf("Please enter type name:\n");
    scanf("%s", source_type_name);

    // Search for the source type directly
    int source_type_index = -1;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, source_type_name) == 0) {
            source = TypesArr[i];
            source_type_index = i;
            break;
        }
    }

    if (source_type_index == -1) {
        printf("Type name doesn't exist.\n");
        return failure;
    }

    printf("Please enter type name to add to %s effective against others list:\n", source_type_name);
    scanf("%s", new_type_name);

    // Search for the type to be added directly
    int addIndex = -1;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, new_type_name) == 0) {
            add = TypesArr[i];
            addIndex = i;
            break;
        }
    }

    if (addIndex == -1) {
        printf("Type name doesn't exist.\n");
        return failure;
    }

    // Check if the type already exists in the effective against others list
    for (int i = 0; i < source->countOther; i++) {
        if (strcmp(source->effective_against_other[i]->ptype, new_type_name) == 0) {
            printf("This type already exists in the list.\n");
            return failure;
        }
    }


	status st = addto_against_others(&source, add);
	if (st == failure){
		printf("Memory Problem.\n");
		ExitNow();
		exit(1);
	}
	print_Type(source);

	return success;

}


status deleteType_from_me() {
    char source_type_name[30];
    char new_type_name[30];
    PokemonType* source;
    PokemonType* del= NULL;

    printf("Please enter type name:\n");
    scanf("%s", source_type_name);
// Search for the source type directly
	int source_type_index = -1;
	for (int i = 0; i < numberOfTypes; i++) {
		if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, source_type_name) == 0) {
			source = TypesArr[i];
			source_type_index = i;
			break;
		}
	}

	if (source_type_index == -1) {
		printf("Type name doesn't exist.\n");
		return failure;
	}

    printf("Please enter type name to remove from %s effective against me list:\n", source_type_name);
    scanf("%s", new_type_name);

    // Search for the type to be added directly
    int find = 0;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, new_type_name) == 0) {
            find =1;
            del = TypesArr[i];
            break;
        }
    }

    if (find ==0){
    	printf("Type name doesn't exist in the list.\n");
    	return failure;
    }
    int i;
    	for (i = 0; i < source->countMe; i++){
    		if (strcmp(source->effective_against_me[i]->ptype, new_type_name) == 0){
    			status st = deletefrom_against_me(&source, del->ptype);
    			if (st == failure){
    				printf("Memory Problem.\n");
    				ExitNow();
    				exit(1);
    			}
    			print_Type(source);
    			return success;
    		}
    	}

    	printf("Type name doesn't exist in the list.\n");
    	return failure;
    }



status deleteType_from_others() {
    char source_type_name[30];
    char new_type_name[30];
    PokemonType* source;
    PokemonType* del = NULL;

    printf("Please enter type name:\n");
    scanf("%s", source_type_name);
// Search for the source type directly
	int source_type_index = -1;
	for (int i = 0; i < numberOfTypes; i++) {
		if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, source_type_name) == 0) {
			source = TypesArr[i];
			source_type_index = i;
			break;
		}
	}

	if (source_type_index == -1) {
		printf("Type name doesn't exist.\n");
		return failure;
	}

    printf("Please enter type name to remove from %s effective against others list:\n", source_type_name);
    scanf("%s", new_type_name);

    // Search for the type to be added directly
    int find = 0;
    for (int i = 0; i < numberOfTypes; i++) {
        if (TypesArr[i] != NULL && strcmp(TypesArr[i]->ptype, new_type_name) == 0) {
        	find =1;
        	del = TypesArr[i];
            break;
        }
    }
    if (find ==0){
		printf("Type name doesn't exist in the list.\n");
		return failure;
	}
    int i;
    	for (i = 0; i < source->countOther; i++){
    		if (strcmp(source->effective_against_other[i]->ptype, new_type_name) == 0 && del != NULL){
    			status st = deletefrom_against_others(&source, del->ptype);
    			if (st == failure){
    				printf("Memory Problem.\n");
    				ExitNow();
    				exit(1);
    			}
    			print_Type(source);
    			return success;
    		}
    	}

    	printf("Type name doesn't exist in the list.\n");
    	return failure;
    }


static status printByName() {
    char name1[30];
    Pokemon* pokemon;

    printf("Please enter Pokemon name:\n");
    scanf("%s", name1);

    // Search for the Pokemon directly
    int pokemonIndex = -1;
    for (int i = 0; i < numberOfPokemons; i++) {
        if (PokemonsArr[i] != NULL && strcmp(PokemonsArr[i]->name, name1) == 0) {
            pokemon = PokemonsArr[i];
            pokemonIndex = i;
            break;
        }
    }

    if (pokemonIndex == -1) {
        printf("The Pokemon doesn't exist.\n");
        return failure;
    }

    print_Pokemon(pokemon);

    return success;
}

static status printByType() {
    char name1[30];
    int count = 0;

    printf("Please enter type name:\n");
    scanf("%s", name1);



    if (searchForType(TypesArr,name1,numberOfTypes) == NULL) {
        printf("Type name doesn't exist.\n");
        return failure;
    }
    for (int i = 0; i < numberOfPokemons; i++) {
        if (strcmp(PokemonsArr[i]->type->ptype, name1) == 0) {
            count++;
        }
    }

    if (count > 0) {
        printf("There are %d Pokemons with this type:\n", count);
    } else {
        printf("There are no Pokemons with this type.\n");
        return success;
    }

    for (int i = 0; i < numberOfPokemons; i++) {
        if (strcmp(PokemonsArr[i]->type->ptype, name1) == 0) {
            print_Pokemon(PokemonsArr[i]);
        }
    }

    return success;
}




static void freeTypeArr(PokemonType** arr, int size){
	if (arr == NULL)
		return;

	int i;
	for (i = 0; i < size; i++)
		if(arr[i] != NULL){
			freeType(arr[i]);
			arr[i] = NULL;
		}

	free(arr);
}

static void freePokArr(Pokemon** arr, int size){
	if (arr == NULL)
		return;

	int i;
	for (i = 0; i < size; i++)
		if(arr[i] != NULL){
			freePokemon(arr[i]);
			arr[i] = NULL;
		}

	free(arr);
}

static status ExitNow(){
	freeTypeArr(TypesArr, numberOfTypes);
	freePokArr(PokemonsArr, numberOfPokemons);
	return success;
}
