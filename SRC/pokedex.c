// Assignment 2 21T2 COMP1511: Pokedex
// pokedex.c
//
// This program was written by nick kapsanis (z5254990)
// on 4 Aug 2021
//
// Version 1.0.0: Release
// Version 1.0.1: Fixed references to create_new_dexnode in comments and error messages

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ext_save.h"

// ----------------------------------------------
// Add any extra #includes your code needs here.
// ----------------------------------------------

#include "pokedex.h"


// ----------------------------------------------
// Add your own #defines here.
// ----------------------------------------------

// Defines printing hidden or not
#define IS_FOUND 1
#define NOT_FOUND 0

// Defines the max number of pokemon types including none_type
#define MAX_TYPES 19


// Struct pokedex is the pointer to the list head and
// currently selected dexnode
struct pokedex {
    struct dexnode *head;
    struct dexnode *currently_selected;
};


// Struct dexnode creates the body of the pokedex list, contains pointers
// forward and backwards along the list and to its associated pokemon,
// also contains data on the found status of the pokemon it links to
struct dexnode {
    Pokemon pokemon;
    struct dexnode *next;
    struct dexnode *previous;
    int found;
};


// ----------------------------------------------
// Add your own structs here.
// ----------------------------------------------




// ----------------------------------------------
// Own function prototypes
// ----------------------------------------------

// Takes a pointer to a Pokedex, returns a pointer to its end node.
struct dexnode *find_end_node(Pokedex pokedex);

// Creates a new dexnode struct and returns a pointer to it.
static struct dexnode *create_new_dexnode(Pokemon pokemon);

// Prints a string replacing each char with hyphens
void print_name_hidden(Pokemon to_print);

// Checks if a value is stored in an array of size MAX_TYPES
// this is not a general function.
int is_in_array(int *array, int to_check);

// Stores a type_code in an array in the first slot that == NONE_TYPE
// this is not a general function.
void store_in_array(int *array, int to_store);

// prints an array of type codes converting to strings
void print_type_array(int *type_to_print);

// ----------------------------------------------
// Definitions begin here.
// ----------------------------------------------


// Creates a new Pokedex, and returns a pointer to it.
Pokedex new_pokedex(void) {
    // Malloc memory for a new Pokedex + check that malloc succeeded
    Pokedex pokedex = malloc(sizeof(struct pokedex));
    assert(pokedex != NULL);

    // Set the head of the linked list to be NULL.
    // Set the currently selected node to be head.
    pokedex->head = NULL;
    pokedex->currently_selected = NULL;
    
    return pokedex;
}

// Create a new dexnode struct and returns a pointer to it.
//
// This function allocates memory for a dexnode, set its pokemon
// pointer to point to the specified pokemon, and returns a pointer to
// the allocated memory. Sets unknown fields to NULL. Error checks.
static struct dexnode *create_new_dexnode(Pokemon pokemon) {
    struct dexnode *new_dexnode = malloc(sizeof(struct dexnode));
    assert(new_dexnode != NULL);
    new_dexnode->pokemon = pokemon;
    new_dexnode->next = NULL;
    new_dexnode->previous = NULL;
    // new nodes are 'found' until otherwise set
    new_dexnode->found = IS_FOUND;
    
    return new_dexnode;
}

//////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                        //
//////////////////////////////////////////////////////////////////////

// Add a new Pokemon to the Pokedex.
void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    struct dexnode *new_dexnode = create_new_dexnode(pokemon);
    // In the case of a nullset pokedex i.e add first pokemon
    if (pokedex->head == NULL) {
        pokedex->head = new_dexnode;
        // only for first node, set selection
        pokedex->currently_selected = pokedex->head;
    }
    // insert at end of any size list
    else {
        struct dexnode *end_node = find_end_node(pokedex);
        // puts new node at the end of the list
        end_node->next = new_dexnode;
        // points previous as the previous end node
        new_dexnode->previous = end_node;
    }

}

// Print out all of the Pokemon in the Pokedex.
void print_pokedex(Pokedex pokedex) {
    struct dexnode *to_print = pokedex->head;

    while (to_print != NULL) {
        if (to_print == pokedex->currently_selected) {
            printf("--> ");
        }
        else {
            printf("    ");
        }
        printf("#");
        // The following booleans arrange the printed ID to always be 3 digits
        if (pokemon_id(to_print->pokemon) < 10) {
            printf("00%d", pokemon_id(to_print->pokemon));
        } 
        else if (pokemon_id(to_print->pokemon) >= 10 
            && pokemon_id(to_print->pokemon) < 100) {
            printf("0%d", pokemon_id(to_print->pokemon));
        }
        else printf("%d", pokemon_id(to_print->pokemon));
        
        // Print the name varying depending on found status
        printf(": ");
        if (to_print->found == IS_FOUND) {
            printf("%s", pokemon_name(to_print->pokemon));
        }
        else {
            print_name_hidden(to_print->pokemon);
        }
        printf("\n"); 
        
        //move pointer along list
        to_print = to_print->next; 
    }
}

