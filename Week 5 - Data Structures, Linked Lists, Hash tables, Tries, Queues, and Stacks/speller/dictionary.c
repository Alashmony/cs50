// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash
// unsigned int letters = 2;
// unsigned int alpha = 26;
// unsigned int alph_let;
const unsigned int N = 676; // 26;//676;

// Hash table
node *table[N];

// Size of the table
unsigned int dict_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Get the word hash
    unsigned int word_hash = hash(word);

    // Get the location on the dictionary
    node *current_node = table[word_hash];

    // If no nodes available
    if (current_node == NULL)
    {
        return false;
    }

    // Iterate over the list of nodes
    while (current_node != NULL)
    {
        // If we found the word
        if (strcasecmp(word, current_node->word) == 0)
        {
            return true;
        }

        // Go to the next node
        current_node = current_node->next;
    }

    // Word not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Get how many sequential letters of a word can be hashed in this size of a dictionary
    // double letters = log((double) N) / log((double) 26);

    // TODO: Improve this hash function
    unsigned int hash_val = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // The location on the hash table, each integer
        hash_val += tolower(word[i]) - 'a';
    }

    return hash_val % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Cannot open dictionary file!\n");
        return false;
    }

    // Create a placeholder to store the word
    char new_word[LENGTH + 1];
    // if (new_word == NULL)
    //{
    //     printf("Cannot allocate memory for a new word!\n");
    //     return false;
    // }

    // Read each word in the file
    while (fscanf(source, "%s", new_word) != EOF)
    {
        // Create a node for the new word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Cannot allocate enough memory for new nodes!\n");
            return false;
        }

        // Add each word to the hash table
        strcpy(new_node->word, new_word);

        new_node->next = NULL;
        unsigned int word_hash = hash(new_word);

        // Check if there is any words already in this letter
        if (table[word_hash] != NULL)
        {
            // Set the new node to point at the first word in the hash table
            new_node->next = table[word_hash];
        }

        // set the hash table to point to the new node for this node
        table[word_hash] = new_node;

        // Increase the size by 1
        dict_size++;
        // free(new_node);
    }

    // Free up spaces allocated for new nodes and words

    // free(new_word);

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *next_node;
    node *current_node;
    // Iterate over the list
    for (int i = 0; i < N; i++)
    {
        // Get the current and next nodes
        next_node = table[i];
        // current_node = *table[i];

        // Free nodes till you get the end of the list
        while (next_node != NULL)
        {
            // Store the current node
            current_node = next_node;
            // Get the next node
            next_node = next_node->next;
            // Free the current node
            free(current_node);
        }
        // free(next_node);
    }
    return true;
}
