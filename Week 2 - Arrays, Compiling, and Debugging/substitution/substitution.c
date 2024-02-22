#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string key_caps(string key);
string key_len(string key);
string key_valid(string key);
string encrypt_string(string key, string text);
int char_num(char character);

int main(int argc, string argv[])
{
    string valid = "Valid";

    // Check number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    key_caps(key);

    string key_length = key_len(key);

    if (strcmp(key_length, valid) != 0)
    {
        printf("%s", key_length);
        return 1;
    }

    string key_validation = key_valid(key);
    if (strcmp(key_validation, valid) != 0)
    {
        printf("%s", key_validation);
        return 1;
    }
    // printf("valid key %s\n", key);

    string input = get_string("plaintext:  ");
    string encrypted = encrypt_string(key, input);
    printf("ciphertext: %s\n", encrypted);
}

int char_num(char character)
{
    string alpha = "abcdefghijklmnopqrstuvwxyz";
    string cap_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int char_ind = 0;
    if (islower(character) != 0)
    {
        while (alpha[char_ind] != character)
        {
            char_ind++;
        }
    }
    else if (isupper(character) != 0)
    {
        while (cap_alpha[char_ind] != character)
        {
            char_ind++;
        }
    }
    return char_ind;
}

string key_len(string key)
{
    string valid;
    if (strlen(key) != 26)
    {
        valid = "Key length must be 26!\n";
        return valid;
    }
    else
    {
        valid = "Valid";
        return valid;
    }
}

string key_caps(string key)
{
    // int sm_to_cap = (int) "a" - (int)"A";
    // printf("%i",sm_to_cap);
    for (int m = 0; m < 26; m++)
    {
        if (islower(key[m]) != 0)
        {
            // printf("found lower case %c\n", key[m]);
            key[m] = key[m] - 32;
            // printf("changed to %c\n", key[m]);
        }
    }
    return key;
}

string key_valid(string key)
{
    string valid;
    for (int i = 0; i < 26; i++)
    {
        char cur_letter = key[i];
        if (isalpha(cur_letter) == 0)
        {
            valid = "Invalid charachter in Key\n";
            return valid;
        }
        for (int j = 0; j < 26; j++)
        {
            if (i == j)
            {
                continue;
            }
            else
            {
                char comp_letter = key[j];
                if (cur_letter == comp_letter)
                {
                    valid = "Duplication in the key letters\n";
                    return valid;
                }
            }
        }
    }
    valid = "Valid";
    return valid;
}

string encrypt_string(string key, string text)
{
    string encrypted = text;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        char cur_letter = text[i];
        int cur_index = char_num(cur_letter);
        char new_val;
        if (isupper(cur_letter) != 0)
        {
            new_val = key[cur_index];
        }
        else if (islower(cur_letter) != 0)
        {
            new_val = tolower(key[cur_index]);
        }
        else
        {
            new_val = cur_letter;
        }
        encrypted[i] = new_val;
    }

    return encrypted;
}