// Print out the details of the currently selected Pokemon.
void detail_current_pokemon(Pokedex pokedex) {
    Pokemon to_print = get_current_pokemon(pokedex);
    printf("ID: ");
    // The following booleans arrange the printed ID to always be 3 digits
    if (pokemon_id(to_print) < 10) {
        printf("00%d", pokemon_id(to_print));
    } 
    else if (pokemon_id(to_print) >= 10 
        && pokemon_id(to_print) < 100) {
        printf("0%d", pokemon_id(to_print));
    }
    else printf("%d", pokemon_id(to_print));
    printf("\n");
    
    // Print the details of the pokemon
    // or dashes depending on found status
    if (pokedex->currently_selected->found == IS_FOUND) {
        printf("Name: %s\n", pokemon_name(to_print));
        printf("Height: %lf m\n", pokemon_height(to_print));
        printf("Weight: %lf kg\n", pokemon_weight(to_print));
        // Print the type/s depending on if a second type is available
        // coverts type code into string and prints it
        printf("Type: %s ", type_code_to_str(pokemon_first_type(to_print)));
        if (pokemon_second_type(to_print) != NONE_TYPE) {
            printf("%s", type_code_to_str(pokemon_second_type(to_print)));
        }
    }
    else {
        // If pokemon is not found 
        // print hidden name.
        printf("Name: ");
        print_name_hidden(to_print);
        printf("\n");
        // Print other details, all hidden
        printf("Height: --\nWeight: --\nType: --");
    }
    printf("\n");
}

// Return the currently selected Pokemon.
Pokemon get_current_pokemon(Pokedex pokedex) { 
    // if the pokedex and thus selection is NULL, return NULL
    if (pokedex->currently_selected == NULL) return NULL;    
    Pokemon current = pokedex->currently_selected->pokemon;
    
    return current;
}

// Change the currently selected Pokemon to be the next Pokemon in the Pokedex.
void next_pokemon(Pokedex pokedex) {
    // if last node or null is selected, do nothing
    if (pokedex->currently_selected == NULL 
    || pokedex->currently_selected->next == NULL) {
        return;
    }
    // for all other nodes in the list, increment by 1 in the forwards direction.
    else {
        pokedex->currently_selected = pokedex->currently_selected->next;
    }
}

// Change the currently selected Pokemon to be the previous Pokemon in the Pokedex.
void prev_pokemon(Pokedex pokedex) {
    // if first node or null is selected, do nothing
    if (pokedex->currently_selected == NULL 
    || pokedex->currently_selected->previous == NULL) {
        return;
    }
    // for all other nodes in the list, increment by 1 in the backwards direction.
    else {
        pokedex->currently_selected = pokedex->currently_selected->previous;
    }
}

// Change the currently selected Pokemon to be the Pokemon with the ID `id`.
void jump_pokemon(Pokedex pokedex, int id) {   
    // if null is selected, i.e empty list, do nothing
    if (pokedex->currently_selected == NULL) {
        return;
    }
    struct dexnode *pointer = pokedex->head;
    while (pointer != NULL) {
        // If ID's match between list and input, set selection and return
        if (pokemon_id(pointer->pokemon) == id) {
            pokedex->currently_selected = pointer;
            return;
        }
        pointer = pointer->next;
    }
    // If at this point (last node) and no match, ID does not exist.
    printf("Could not find a pokemon by that ID\n");
}

//////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                        //
//////////////////////////////////////////////////////////////////////

// Sets the currently selected pokemon's "found" status
void set_current_pokemon_found(Pokedex pokedex, int found) {
    if (pokedex->currently_selected == NULL) {
        return;
    }
    pokedex->currently_selected->found = found;
}

