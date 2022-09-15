//
// DO NOT CHANGE THIS FILE
//
// You do not submit this file. This file is not marked.
// If you think you need to change this file you have
// misunderstood the assignment - ask in the course forum.
//
// Assignment 2 21T2 COMP1511: Pokedex
// ext_save.h
//
// You must not change this file.
//
// Version 1.0.0: Release

#ifndef _SAVE_H_
#define _SAVE_H_


////////////////////////////////////////////////////////////////////////
//  IS VALID SAVE NAME
//
// Return 1 if `name` is a valid save name, and 0 otherwise
// Valid names are [[ numbers, letter   es, underscores ]] ?
int is_valid_save_name(char *name);

////////////////////////////////////////////////////////////////////////
//  SAVE STRING
//
// Save a string to disk calling it "name", overwriting anything already there
void save_string(char *name, char *string);

////////////////////////////////////////////////////////////////////////
//  LOAD STRING
//
// Return the string stored as "name" to the user.
// It is the user's responsibility to free the memory
char *load_string(char *name);


#endif //  _SAVE_H_