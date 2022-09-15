// Assignment 2 21T2 COMP1511: Pokedex
// pokedex.h
//
// You must not change this file.
//
// Version 1.0.0: Assignment released.

#include "pokemon.h"

#ifndef _POKEDEX_H_
#define _POKEDEX_H_

typedef struct pokedex *Pokedex;

/*
 * Create a new Pokedex and return a pointer to it.
 * The pointer is to a malloced piece of memory, and it is the caller's
 * responsibility to call 'destroy_pokedex' to free that memory.
 */
Pokedex new_pokedex(void);

/*
 * Add a new Pokemon to the end of the Pokedex.
 * By default, the Pokemon is set as "found", but this doesn't need to be 
 * implemented until Stage 2.
 *
 * When the first Pokemon is added to the Pokédex, the currently
 * selected Pokemon becomes this Pokemon.
 *
 * The currently selected Pokemon remains the first Pokemon that was
 * inserted into the Pokedex, until the `jump_pokemon`, `next_pokemon`,
 * or `prev_pokemon` functions are called.
 *
 * Assume that no two pokemon of the same ID will ever be input into the pokedex. 
 * You therefore do not need to handle this case.
 *
 * Assume that the pokemon provided is valid.
 */
void add_pokemon(Pokedex pokedex, Pokemon pokemon);

/*
 * Print out all the Pokemon in the Pokedex in the form:
 *
 * --> #001: Bulbasaur
 *     #004: **********
 *     #007: ********
 *     #010: Caterpie
 *     #013: Weedle
 *     #016: Pidgey
 *
 * An arrow should be displayed next to the currently selected Pokemon.
 *
 * Starting from Stage 2, Any Pokemon not yet 'found' should have its name replaced with
 * asterisks. The number of asterisks to be printed should be equal to the 
 * number of characters in the pokemon's name. This is not necessary for stage 1
 *
 * The pokemon_id should be displayed with leading '0's such that all of
 * the numbers align.
 *
 * The number of digits to be displayed is 3. e.g. if only 1 pokemon exists on the Pokedex
 * with pokemon_id 1, the ID should still have leading zeroes added such that they are
 * 3 digits long (i.e #001). 
 *
 * You can assume that no pokemon with ID > 999 will be added to the pokedex. i.e. Always print
 * IDs with 3 digits.
 *
 * For example:
 *
 * --> #001: Bulbasaur
 *     #004: **********
 *     #007: ********
 *     #000: Caterpie
 *     #003: Weedle
 *     #010: Jabberwocky
 *
 * The Pokemon should be printed in the order that they were inserted
 * into the Pokedex.
 */
void print_pokedex(Pokedex pokedex);

/*
 * Print out the details of currently selected Pokemon in the form:
 *   ID: 007
 *   Name: Squirtle
 *   Height: 0.510000 m
 *   Weight: 9.000000 kg
 *   Type: Water
 *
 * The pokemon_id should be displayed with leading '0's such that all of
 * the numbers align.
 *
 * The number of digits to be displayed is 3. e.g. if only 1 pokemon exists on the Pokedex
 * with pokemon_id 1, the ID should still have leading zeroes added such that they are
 * 3 digits long (i.e #001). 
 *
 * You can assume that no pokemon with ID > 999 will be added to the pokedex. i.e. Always print
 * IDs with 3 digits.
 *
 * Starting from Stage 2, if the Pokemon has not been found, the output should be:
 *   ID: 007
 *   Name: ********
 *   Height: --
 *   Weight: --
 *   Type: --
 *
 * The number of asterisks to be printed should be equal to the 
 * number of characters in the pokemon's name.
 * The number of dashes to be printed after `Height`, `Weight` and `Type`
 * is always 2.
 *
 * If the Pokemon has multiple types, they should be separated by a space:
 *   ID: 006
 *   Name: Charizard
 *   Height: 1.700000 m
 *   Weight: 90.500000 kg
 *   Type: Fire Flying
 *
 * If the Pokedex is empty, this function should do nothing.
 */
