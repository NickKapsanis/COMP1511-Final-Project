// Assignment 2 21T2 COMP1511: Pokedex
// pokemon_types.c
//
// You must not change this file.
//
// Version 1.0.0: Assignment released.

#include <string.h>
#include <ctype.h>

#include "pokemon_types.h"

const static char *types[] = {
    [NONE_TYPE]     = "None",
    [NORMAL_TYPE]   = "Normal",
    [FIRE_TYPE]     = "Fire",
    [FIGHTING_TYPE] = "Fighting",
    [WATER_TYPE]    = "Water",
    [FLYING_TYPE]   = "Flying",
    [GRASS_TYPE]    = "Grass",
    [POISON_TYPE]   = "Poison",
    [ELECTRIC_TYPE] = "Electric",
    [GROUND_TYPE]   = "Ground",
    [PSYCHIC_TYPE]  = "Psychic",
    [ROCK_TYPE]     = "Rock",
    [ICE_TYPE]      = "Ice",
    [BUG_TYPE]      = "Bug",
    [DRAGON_TYPE]   = "Dragon",
    [GHOST_TYPE]    = "Ghost",
    [DARK_TYPE]     = "Dark",
    [STEEL_TYPE]    = "Steel",
    [FAIRY_TYPE]    = "Fairy",
};

pokemon_type type_str_to_code(char *str) {
    int i = 0;
    while (i < sizeof(types) / sizeof(types[0])) {
        if (strcasecmp(types[i], str) == 0) {
            return (pokemon_type) i;
        }
        i++;
    }
    return NONE_TYPE;
}

const char *type_code_to_str(pokemon_type type) {
    return types[type];
}
