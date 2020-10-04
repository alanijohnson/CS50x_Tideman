#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
}
pair;

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
void print_winner(void);
int binarysearch( int position, int left, int right);
void binaryinsertionsort( int length);
void moveElements( int start, int stop);
int getMargin(pair pair);
bool checkCycle(int winner, int initialLoser);

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
    //save rankings for each voter
    //search for candidates name and save the ranking
    //error checking - return false if incorrect name was selected
    for( int i= 0; i < candidate_count; i++){
        //if name is found add to ranks array and then stop looking for string
        if( strcmp(name,candidates[i]) == 0){
            ranks[rank] = i;
            return true;
        }

    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // printf("Recording Preferences:\n");
    // for(int i=0; i<candidate_count; i++){
    //     printf("Rank %i: %i\n",i+1,ranks[i]);
    // }

    // Upadate preferences between pairs of candidates
    // Iterate through ranks array and update preference for each possible pair
    for(int i=0; i < candidate_count; i++){
        for(int j=i+1; j < candidate_count; j++){
            preferences[ranks[i]][ranks[j]]++;
            //printf("%s vs %s increased count\n", candidates[ranks[i]], candidates[ranks[j]]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    //printf("\nAdd Pairs\n");
    //pair_count = pair_counter(candidate_count);

    // initialize count for pairs - pair count isn't the same as the possible number of pairs.
    int pairticker = 0;

    // iterate through candidate combinations and determine if winner exists.
    // if so store to the array pairs and declare winner and loser of the pair.
    for(int i=0; i < candidate_count; i++){
        for(int j=i+1; j < candidate_count; j++){
            if(preferences[i][j] > preferences[j][i]){
                pairs[pairticker].winner = i;
                pairs[pairticker].loser = j;
                pairticker++;
                //printf("Pair %i: winner - %s loser - %s\n",pairticker,  candidates[pairs[pairticker].winner], candidates[pairs[pairticker].loser]);
            } else if(preferences[i][j] < preferences[j][i]) {
                pairs[pairticker].winner = j;
                pairs[pairticker].loser = i;
                pairticker++;
                //printf("Pair %i: winner - %s loser - %s\n",pairticker,  candidates[pairs[pairticker].winner], candidates[pairs[pairticker].loser]);
            } else {
                //printf("%s vs %s: PAIR NOT ADDED - TIE\n", candidates[i], candidates[j]);
            }




        }
    }

    // final pair_count is pairticker. Pairticker started at 0, so extra increment makes it true total
    pair_count = pairticker;

    /*printf("PAIRS ADDED\n");
    for(int i=0; i<pair_count; i++){
        printf("Pair %i: %s vs %s ",i, candidates[pairs[i].winner],candidates[pairs[i].loser]);
    }*/

}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    // using binary insertion sort - the number of pairs increases quickly for the number of candidates added
    // Max number of candidates is set to nine currently, thus max pairs is 36.
    // If Max number of candididates increases beyond 12 (pairs = 66), then timsort should be used instead of binary insertion sort

    binaryinsertionsort(pair_count);

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // initialize variables
    int winner;
    int loser;

    // iterate through pairs to add edges
    for(int i=0; i<pair_count; i++){
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        // check to ensure cycle isn't created before adding edge
        if(!checkCycle(winner,loser)){
            locked[winner][loser] = true;
        }

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // search for the winner by iterating through contestants and seeing if there is no edge pointing towards them
    // i - candidate to be declared winner if they are never the loser
    // j - opposing candidates
    int winner;
    for(int i=0; i < candidate_count; i++){
        winner = i;
        // determine if current candidate is winner
        // if not increment winner
        for(int j=0; j < candidate_count; j++){
            if( locked[j][i] == true){
                // move onto the next candidate
                winner++;
                break;
            }

        }

        // if winner wasn't incremented, winner found
        if(winner == i){
            printf("%s\n", candidates[i]);
        }
    }

    return;
}

void binaryinsertionsort( int length){

    //iterate through elements
    //starting at i=1 because first element is considered sorted

    pair tempPair;
    int newposition;

    for(int i = 1; i < length; i++){

        //store element
        tempPair = pairs[i];
        // search for elements new location
        newposition = binarysearch( i, 0, i-1);


        //printf("Location %i: %i\n",i,newposition);

        // reposition elements
        moveElements( newposition, i);
        pairs[newposition] = tempPair;

        //printf("Iteration %i:\n",i);
        //for(int j=0; j < length; j++){
           // printf("%s vs %s: %i\n",candidates[pairs[j].winner], candidates[pairs[j].loser], getMargin(pairs[j]));
        //}

        //printf("\n");
    }

    return;

}


int binarysearch( int position, int left, int right){
    // returns new position of value at position.
    // written to use the scores in the preferences array
    // sorts high to low
    // to sort low to high switch directionality of comparators

    // sorts high to low

    int mid = ((right - left) / 2) + left;
    //printf("SEARCH: %i, %i, %i, %i\n", position,left, mid, right);
    //printf("SEARCH: %i, %i, %i, %i\n", getMargin(pairs[position]),getMargin(pairs[left]), getMargin(pairs[mid]), getMargin(pairs[right]));

    if( left == right){
        // if left and right are the same, meaning there is only one element, determine if element should go before or after
        if (getMargin(pairs[position]) > getMargin(pairs[left])){
            //printf("Return Left\n");
            return left;
        } else {
            //printf("Return Right\n");
            return left+1;
        }
    } else if ( getMargin(pairs[mid]) == getMargin(pairs[position])){
        // if element is the same as mid return mid
        //printf("Return Mid\n");
        return mid;
    } else if (getMargin(pairs[position]) > getMargin(pairs[mid])){
        // if element is less than mid, binary search the left half
        //printf("Search Left: %i, %i\n", left, mid);
        return binarysearch( position, left, mid);
    } else if ( getMargin(pairs[position]) < getMargin(pairs[mid])){
        // if element is greater than mid, binary search the right half
        //printf("Search Right: %i, %i\n",mid+1, right);
        return binarysearch( position, mid+1, right);
    } else{
        //printf("YOU SHOULDN'T BE HERE\n");
        return -1;
    }

}

void moveElements( int start, int stop){
    // shift elements in the array
    // helper function for binary insertion sort
    // start - element to begin moving
    // stop - last element to be replaced by shift

    for(int i=stop; i > start; i--){
        pairs[i] = pairs[i-1];
    }
}

int getMargin(pair pair){
    // get the margin of a pair
    return abs(preferences[pair.winner][pair.loser] - preferences[pair.loser][pair.winner]);
}

// checks for cycles in directed graph
// cycle doesn't exist yet in graph
bool checkCycle(int winner, int initialLoser){

    // check to see if you reach the initial loser node.
    // reaching this node indicates adding the edge will result in a cycle
    if( winner == initialLoser){
        return true;
    }

    // iterate through all the candidates and determine if the current winner was beat by anyone else
    // if they were beat, see if there is a cycle present
    for(int i=0; i<candidate_count; i++){
        if( locked[i][winner] == true){
            if( checkCycle(i,initialLoser) == true){
                return true;
            }
        }
    }

    return false;

}