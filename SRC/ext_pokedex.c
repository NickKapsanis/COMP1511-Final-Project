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

// Defines the max string length of ascii codes height or weight can be
// a reasonable lower limit is 40. But this is set much higher,
// to make it less likley to encounter a buffer overflow from silly pokemon names
#define MAX_ACSII 400

// Struct pokedex is the pointer to the list head and
// currently selected dexnode
struct pokedex {
    struct dexnode *head;
    struct dexnode *currently_selected;
    Pokedex previous;
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

// saves the data about a pokemon in ascii codes
char *save_pokemon_to_string(Pokemon pokemon, char *pokemon_string);

// function will increment along a string until it counts 5 white spaces.
// returns an int of the amount incremented by. for use in divinging load file txt
int count_spaces_till_6(char string[], int index);

// function will determine if a string exists in another string 
// retuns 1 if it is and 0 if not
int string_in_string(char check_for[], char search_in[]);

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
    
    // Set the next and previous pointers
    pokedex->previous = NULL;
    
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
    //printf("name: %s\n", pokemon_name(pokemon));
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
    
    // In the case of invalid filename do nothing
    if (is_valid_save_name(filename) == 0) {
        return;
    }
    // In case of empty pokedex, save an empty string;
    if (pokedex->head == NULL) {
        save_string(filename, NULL);
    }
    // If filename is valid and pokedex is at least 1 node filled
    struct dexnode *current = pokedex->head;
    // sets the save limit to be the size of 999 pokemon macrostrings as 999
    // is max allowable pokemon
    char total_data[MAX_ACSII * 6 * 999] = {'\0'};
    while (current != NULL) {
        // create a macrostring the size of 6 microstrings and save 1 pokemon to it
        char pokemon_string[MAX_ACSII * 6] = {'\0'};  
        save_pokemon_to_string(current->pokemon, pokemon_string);
        // save macrostring to the end of the "megastring"
        strcat(total_data, pokemon_string);

        current = current->next;       
    }
    // save the megastring to file.
    save_string(filename, total_data);
}

/*
 * Loads a pokedex from a text file
 * Use the functions in ext_save.h to load the text from a file
 */
Pokedex load_pokedex(char *filename) {
    // load the save file back in as a string
    char *total_data = load_string(filename);

    // create a new pokedex that is empty. to be populated with save file
    Pokedex load_pokedex = new_pokedex();
    
    if (total_data == NULL) {
        return load_pokedex;
    }
          
    int i = 0;
    while (total_data[i] != '\0') {
        int id;
        char name[MAX_ACSII];
        double height;
        double weight;
        char type1[MAX_ACSII];
        char type2[MAX_ACSII];
        
        // scan a pokemons worth of data into a string, starting at string index of 'i' 
        sscanf(&total_data[i], "%d %s %lf %lf %s %s ",
        &id, name, &height, &weight, type1, type2);
        
        // convert string into type code  
        pokemon_type first_type = type_str_to_code(type1);
        pokemon_type second_type = type_str_to_code(type2);
        
        // create a new pokemon using the data from the save string
        Pokemon pokemon = new_pokemon(id, name, height, weight, first_type, second_type);
           
        // add the new pokemon node to the new load_pokedex
        add_pokemon(load_pokedex, pokemon);
           
        // increment along the data string by 1 pokemons worth of whitespace
        i = count_spaces_till_6(total_data, i);       
    }
    free(total_data);
    
    return load_pokedex;
}

//////////////////////////////////////////////////////////////////////
//                     Stage 5 Extension Functions                  //
//////////////////////////////////////////////////////////////////////

// Create a new Pokedex which contains only the Pokemon of a specified
// type from the original Pokedex.
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    // Create new pokedex and index it after the current pokedex
    Pokedex type_sorted_pokedex = new_pokedex();
    type_sorted_pokedex->previous = pokedex;
    
    // Create a pointer for the origional pokedex to check for types
    // and use it to scan the pokedex
    struct dexnode *pointer = pokedex->head;
    while (pointer != NULL) {
        if (pointer->found == IS_FOUND) {
            if (pokemon_first_type(pointer->pokemon) == type
            || pokemon_second_type(pointer->pokemon) == type) {       
                // if types match copy the pokemon to the new pokedex.
                add_pokemon(type_sorted_pokedex, clone_pokemon(pointer->pokemon));
            }
        }      
        pointer = pointer->next;
    }
    return type_sorted_pokedex;
}

