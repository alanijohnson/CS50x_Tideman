#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int pair_counter(int elementCount);

int main(int argc, string argv[]){

   printf("%i\n", pair_counter(atoi( argv[1])));
}

int pair_counter(int elementCount){
    int count = 0;

    if( elementCount == 1 ){
        return 0;
    }

    count = (elementCount - 1) + pair_counter(elementCount - 1);

    return count;

}