// Assignment 2 20T2 COMP1511: Pokedex
// main.c
//
// You should not need to change this file.
// This file will not be marked.
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>

#include "pokedex.h"
#include "pokemon.h"

#define TRUE  1
#define FALSE 0

#define MAX_LINE    1024

struct pokedex_state {
    Pokedex pokedex;
    int save_load_ability;
    int finished;
};

static struct pokedex_state run_command(Pokedex pokedex, int save_load_ability, char *line);

static void print_welcome_msg(void);
static void show_help(void);
static void print_prompt(void);

static void do_add(Pokedex pokedex, char *line);
static void do_print(Pokedex pokedex);
static void do_details(Pokedex pokedex);
static void do_insert(Pokedex pokedex, char *line);
static void do_count_type(Pokedex pokedex, char *line);
static void do_next(Pokedex pokedex);
static void do_prev(Pokedex pokedex);
static void do_jump(Pokedex pokedex, char *line);
static void do_remove(Pokedex pokedex);
static void do_set_find(Pokedex pokedex, char *line);
static void do_show_types(Pokedex pokedex);
static void do_count(Pokedex pokedex);
static void do_count_found(Pokedex pokedex);
static void do_save(Pokedex pokedex, char *line, int save_load_ability);
static Pokedex do_load(Pokedex pokedex, char *line, int save_load_ability);
static Pokedex do_filter_found(Pokedex pokedex);
static Pokedex do_filter_search(Pokedex pokedex, char *line);
static Pokedex do_filter_type(Pokedex pokedex, char *line);
static Pokedex do_filter_end(Pokedex pokedex);

static void do_quit(void);

int main(void) {
    char line[MAX_LINE];

    Pokedex pokedex = new_pokedex();
    print_welcome_msg();

    struct pokedex_state state;
    state.pokedex = pokedex;
    state.finished = FALSE;
    state.save_load_ability = TRUE;

    print_prompt();
    while (!state.finished && fgets(line, MAX_LINE, stdin) != NULL) {
        state = run_command(state.pokedex, state.save_load_ability, line);
        if (!state.finished) {
            print_prompt();
        }
    }

    destroy_pokedex(state.pokedex);
    return 0;
}

static struct pokedex_state run_command(Pokedex pokedex, int save_load_ability, char *line) {
    char cmd = line[0];
    struct pokedex_state state;
    state.pokedex = pokedex;
    state.save_load_ability = save_load_ability;
    state.finished = FALSE;

    Pokedex new = NULL;

    if (cmd == 'a') {
        do_add(pokedex, line);
    } else if (cmd == 'p') {
        do_print(pokedex);
    } else if (cmd == 'd') {
        do_details(pokedex);
    } else if (cmd == 'i') {
        do_insert(pokedex, line);
    } else if (cmd == 't') {
        do_count_type(pokedex, line);
    } else if (cmd == '>') {
        do_next(pokedex);
    } else if (cmd == '<') {
        do_prev(pokedex);
    } else if (cmd == 'j') {
        do_jump(pokedex, line);
    } else if (cmd == 'r') {
        do_remove(pokedex);
    } else if (cmd == 'f') {
        do_set_find(pokedex, line);
     } else if (cmd == 'y') {
        do_show_types(pokedex);   
    } else if (cmd == 'c') {
        do_count_found(pokedex);
    } else if (cmd == 'n') {
        do_count(pokedex);
    } else if (cmd == 'S') {
        do_save(pokedex, line, save_load_ability);
    } else if (cmd == 'L') {
        new = do_load(pokedex, line, save_load_ability);
    } else if (cmd == 'F') {
        state.save_load_ability = FALSE;
        new = do_filter_found(pokedex);
    } else if (cmd == 'R') {
        state.save_load_ability = FALSE;
        new = do_filter_search(pokedex, line);
    } else if (cmd == 'T') {
        state.save_load_ability = FALSE;
        new = do_filter_type(pokedex, line);
    } else if (cmd == 'E') {
        new = do_filter_end(pokedex);
    } else if (cmd == 'q') {
        do_quit();
        state.finished = TRUE;
    } else if (cmd == '?') {
        show_help();
    } else if (cmd == '\n') {
        // Don't do anything, just print the prompt again.
    } else {
        printf("Unknown Command '%c'\n", cmd);
    }

