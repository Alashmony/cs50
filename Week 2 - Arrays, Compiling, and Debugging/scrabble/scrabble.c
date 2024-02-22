#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Prompt the user for two words
    string p1_input = get_string("Player 1: ");
    string p2_input = get_string("Player 2: ");

    // Compute the score of each word
    int p1_score = compute_score(p1_input);
    int p2_score = compute_score(p2_input);

    // Print the winner
    if (p1_score > p2_score)
    {
        printf("Player 1 wins!\n");
    }
    else if (p1_score < p2_score)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Keep track of score
    int word_value = 0;

    // Compute score for each character
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        if (isupper(word[i]))
        {
            word_value += POINTS[word[i] - 'A'];
        }
        else if (islower(word[i]))
        {
            word_value += POINTS[word[i] - 'a'];
        }
    }

    return word_value;
}
