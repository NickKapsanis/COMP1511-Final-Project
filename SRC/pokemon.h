// Assignment 2 21T2 COMP1511: Pokedex
// pokemon.h
//
// You must not change this file.
//
// Version 1.0.0: Assignment released.

#include "pokemon_types.h"

#ifndef _POKEMON_H_
#define _POKEMON_H_

#define DOES_NOT_EVOLVE (-42)

typedef struct pokemon *Pokemon;

/*
 * Create a new Pokemon, given its pokemon_id, name, height, weight, and
 * type.
 *
 * The pokemon_id must be between 0 and 999 inclusive.
 *
 * The name must consist only of uppercase letters, lowercase letters,
 * and hyphens ('-').
 *
 * If the name provided contains any invalid characters, the function
 * should print an appropriate error message and exit the program.
 *
 * The two types provided must be integers representing the type->int
 * mapping in pokemon_types.h.
 *
 * If the Pokemon only has one type, type2 should be NONE_TYPE.
 *
 * Weight is given in kilograms, and height is given in metres.
 *
 * Example: new_pokemon(0, "Pikachu", 12, 2.5, ELECTRIC_TYPE, NONE_TYPE)
 */
Pokemon new_pokemon(int pokemon_id, char *name, double height,
    double weight, pokemon_type type1, pokemon_type type2);

/*
 * Create a clone of the given Pokemon.
 *
 * The cloned Pokemon should have all of the same attributes as the
 * original Pokemon (e.g. same pokemon_id, same name, ...).
 *
 * The cloned Pokemon should be entirely separate from the original
 * Pokemon, such that later deleting the original Pokemon will not
 * affect the cloned Pokemon.
 */
Pokemon clone_pokemon(Pokemon pokemon);

/*
 * Return the pokemon_id of a given Pokemon.
 */
int pokemon_id(Pokemon pokemon);

/*
 * Return the name of a given Pokemon.
 * This should not be freed by the caller.
 */
char *pokemon_name(Pokemon pokemon);

/*
 * Return the height of a given Pokemon.
 */
double pokemon_height(Pokemon pokemon);

/*
 * Return the weight of a given Pokemon.
 */
double pokemon_weight(Pokemon pokemon);

/*
 * Return the first type of a given Pokemon.
 */
pokemon_type pokemon_first_type(Pokemon pokemon);

/*
 * Return the second type of a given Pokemon.
 * If the Pokemon only has one type, returns NONE_TYPE.
 */
pokemon_type pokemon_second_type(Pokemon pokemon);

/*
 * Free the memory associated with a given Pokemon.
 */
void destroy_pokemon(Pokemon pokemon);

#endif // _POKEMON_H_