    if (new) state.pokedex = new;

    return state;
}

static void print_welcome_msg(void) {
    printf(
        "===========================[ Pokédex ]==========================\n"
        "            Welcome to the Pokédex!  How can I help?\n"
        "================================================================\n"
    );
}

static void show_help(void) {
    char *msg =
        "============================[ Help ]============================\n"
        "  a [pokemon_id] [name] [height] [weight] [type1] [type2]\n"
        "    Add a Pokemon to the list\n"
        "  p\n"
        "    Print all of the Pokemon in the Pokedex (in the order they were added)\n"
        "  d\n"
        "    Display details of the currently selected Pokemon\n"
        "  >\n"
        "    Move the cursor to the next Pokemon in the list\n"
        "  <\n"
        "    Move the cursor to the previous Pokemon in the list\n"
        "  j [pokemon_id]\n"
        "    Move the cursor to a specific Pokemon with the specified pokemon_id\n"
        "  i [pokemon_id] [name] [height] [weight] [type1] [type2]\n"
        "    Insert pokemon before currently selected pokemon \n"
        "  t [type]\n"
        "    Count pokemon of a specific type\n"
        "  r\n"
        "    Remove the current Pokemon from the Pokedex\n"
        "  f [found]\n"
        "    Set current pokemon's 'found' status (1 = True, 0 = False)\n"
        "  y\n"
        "    Show Pokemon types\n"
        "  c\n"
        "    Print out the count of Pokemon who have been found\n"
        "  n\n"
        "    Print out the total count of Pokemon in the Pokedex\n"
        "  S [filename]\n"
        "    Save Pokedex state to file\n"
        "  L [filename]\n"
        "    Load Pokedex state from file\n"
        "  F\n"
        "    Filters the current Pokedex to only contain Pokemon that have previously been found\n"
        "  R [string]\n"
        "    Filters the current Pokedex to only contain Pokemon that have the specified string in their name\n"
        "  T [type]\n"
        "    Filters the current Pokedex to only contain Pokemon that have the specified type\n"
        "  E\n"
        "    Reverts a filtered Pokedex back into the original Pokedex\n"
        "  q\n"
        "    Quit\n"
        "  ?\n"
        "    Show help\n"
        "================================================================\n";
    printf("%s", msg);
}

static void print_prompt(void) {
    printf("Enter command: ");
}

static void do_add(Pokedex pokedex, char *line) {
    int pokemon_id;
    double height, weight;
    char name[MAX_LINE] = {0};
    char type1[MAX_LINE] = {0};
    char type2[MAX_LINE] = {0};

    int items_read = sscanf(line, "a %d %s %lf %lf %s %s", &pokemon_id,
        name, &height, &weight, type1, type2);

    if (items_read < 5) {
        printf("Invalid Add Command\n");
        return;
    }

    Pokemon pokemon = new_pokemon(pokemon_id, name, height, weight,
        type_str_to_code(type1), type_str_to_code(type2));

    add_pokemon(pokedex, pokemon);
    printf("Added %s to the pokedex!\n", name);
}

static void do_print(Pokedex pokedex) {
    print_pokedex(pokedex);
}

static void do_details(Pokedex pokedex) {
    Pokemon pokemon = get_current_pokemon(pokedex);
    if (pokemon == NULL) {
        printf("No currently selected pokemon\n");
    } else {
        detail_current_pokemon(pokedex);
    }
}

