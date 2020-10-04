#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int binarysearch(int integers[], int position, int left, int right);
void binaryinsertionsort(int integers[], int length);
void moveElements(int integers[], int start, int stop);

int main(int argc, string argv[]){

    int integers[argc-1];

    for (int i = 0; i < argc - 1; i++){
        integers[i] = atoi(argv[i+1]);
    }

    //moveElements(integers, 0, 5);

    printf("Initial Array: ");
    for(int i=0; i < argc - 1; i++){
        printf("%i",integers[i]);
    }
    printf("\n");

    binaryinsertionsort(integers,argc - 1);



}

void binaryinsertionsort(int integers[], int length){

    //iterate through elements
    //starting at i=1 because first element is considered sorted

    int tempvalue;
    int newposition;

    for(int i = 1; i < length; i++){



        //store element
        tempvalue = integers[i];
        newposition = binarysearch(integers, i, 0, i-1);

        //search for where element should go
        printf("Location %i: %i\n",integers[i],newposition);

        moveElements(integers, newposition, i);

        integers[newposition] = tempvalue;

        printf("Iteration %i: ",i);
        for(int j=0; j < length; j++){
            printf("%i ",integers[j]);
        }

        printf("\n");
    }

    return;

}

//returns new position of value at position.
int binarysearch(int integers[], int position, int left, int right){

    int mid = ((right - left) / 2) + left;
    printf("SEARCH: %i, %i, %i, %i\n", integers[position],left, mid, right);
    printf("SEARCH: %i, %i, %i, %i\n", integers[position],integers[left], integers[mid], integers[right]);

    if( left == right){
        //if left and right are the same, meaning there is only one element, determine if element should go before or after
        if (integers[position] < integers[left]){
            printf("Return Left\n");
            return left;
        } else {
            printf("Return Right\n");
            return left+1;
        }
    } else if ( integers[mid] == integers[position]){
        //if element is the same as mid return mid
        printf("Return Mid\n");
        return mid;
    } else if (integers[position] < integers[mid]){
        //if element is less than mid, binary search the left half
        printf("Search Left: %i, %i\n", left, mid);
        return binarysearch(integers, position, left, mid);
    } else if ( integers[position] > integers[mid]){
        //if element is greater than mid, binary search the right half
        printf("Search Right: %i, %i\n",mid+1, right);
        return binarysearch(integers, position, mid+1, right);
    } else{
        printf("YOU SHOULDN'T BE HERE\n");
        return -1;
    }

}

void moveElements(int integers[], int start, int stop){
    //shift elements up
    //start - element to begin moving
    //stop - last element to be replaced by shift
    printf("Shifting Array: ");
    for(int i=start; i <= stop; i++){
        printf("%i ",integers[i]);
    }
    printf(" ");

    for(int i=stop; i > start; i--){
        integers[i] = integers[i-1];
    }

    printf("Shifted Array: ");
    for(int i=start; i <= stop; i++){
        printf("%i ",integers[i]);
    }
    printf("\n");
}