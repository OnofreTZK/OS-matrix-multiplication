#include <fstream>
#include <iostream>
#include <random>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <string>

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
int ** gen_matrix(int row_size, int col_size){
    
    // Allocation
    int ** matrix = new int * [row_size];

    for(int i = 0; i < row_size; i++){
        matrix[i] = new int [col_size];
    }
    //----------------------------

    for(int row = 0; row < row_size; row++){
        for(int col = 0; col < col_size; col++){
            matrix[row][col] = gen_number((row_size*col_size), (row*col));
        }
    }

    return matrix;
}
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Write Matrix
// ------------------------------------------------------------------------------------------------
bool write_matrix(int ** matrix, std::string id, int row_size, int col_size){
    
    std::stringstream file_name; 

    file_name << "../matrices/matrix_" << id << ".dat";
    
    std::ofstream output_file;

    output_file.open(file_name.str());
    
    if(!output_file){
        std::cerr << "Unable to open file! Please certify you had given the correct directory path to "
                  << "a valid config file( .dat or .txt )" << std::endl;

        return false;

    } else {
        
        // header row col
        output_file << row_size << " " << col_size << std::endl; 

        for(int row = 0; row < row_size; row++){
            for(int col = 0; col < col_size; col++){
                output_file << matrix[row][col] << std::endl; 
            }
        }
        return true;
    }

}
// ------------------------------------------------------------------------------------------------

int main(int argc, char * argv[]){

    if(argc < 5){
        std::cerr << "Correct usage: ./genmtx <row_size_matrix_1>" 
                  << " <col_size_matrix_1> <row_size_matrix_2> <col_size_matrix_2>" << std::endl;

        return EXIT_FAILURE;
    }

    int n1, m1, n2, m2;

    n1 = atoi(argv[1]);
    m1 = atoi(argv[2]);
    n2 = atoi(argv[3]);
    m2 = atoi(argv[4]);

    int ** mat_1 = gen_matrix(n1, m1);
    int ** mat_2 = gen_matrix(n2, m2);

    if(write_matrix(mat_1, "01", n1, m1)){
        if(write_matrix(mat_2, "02", n2, m2)){
            std::cout << "Matrices successful generated!" << std::endl;
            return EXIT_SUCCESS;
        }
    }
    // else
    std::cerr << "Something goes wrong on generation..." << std::endl;
    return EXIT_FAILURE;
}
