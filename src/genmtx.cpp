#include <iostream>
#include <random>
#include <cstdlib>
#include <iomanip>
#include <chrono>

// function to generate and a function to write in a file

// ------------------------------------------------------------------------------------------------
// Gen Random Number 
// ------------------------------------------------------------------------------------------------
int gen_number(int range_size, int seed){
    
    std::mt19937 gen( seed );

    std::uniform_int_distribution<int> distributor(0, range_size);

    return distributor( gen );
}
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gen Matrix
// ------------------------------------------------------------------------------------------------
int ** gen_matrix(int n, int m){
    
    // Allocation
    int ** matrix = new int * [n];

    for(int i = 0; i < m; i++){
        matrix[i] = new int [m];
    }
    //----------------------------

    for(int row = 0; row < n; row++){
        for(int col = 0; col < m; col++){
            matrix[row][col] = gen_number((n*m), (row*col));
        }
    }


    return matrix;
}
// ------------------------------------------------------------------------------------------------

int main(int argc, char * argv[]){

    int n1, m1, n2, m2;

    std::cout << "Hello, SO, i'll defeat you this time motherfucker!\n";

    return 0;
}