// Create a new Pokedex which contains only the Pokemon that have
// previously been 'found' from the original Pokedex.
Pokedex get_found_pokemon(Pokedex pokedex) {
    // Create new pokedex and index it after the current pokedex
    Pokedex found_sorted_pokedex = new_pokedex();
    found_sorted_pokedex->previous = pokedex;
    
    // Create a pointer for the origional pokedex to check if found
    // and use it to scan the pokedex
    struct dexnode *pointer = pokedex->head;
    while (pointer != NULL) {
        if (pointer->found == IS_FOUND) {       
            // if pokemon is found copy the pokemon to the new pokedex.
            add_pokemon(found_sorted_pokedex, clone_pokemon(pointer->pokemon));
        }      
        pointer = pointer->next;
    }
    return found_sorted_pokedex;
}

// Create a new Pokedex containing only the Pokemon from the original
// Pokedex which have the given string appearing in its name.
Pokedex search_pokemon(Pokedex pokedex, char *text) {
    // Create new pokedex and index it after the current pokedex
    Pokedex search_sorted_pokedex = new_pokedex();
    search_sorted_pokedex->previous = pokedex;
    
    // Create a pointer for the origional pokedex to check matches
    // and use it to scan the pokedex
    struct dexnode *pointer = pokedex->head;
    while (pointer != NULL) {
        if (pointer->found == IS_FOUND) {
            if (string_in_string(text, pokemon_name(pointer->pokemon)) == 1) {       
                // if conditions allow, add a copy of the pokemon to the new pokedex
                add_pokemon(search_sorted_pokedex, clone_pokemon(pointer->pokemon));
            }
        }         
        pointer = pointer->next;
    }
    return search_sorted_pokedex;
}

// Free's the current sub-list and returns the original pokedex state, 
// prior to the search
Pokedex end_search(Pokedex sub_pokedex) {
    // if the current pokedex is the origional return
    if (sub_pokedex->previous == NULL) {
        return sub_pokedex;
    }
    // if not, create a pointer to the previous pokedex, to return
    Pokedex previous_pokedex = sub_pokedex->previous;
    // destory (free), the current pokedex 
    destroy_pokedex(sub_pokedex);
    
    return previous_pokedex;
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
// takes a pointer to a pokemon node, returns a single string "macrostring"containing its
// data in the form "ID NAME HEIGHT WEIGHT TYPE1 TYPE1"
char *save_pokemon_to_string(Pokemon pokemon, char *pokemon_string) {
    char poke_id[MAX_ACSII] = {'\0'};  
    sprintf(poke_id, "%d", pokemon_id(pokemon));
    // start by converting all data points of a pokemon into 'microstrings'
    char *poke_name = pokemon_name(pokemon);
    
    // convert doubles using sprintf
    char poke_height[MAX_ACSII] = {'\0'};
    sprintf(poke_height, "%f", pokemon_height(pokemon));
    char poke_weight[MAX_ACSII] = {'\0'};
    sprintf(poke_weight, "%f", pokemon_weight(pokemon));
    
    // converting/ saving static char type strings using string copy
    char poke_type_1[MAX_ACSII] = {'\0'};
    strcpy(poke_type_1, type_code_to_str(pokemon_first_type(pokemon)));
    char poke_type_2[MAX_ACSII] = {'\0'};
    strcpy(poke_type_2, type_code_to_str(pokemon_second_type(pokemon)));

    // use strcat to add each microstring to the end of the macrostring
    // adding a space bewteen each microstring, as it is an invalid name double or tyoe
    // so will be easily recognicable when reading the save file to load it
    strcat(pokemon_string, poke_id);
    strcat(pokemon_string, " ");
    strcat(pokemon_string, poke_name);
    strcat(pokemon_string, " ");
    strcat(pokemon_string, poke_height);
    strcat(pokemon_string, " ");
    strcat(pokemon_string, poke_weight);
    strcat(pokemon_string, " ");
    strcat(pokemon_string, poke_type_1);
    strcat(pokemon_string, " ");
    strcat(pokemon_string, poke_type_2);
    strcat(pokemon_string, " ");

    return pokemon_string;
}
// function will increment along a string until it counts 5 white spaces.
// returns an int of the amount incremented by. for use in divinging load file txt
int count_spaces_till_6(char string[], int index) {
    int counter = 0;
    while (counter < 6) {
        if (string[index] == ' ') {
            counter++;
        }
        index++;
    }
    return index;
}
// function will determine if a string exists in another string 
// retuns 1 if it is and 0 if not
int string_in_string(char check_for[], char search_in[]) {
    // first loop looks for a matching character, if this does not 
    //exist return 0, if it does continue to next loop
    int search_index = 0;
    while (search_in[search_index] != '\0') {
        if (strncasecmp(check_for, &search_in[search_index], 1) == 0) {
            // do string compare on the remainder of search_in
            int check = strncasecmp(check_for, 
            &search_in[search_index], strlen(check_for));
            if (check == 0) {
                return 1;
            }
        }
        search_index++;
       
    }
    return 0;
}