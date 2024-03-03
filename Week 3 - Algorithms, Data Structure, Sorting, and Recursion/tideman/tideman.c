#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool find_route(int winner, int looser);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int preffered = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            int not_pref = ranks[j];
            preferences[preffered][not_pref]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair the_pair;
                the_pair.winner = i;
                the_pair.loser = j;
                pairs[pair_count] = the_pair;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pair the_pair;
                the_pair.winner = j;
                the_pair.loser = i;
                pairs[pair_count] = the_pair;
                pair_count++;
            }
        }
    }
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Store the one in this location to be changed if needed
        pair to_change = pairs[i];

        // To compare items
        int higest_pair;
        int max_diff = 0;

        // Iterate starting with the current element not from the start
        for (int j = i; j < pair_count; j++)
        {
            pair current_pair = pairs[j];
            int current_diff =
                preferences[current_pair.winner][current_pair.loser] -
                preferences[current_pair.loser][current_pair.winner];
            if (current_diff > max_diff)
            {
                max_diff = current_diff;
                higest_pair = j;
            }
        }

        // Change the highest pair with the one in the current location
        pairs[i] = pairs[higest_pair];
        pairs[higest_pair] = to_change;
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Iterate over pair of candidates to record the arrows
    for (int i = 0; i < pair_count; i++)
    {
        int win = pairs[i].winner;
        int los = pairs[i].loser;
        if (i == 0)
        {
            locked[win][los] = true;
        }
        else if (!(find_route(los, win)))
        {
            locked[win][los] = true;
        }
    }

    return;
}

bool find_route(int looser, int winner)
{
    if (looser == winner)
    {
        return true;
    }

    // Search indirect path from looser to winner
    // Check if the looser is a winner in another head-to-head
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[looser][i])
        {
            // Check if the one lost is a winner for the winner
            if (find_route(i, winner))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // int win_for[candidate_count];
    // int max_winner = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        int won_for = 0;

        for (int j = 0; j < candidate_count; j++)

            if (!locked[j][i])
            {
                won_for++;
            }

        // win_for[i] = won_for;
        if (won_for == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}