// Add a pokemon before the current pokemon.
void insert_pokemon_before_current(Pokedex pokedex, Pokemon pokemon) {
    // initialise the dexnode
    struct dexnode *new_dexnode = create_new_dexnode(pokemon);
    new_dexnode->found = NOT_FOUND;
    
    // In the case of null list
    if (pokedex->head == NULL) {
        pokedex->head = new_dexnode;
    }
    
    // In the case of placing before current head
    else if (pokedex->currently_selected == pokedex->head) {
        // Sets previous and next pointers for 'forward' node
        pokedex->currently_selected->previous = new_dexnode;
        new_dexnode->next = pokedex->currently_selected;
        // Move the head pointer to the new node
        pokedex->head = new_dexnode;       
    }
    
    // In all other cases
    else {
        // Create a pointer to the node 'in front of' where 
        // the new dexnode will go. i.e the node previous to new_dexnode
        struct dexnode *temp_pointer = pokedex->currently_selected->previous;
        
        // Set relations with 'next' node i.e the node currently selected
        pokedex->currently_selected->previous = new_dexnode;
        new_dexnode->next = pokedex->currently_selected;
        
        // Set relations with 'previous' node using the temp pointer
        new_dexnode->previous = temp_pointer;
        temp_pointer->next = new_dexnode;
    }
    // Set currently selected to be the new dexnode
    pokedex->currently_selected = new_dexnode;
}

// Return the total number of Pokemon in the Pokedex.
int count_pokemon_of_type(Pokedex pokedex, char *type) {
    struct dexnode *pointer = pokedex->head;
    int type_code = type_str_to_code(type);
    int count = 0;
    while (pointer != NULL) {
        if (pokemon_first_type(pointer->pokemon) == type_code
        || pokemon_second_type(pointer->pokemon) == type_code) {
            count++;
        }
        pointer = pointer->next;
    }
    return count;
}

//////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                        //
//////////////////////////////////////////////////////////////////////


// Remove the currently selected Pokemon from the Pokedex.
void remove_pokemon(Pokedex pokedex) {
    // in the case of null list
    if (pokedex->head == NULL) return;
    struct dexnode *to_delete = pokedex->currently_selected;
    
    // free the pokemon node
    destroy_pokemon(to_delete->pokemon);
    
    if (to_delete == pokedex->head) {
    
        // if this is true to_delete is the head of a single node list
        if (to_delete->next == NULL) {
    
            // make null all pointers of pokedex and free the head
            pokedex->head = NULL;
            pokedex->currently_selected = NULL;
            free(to_delete);
            return;
        }
        // if this is true to_delete is the head of a list of n nodes
        // reassign the head of the list and the selection
        pokedex->head = to_delete->next;
        pokedex->currently_selected = to_delete->next;
        // reassign the new head and frees to_delete
        pokedex->head->previous = NULL;
        free(to_delete);
        return;
    }
    // this is triggered the node CANNOT BE a head node
    // MUST be a tail node of a list of n nodes
    if (to_delete->next == NULL) {
        // reasign previous node and selection, and 'free' to_delete
        to_delete->previous->next = NULL;
        pokedex->currently_selected = pokedex->head;
        free(to_delete);
        return;  
    } 
    // for the case of any other node, mid list
    // reassign the nodes on either side to "pass through" to_delete
    to_delete->previous->next = to_delete->next;
    to_delete->next->previous = to_delete->previous;
    pokedex->currently_selected = to_delete->next;
    free(to_delete);
    return;
}

// Destroy the given Pokedex and free all associated memory.
void destroy_pokedex(Pokedex pokedex) {
    
    // if the pokedex is empty free the pokedex
    if (pokedex->head == NULL) {
        free(pokedex);
        return;
    }
    // otherwise continue freeing the head of the list until it is
    pokedex->currently_selected = pokedex->head;
    while (pokedex->head != NULL) {
        remove_pokemon(pokedex);
    }
    // at this stage the pokedex is empty
    free(pokedex);
    return;
}

// Prints out all types of found Pokemon stored in the Pokedex
void show_types(Pokedex pokedex) {

    int type_counter[MAX_TYPES] = {NONE_TYPE};
    struct dexnode *pointer = pokedex->head;
    while (pointer != NULL) {
        // only check types if pokemon is found
        if (pointer->found == IS_FOUND) {
            int type_1 = pokemon_first_type(pointer->pokemon);
            int type_2 = pokemon_second_type(pointer->pokemon);
            
            // if type 1 IS NOT in the array, store it in the next slot       
            if (is_in_array(type_counter, type_1) == 0) {
                store_in_array(type_counter, type_1);
            }
            
            // if type 2 IS NOT in the array, store it in the next slot
            // note that as array is initialised as NONE_TYPE, a pokemon
            // with only a first type will store only a first type
            if (is_in_array(type_counter, type_2) == 0) {
                store_in_array(type_counter, type_2);          
            } 
        }       
        pointer = pointer->next;
    }
    //prints the array of all types
    print_type_array(type_counter);
}