void detail_current_pokemon(Pokedex pokedex);

/*
 * Get the currently selected Pokemon.
 *
 * If the Pokedex is empty, this function should return NULL.
 */
Pokemon get_current_pokemon(Pokedex pokedex);

/*
 * Change the currently selected Pokemon to be the next in the Pokedex.
 *
 * If there is no next Pokemon, the current Pokemon is unchanged.
 *
 * If there are no Pokemon in the Pokedex, this function does nothing.
 */
 void next_pokemon(Pokedex pokedex);

/*
 * Change the currently selected Pokemon to be the previous in the
 * Pokedex.
 *
 * If there is no previous Pokemon, the current Pokemon is unchanged.
 *
 * If there are no Pokemon in the Pokedex, this function does nothing.
 */
void prev_pokemon(Pokedex pokedex);

/*
 * "Jump" the currently selected Pokemon to be the Pokemon with the
 * ID `pokemon_id`.
 *
 * If there is no Pokemon with the ID `pokemon_id`, this function prints:
 * "Could not find a pokemon by that ID".
 */
void jump_pokemon(Pokedex pokedex, int pokemon_id);

/*
 * Sets the currently selected pokemon's "found" status.
 * 
 * Assume that `found` will only ever be 1 (True) or 0 (false)
 *
 * If there is no currently selected pokemon, this function does nothing.
 */
void set_current_pokemon_found(Pokedex pokedex, int found);

/*
 * Inserts a new pokemon before the currently selected pokemon in pokedex.
 *
 * The newly inserted pokemon should be set as NOT found, and as 
 * the current pokemon in the Pokedex.
 *
 * If the pokedex is empty, you should insert the pokemon as the first element 
 * in the pokedex.
 *
 * Assume that no two pokemon of the same ID will ever be input into the pokedex. 
 * You therefore do not need to handle this case.
 *
 * Assume that the pokemon provided is valid.
 */
void insert_pokemon_before_current(Pokedex pokedex, Pokemon pokemon);

/*
 *  Return the number of Pokemon in the Pokedex of a specific type.
 *  Refer to pokemon_types.h & pokemon_types.c for more information on types
 */
int count_pokemon_of_type(Pokedex pokedex, char *type);

/*
 * Remove the currently selected Pokemon from the Pokedex.
 * Associated Pokemon information should be free'd.
 *
 * The cursor (currently-selected pokemon) should then be set to 
 * the next successive pokemon. 
 *
 * If there is no successive pokemon, the cursor should be reset to
 * the first pokemon of the Pokedex.
 *
 * If there are no Pokemon in the Pokedex, this function does nothing.
 */
void remove_pokemon(Pokedex pokedex);

/*
 * Destroy a given Pokedex and free all associated memory.
 */
void destroy_pokedex(Pokedex pokedex);

/*
 * Prints out all unique types of found Pokemon stored in the Pokedex,
 * each separated by a new-line character.
 *
 * E.g. If only one "ghost-type" Pokemon exists on the Pokedex,
 * but is not set as `found`, then this function should NOT print out
 * "Ghost".
 *
 * If multiple pokemon of a single type exist on the Pokedex, the type
 * should only be printed once.
 * E.g. if two "ice-type" Pokemon exist on the Pokedex,
 * then this function should only print out "Ice" ONCE.
 *
 * If the Pokedex is empty, this function does nothing.
 *
 * Types should be printed out in the order that they appear in the Pokedex.
 * Refer to the reference solution for this behaviour.
 *
 * The "None"-type should NOT be printed in this function.
 * 
 * Refer to pokemon_types.h & pokemon_types.c for further information on types.
 */
void show_types(Pokedex pokedex);

/*
 * Return the number of Pokemon in the Pokedex that have been found.
 */
int count_found_pokemon(Pokedex pokedex);

/*
 * Return the total number of Pokemon in the Pokedex, whether or not
 * they have been found.
 */