static void do_insert(Pokedex pokedex, char *line) {
    int pokemon_id;
    double height, weight;
    char name[MAX_LINE] = {0};
    char type1[MAX_LINE] = {0};
    char type2[MAX_LINE] = {0};

    int items_read = sscanf(line, "i %d %s %lf %lf %s %s", &pokemon_id,
        name, &height, &weight, type1, type2);

    if (items_read < 6) {
        printf("Invalid Insert Command\n");
        return;
    }

    Pokemon pokemon = new_pokemon(pokemon_id, name, height, weight,
        type_str_to_code(type1), type_str_to_code(type2));
    insert_pokemon_before_current(pokedex, pokemon);

    printf("Inserted %s into the pokedex!\n", name);
}

static void do_count_type(Pokedex pokedex, char *line) {
    char type[MAX_LINE] = {0};
    int items_read = sscanf(line, "t %s", type);

    if (items_read < 1 || type_str_to_code(type) == NONE_TYPE) {
        printf("Invalid Count Type Command\n");
        return;
    }

    int num = count_pokemon_of_type(pokedex, type);
    printf("%d pokemon of type %s inside the pokedex!\n", num, type);
}

static void do_next(Pokedex pokedex) {
    next_pokemon(pokedex);
}

static void do_prev(Pokedex pokedex) {
    prev_pokemon(pokedex);
}

static void do_jump(Pokedex pokedex, char *line) {
    int pokemon_id;
    if (sscanf(line, "j %d", &pokemon_id) < 1) {
        printf("Invalid Jump Command\n");
        return;
    }
    jump_pokemon(pokedex, pokemon_id);
}

static void do_remove(Pokedex pokedex) {
    remove_pokemon(pokedex);
}

static void do_set_find(Pokedex pokedex, char *line) {
    int found;
    if (sscanf(line, "f %d", &found) < 1 ||
        (found != 1 && found != 0)) {
        printf("Invalid Find Command\n");
        return;
    }
    set_current_pokemon_found(pokedex, found);
    if (found) {
        printf("Set current pokemon to found\n");
    } else {
        printf("Set current pokemon to not found\n");
    }
}

static void do_show_types(Pokedex pokedex) {
    show_types(pokedex);
}

static void do_count_found(Pokedex pokedex) {
    printf("Total Found Pokemon: %d\n", count_found_pokemon(pokedex));
}

static void do_count(Pokedex pokedex) {
    printf("Total Pokemon: %d\n", count_total_pokemon(pokedex));
}

static void do_save(Pokedex pokedex, char *line, int save_load_ability) {
    char filename[MAX_LINE];
    if (sscanf(line, "S %s", filename) != 1) {
        printf("Invalid Save Command\n");
        return;
    } else if (!save_load_ability) {
        printf("Save/Load functionality is DISABLED. To re-enable, quit the program and start again.\n");
        return;
    }
    save_pokedex(pokedex, filename); 
}

static Pokedex do_load(Pokedex pokedex, char *line, int save_load_ability) {
    char filename[MAX_LINE];
    if (sscanf(line, "L %s", filename) != 1) {
        printf("Invalid Load Command\n");
        return NULL;
    } else if (!save_load_ability) {
        printf("Save/Load functionality is DISABLED. To re-enable, quit the program and start again.\n");
        return NULL;
    }
    Pokedex to_return = load_pokedex(filename);
    if (to_return) destroy_pokedex(pokedex);
    return to_return; 
}

static Pokedex do_filter_found(Pokedex pokedex) {
    return get_found_pokemon(pokedex);
}

static Pokedex do_filter_search(Pokedex pokedex, char *line) {
    char search_word[MAX_LINE];
    if (sscanf(line, "R %s", search_word) != 1) {
        printf("Invalid Filter Search Command\n");
        return NULL;
    }
    return search_pokemon(pokedex, search_word);
}

static Pokedex do_filter_type(Pokedex pokedex, char *line) {
    char type[MAX_LINE];
    if (sscanf(line, "T %s", type) != 1
        || type_str_to_code(type) == NONE_TYPE) {
        printf("Invalid Filter Type Command\n");
        return NULL;
    }
    return get_pokemon_of_type(pokedex, type_str_to_code(type));
}

static Pokedex do_filter_end(Pokedex pokedex) {
    return end_search(pokedex);
}

static void do_quit(void) {
    printf("Goodbye.\n");
}