// Return the number of Pokemon in the Pokedex that have been found.
int count_found_pokemon(Pokedex pokedex) {
    if (pokedex->head == NULL) {
        return 0;
    }
    // cycle the list counting every node thats also "found"
    struct dexnode *pointer = pokedex->head;
    int pokemon_count = 1;
    while (pointer->next != NULL) {
        if (pointer->found == IS_FOUND) {
            pokemon_count++;
        }
        pointer = pointer->next;
    }
    return pokemon_count;
}

// Return the total number of Pokemon in the Pokedex.
int count_total_pokemon(Pokedex pokedex) {
    if (pokedex->head == NULL) {
        return 0;
    }
    // cycle the list counting every node
    struct dexnode *pointer = pokedex->head;
    int pokemon_count = 1;
    while (pointer->next != NULL) {
        pokemon_count++;
        pointer = pointer->next;
    }
    return pokemon_count;
}

// NOTE: Make sure you submit ass2_pokedex before completing these.
//////////////////////////////////////////////////////////////////////
//                     Stage 4 Extension Functions                  //
//////////////////////////////////////////////////////////////////////

/*
 * Saves a pokedex to a text file
 * Use the functions in ext_save.h to save the contents to a file
 */
void save_pokedex(Pokedex pokedex, char *filename) {
    fprintf(stderr,
     "exiting because you have not implemented"
     " the save_pokedex function\n");
    exit(1);

}

/*
 * Loads a pokedex from a text file
 * Use the functions in ext_save.h to load the text from a file
 */
Pokedex load_pokedex(char *filename) {
    fprintf(stderr, 
    "exiting because you have not implemented"
    " the load_pokedex function\n");
    exit(1);

    return NULL;
}

//////////////////////////////////////////////////////////////////////
//                     Stage 5 Extension Functions                  //
//////////////////////////////////////////////////////////////////////

// Create a new Pokedex which contains only the Pokemon of a specified
// type from the original Pokedex.
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    fprintf(stderr, 
    "exiting because you have not implemented"
     " the get_pokemon_of_type function\n");
    exit(1);
}

// Create a new Pokedex which contains only the Pokemon that have
// previously been 'found' from the original Pokedex.
Pokedex get_found_pokemon(Pokedex pokedex) {
    fprintf(stderr,
    "exiting because you have not implemented"
    " the get_found_pokemon function\n");
    exit(1);
}

// Create a new Pokedex containing only the Pokemon from the original
// Pokedex which have the given string appearing in its name.
Pokedex search_pokemon(Pokedex pokedex, char *text) {
    fprintf(stderr,
     "exiting because you have not implemented"
     " the search_pokemon function\n");
    exit(1);
}

// Free's the current sub-list and returns the original pokedex state, 
// prior to the search
Pokedex end_search(Pokedex sub_pokedex) {
    fprintf(stderr,
     "exiting because you have not "
     "implemented the end_search function\n");
    exit(1);
}

// Add definitions for your own functions here.

// Function takes a pointer to a pokedex and returns a pointer to its final node
// will not alter any values pointers or nodes will not accept a null pointer
struct dexnode *find_end_node(Pokedex pokedex) {
    struct dexnode *pointer = pokedex->head;
    while (pointer->next != NULL) {
        pointer = pointer->next;
    }
    return pointer;
}
// takes a pointer to a pokemon and prints its name in asterisks
//      DOES NOT ERROR CHECK, WILL BREAK IF GIVEN NULL POINTER
void print_name_hidden(Pokemon to_print) {
    int name_length = strlen(pokemon_name(to_print));
    int i = 0;
    while (i < name_length) {
        printf("*");
        i++;
    }
}
// takes an array, and a value and checks if that value is already 
// stored in the array returns a 1 if match and 0 if not
int is_in_array(int *array, int to_check) {
    int i = 0;
    while (i < MAX_TYPES) {
        // if match end function and return 1
        if (array[i] == to_check) {
            return 1;
        }
        i++;
    }
    // if no match by end of array, there is no match return 0
    return 0;
}
// Stores a code in an array, in the first slot it reads that 
// has a type_code of NONE_TYPE in it. 
void store_in_array(int *array, int to_store) {
    int i = 0;
    while (i < MAX_TYPES) {
        // if NONE_TYPE, overwrite the value with to_store and returns
        if (array[i] == NONE_TYPE) {
            array[i] = to_store;
            return;
        }
        i++;
    }
    return;
}
// prints an array of type codes converted into string
void print_type_array(int *type_to_print) {
    int i = 0;
    // array should be filled with lots of types
    // but will terminate with NONE_TYPE so do not print those
    while (i < MAX_TYPES) {
        if (type_to_print[i] != 0) {
            printf("%s\n", type_code_to_str(type_to_print[i]));
        }
        i++;
    }
}
