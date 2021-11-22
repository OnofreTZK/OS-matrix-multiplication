#include <iostream>
#include <random>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>

// Custom type matrix
typedef struct{
    int row_size;
    int col_size;
    int ** matrix;

} Matrix;


// ------------------------------------------------------------------------------------------------
// Read File
// ------------------------------------------------------------------------------------------------
Matrix read_file(std::string file_name){
    
    std::ifstream input_file;
    std::string buffer;
    std::stringstream ss;
    Matrix mat;
    
    input_file.open(file_name);

    if(!input_file){
        std::cerr << "Unable to open file! Please certify you had given the correct directory path to "
                  << "a valid config file( .dat or .txt )" << std::endl;
        
        exit(1);
    } else {
       
        // buffering each line to the stream
        while(std::getline(input_file, buffer)){
            ss << buffer << std::endl;
        }

        input_file.close();

        int row_size, col_size;

        ss >> row_size >> col_size;

        mat.row_size = row_size;
        mat.col_size = col_size;

        // Allocating the matrix
        mat.matrix = new int * [row_size];

        for(int i = 0; i < row_size; i++){
            mat.matrix[i] = new int [col_size];
        }
        // ---------------------------------

        for(int row = 0; row < row_size; row++){
            for(int col = 0; col < col_size; col++){
                ss >> mat.matrix[row][col]; 
            }
        }

        return mat;
    }
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Get Matrices
// ------------------------------------------------------------------------------------------------
std::pair<Matrix, Matrix> get_matrices(std::string file_01, std::string file_02){

    Matrix mat_01 = read_file(file_01);
    Matrix mat_02 = read_file(file_02);

    return std::make_pair(mat_01, mat_02);

}
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Multiplication
// ------------------------------------------------------------------------------------------------
Matrix multiplication(Matrix mat_01, Matrix mat_02){

    int sum;
    Matrix product;

    // Allocating the matrix
    product.matrix = new int * [mat_01.row_size];

    for(int i = 0; i < mat_01.row_size; i++){
        product.matrix[i] = new int [mat_01.col_size];
    }
    // ----------------------------------------------------
    
    product.row_size = mat_01.row_size;
    product.col_size = mat_01.col_size;
    
    for (int row = 0; row < mat_01.row_size; row++){
        for (int col = 0; col < mat_01.col_size; col++){
            sum = 0;
            for (int aux = 0; aux < mat_01.row_size; aux++){
                sum += mat_01.matrix[row][aux] * mat_02.matrix[aux][col];
            }
            
            product.matrix[row][col] = sum;
        }
    }

    delete mat_01.matrix;
    delete mat_02.matrix;
    return product;

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
        
        output_file.close();

        return true;
    }

}
// ------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    if(argc < 3){
        std::cerr << "Correct usage: ./serialmtx <matrix_01_file> <matrix_02_file>" << std::endl;

        return EXIT_FAILURE; 
    }
    
    std::pair<Matrix, Matrix> matrices = get_matrices(argv[1], argv[2]);

    Matrix product = multiplication(matrices.first, matrices.second);

    if(write_matrix(product.matrix, "serial", product.row_size, product.col_size)){
        std::cout << "Matrices successful generated!" << std::endl;
        delete product.matrix;
        return EXIT_SUCCESS;
    }
    // else
    std::cerr << "Something goes wrong on writing..." << std::endl;
    return EXIT_FAILURE;

}
