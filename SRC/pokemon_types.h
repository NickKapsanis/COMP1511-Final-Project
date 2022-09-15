// Assignment 2 21T2 COMP1511: Pokedex
// pokemon_types.h
//
// You must not change this file.
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Free not required.

#ifndef _POKEMON_TYPES_H_
#define _POKEMON_TYPES_H_

typedef enum pokemon_type {
    NONE_TYPE,
    NORMAL_TYPE,
    FIRE_TYPE,
    FIGHTING_TYPE,
    WATER_TYPE,
    FLYING_TYPE,
    GRASS_TYPE,
    POISON_TYPE,
    ELECTRIC_TYPE,
    GROUND_TYPE,
    PSYCHIC_TYPE,
    ROCK_TYPE,
    ICE_TYPE,
    BUG_TYPE,
    DRAGON_TYPE,
    GHOST_TYPE,
    DARK_TYPE,
    STEEL_TYPE,
    FAIRY_TYPE
} pokemon_type;

/*
 * Convert a string representation of a Pokemon type into its
 * corresponding type code, as defined in pokemon_types.h (this file).
 */
pokemon_type type_str_to_code(char *str);

/*
 * Convert a Pokemon type code into its string representation.
 *
 * NOTE: The string returned does not need to be freed by the caller.
 *       A previous version of this file stated otherwise.
 */
const char *type_code_to_str(pokemon_type type);

#endif // _POKEMON_TYPES_H
