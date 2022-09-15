// Assignment 2 21T2 COMP1511: Pokedex
// pokemon.c
//
// You must not change this file.
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: All indications to edit this file have been removed.

#include <stdio.h>
#include <stdlib.h>

#include "pokemon.h"

struct pokemon {
    int pokemon_id;
    char* name;
    double height;
    double weight;
    pokemon_type type1;
    pokemon_type type2;
};

static char *name_dup(char *s);
static int name_len(char* s);

Pokemon new_pokemon(int pokemon_id, char *name, double height,
    double weight, pokemon_type type1, pokemon_type type2) {
    if (pokemon_id < 0 || pokemon_id > 999) {
        printf("Invalid Pokemon ID\n");
        exit(1);
    }
    if (type1 == NONE_TYPE) {
        printf("Invalid Pokemon Types\n");
        exit(1);
    }
    Pokemon new_pkmn = malloc(sizeof(struct pokemon));
    new_pkmn->pokemon_id = pokemon_id;
    new_pkmn->name =  name_dup(name);
    new_pkmn->height = height;
    new_pkmn->weight = weight;
    new_pkmn->type1 = type1;
    new_pkmn->type2 = type2;
    return new_pkmn;
}

int pokemon_id(Pokemon pokemon) {
    return pokemon->pokemon_id;
}

char *pokemon_name(Pokemon pokemon) {
    return pokemon->name;
}

double pokemon_height(Pokemon pokemon) {
    return pokemon->height;
}

double pokemon_weight(Pokemon pokemon) {
    return pokemon->weight;
}

pokemon_type pokemon_first_type(Pokemon pokemon) {
    return pokemon->type1;
}

pokemon_type pokemon_second_type(Pokemon pokemon) {
    return pokemon->type2;
}

Pokemon clone_pokemon(Pokemon pokemon) {
    return new_pokemon(
        pokemon->pokemon_id,
        pokemon->name,
        pokemon->height,
        pokemon->weight,
        pokemon->type1,
        pokemon->type2
    );
}

void destroy_pokemon(Pokemon pokemon) {
    free(pokemon->name);
    free(pokemon);
}

static int name_len(char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

static char *name_dup(char* s) {
    int len = name_len(s)+1;
    // +1 for null byte
    char* new = calloc(len,sizeof(char));
    int i = 0;
    while(i < len) {
        new[i] = s[i];
        i++;
    }
    return new;
}
