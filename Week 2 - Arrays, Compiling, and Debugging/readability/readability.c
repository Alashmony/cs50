#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
string sen_end = ".?!";

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // printf("Letters are %i, words are %i, and sentenses are %i\n",letters, words, sentences);

    // Compute the Coleman-Liau index

    // L is the average number of letters per 100 words in the text: that is, the number of letters divided by the number of words,
    // all multiplied by 100.
    float l = ((float) letters / (float) words) * 100.0;

    // S is the average number of sentences per 100 words in the text: that is, the number of sentences divided by the number of
    // words, all multiplied by 100.
    float s = ((float) sentences / (float) words) * 100.0;

    // index = 0.0588 * L - 0.296 * S - 15.8
    float col_liau = (0.0588 * l) - (0.296 * s) - 15.8;

    // Print the grade level
    int grade = round(col_liau);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    // Track the letters count
    int letters_count = 0;
    // iterate over the sentence
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Check if the letter is an alphabit
        if (isalpha(text[i]))
        {
            letters_count++;
        }
    }
    return letters_count;
}

int count_words(string text)
{
    // Track the number of words - start with 1 to calculate the last word
    int words_count = 1;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isspace(text[i]) != 0)
        {
            words_count++;
        }
    }
    return words_count;
}

int count_sentences(string text)
{
    int sent_count = 0;
    // Return the number of sentences in text
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        char cur_letter = text[i];
        if (cur_letter == sen_end[0] || cur_letter == sen_end[1] || cur_letter == sen_end[2])
        // if ispunct(cur_letter)
        {
            sent_count++;
        }
    }
    return sent_count;
}