int count_total_pokemon(Pokedex pokedex);

/*
 * Saves all information within pokedex to a text file.
 *
 * You do NOT need to save a Pokemon's "found" status.
 *
 * Use the functions in ext_save.h to save the contents to a file.
 */
void save_pokedex(Pokedex pokedex, char* filename);

/*
 * Loads a pokedex from information stored in a text file.
 * The way the information is formatted is entirely dependent
 * on how you decided to implement the "save_pokedex" function.
 *
 * All pokemon should be set to "found".
 *
 * Use the functions in ext_save.h to load the text from a file.
 */
Pokedex load_pokedex(char* filename);

/*
 * Create a new Pokedex which contains only the Pokemon of a specified
 * type from the original Pokedex.
 *
 * If a Pokemon has more than one type, only one of its types has to
 * match the given type to be included in the Pokedex. i.e. if either
 * the first_type or second_type matches the given type, it should be
 * included in the new Pokedex.
 *
 * The Pokemon should appear in this Pokedex in the same order as they
 * appeared in the original Pokedex.
 *
 * If a Pokemon has not yet been found, it should not be added to the
 * new Pokedex.
 *
 * The currently selected Pokemon in the new Pokedex should be the
 * first Pokemon inserted into the Pokedex.
 *
 * If there are no matching Pokemon, this function should return an
 * empty Pokedex.
 *
 * This function should not alter the original Pokedex. It should create
 * copies of any matching Pokemon in the original Pokedex, and insert
 * those copies into the new Pokedex.
 *
 * When this function is called, you must also save the original Pokedex state, so 
 * you can return to how the Pokedex was prior to the search
 *
 */
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type);

/*
 * Create a new Pokedex which contains only the Pokemon that have
 * previously been 'found' from the original Pokedex.
 *
 * The Pokemon should appear in this Pokedex in the order that they appeared in the
 * original Pokedex.
 *
 * The currently selected Pokemon in the new Pokedex should be the
 * first Pokemon in the Pokedex.
 *
 * If there are no matching Pokemon, this function should return an
 * empty Pokedex.
 *
 * This function should not alter the original Pokedex.  It should
 * create copies of any Pokemon which are in the original Pokedex, and
 * insert those copies into the new Pokedex.
 *
 * When this function is called, you must also save the original Pokedex state, so 
 * you can return to how the Pokedex was prior to the search
 *
 */
Pokedex get_found_pokemon(Pokedex pokedex);

/*
 * Create a new Pokedex containing only the Pokemon from the original
 * Pokedex which have the given string appearing in its name.
 * (e.g. if the text provided is 'basau' then 'Bulbasaur' should be one
 * of the Pokemon in the returned Pokedex)
 *
 * The text provided is not case sensitive,  e.g. if the text provided
 * is 'bulb', then 'Bulbasaur' should be one of the Pokemon in the
 * returned Pokedex.
 *
 * If a Pokemon has not yet been found, it should not be added to the
 * new Pokedex.
 *
 * The currently selected Pokemon in the new Pokedex should be the
 * first Pokemon in the Pokedex.
 *
 * If there are no matching Pokemon, this function should return an
 * empty Pokedex.
 *
 * This function should not alter the original Pokedex.  It should
 * create copies of any Pokemon which are in the original Pokedex, and
 * insert those copies into the new Pokedex.
 *
 * When this function is called, you must also save the original Pokedex state, so 
 * you can return to how the Pokedex was prior to the search
 *
 * !! You must not call the strstr() function from string.h in this function !!
 */
Pokedex search_pokemon(Pokedex pokedex, char *text);

/*
 * Destroys the current sub-list and returns the previous pokedex state, 
 * prior to the most recent search.
 *
 * I.e if both get_found_pokemon() and get_pokemon_of_type() were subsequently
 * called on a Pokedex, end_search would have to be called TWICE to revert back
 * to the original Pokedex state.
 */
Pokedex end_search(Pokedex sub_pokedex);

#endif //  _POKEDEX_H_
