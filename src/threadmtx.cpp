#include <iostream>
#include <pthread.h>
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
    long int ** matrix;
} Matrix;

typedef struct{
    int * id;
    int num_threads;
    Matrix mat_01;
    Matrix mat_02;
} Args;

// Global final matrix
Matrix product;


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
        mat.matrix = new long int * [row_size];

        for(int i = 0; i < row_size; i++){
            mat.matrix[i] = new long int [col_size];
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
// Write Thread Time
// ------------------------------------------------------------------------------------------------
void register_thread_time(double time, int num_threads, int thr, int size){

    std::stringstream file_name;

    file_name << "../matrices/matrix_" << size << "x" << size << "_threads_" << num_threads 
              << ".dat";

    std::ofstream output_file;

    output_file.open(file_name.str(), std::ofstream::out | std::ofstream::app);

    if(!output_file){
        std::cerr << "Unable to open file! Please certify you had given the correct directory path to "
                  << "a valid config file( .dat or .txt )" << std::endl;

        exit(1);
    } else {
        output_file << "thread: " << thr << "| Time: " << time << std::endl; 
        
        output_file.close();
    }
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Multiplication
// ------------------------------------------------------------------------------------------------
void * multiplication(void * args){

    int sum;
    Matrix mat_01 = ((Args *) args)->mat_01;
    Matrix mat_02 = ((Args *) args)->mat_02;

    // Vars for threads
    int local_thread_id = *(((Args *) args)->id);
    int num_threads = ((Args *) args)->num_threads;
    int slice = (mat_01.row_size / num_threads);
    int start = local_thread_id * slice;
    int end = (local_thread_id+1) * slice;

 
    std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
    for (int row = start; row < end; row++){
        for (int col = 0; col < mat_01.col_size; col++){
            sum = 0;
            for (int aux = 0; aux < mat_01.row_size; aux++){
                sum += mat_01.matrix[row][aux] * mat_02.matrix[aux][col];
            }
            
            product.matrix[row][col] = sum;
        }
    }
    std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();
    
    auto timer = (STOP - START);

    double final_time = std::chrono::duration<double> (timer).count();

    register_thread_time(final_time, num_threads, local_thread_id, mat_01.row_size);

    return 0;

}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Write Matrix
// ------------------------------------------------------------------------------------------------
bool write_matrix(long int ** matrix, std::string id, int row_size, int col_size, 
        int num_threads, double time){
    
    std::stringstream file_name; 

    file_name << "../matrices/matrix_" << id << "_" << num_threads << "_" << row_size <<".dat";
    
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
        
        output_file << time << std::endl;

        output_file.close();

        return true;
    }

}
// ------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    if(argc < 4){
        std::cerr << "Correct usage: ./threadmtx <num_threads>"
                  << "<matrix_01_file> <matrix_02_file>" << std::endl;

        return EXIT_FAILURE; 
    }
    
    std::pair<Matrix, Matrix> matrices = get_matrices(argv[2], argv[3]);

    int num_threads = atoi(argv[1]);
    pthread_t * threads = new pthread_t[num_threads];
    
    // Treating limitations
    if (matrices.first.row_size % num_threads != 0){
        std::cerr << "Due to limitations of time, this program only accept square matrices!\n";
        return EXIT_FAILURE;
    }
    
    // Allocating the matrix
    // ----------------------------------------------------------
    product.matrix = new long int * [matrices.first.row_size];

    for(int i = 0; i < matrices.first.row_size; i++){
        product.matrix[i] = new long int [matrices.first.col_size];
    }
    
    product.row_size = matrices.first.row_size;
    product.col_size = matrices.first.col_size;
    // ----------------------------------------------------------
   
    std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
    // Starting routines
    for (int thr = 0; thr < num_threads; ++thr){
        int * thread_id;
        thread_id = new int;
        *thread_id = thr;

        Args * arguments = new Args;
        arguments->id = thread_id;
        arguments->num_threads = num_threads;
        arguments->mat_01 = matrices.first;
        arguments->mat_02 = matrices.second;
        pthread_create(&threads[thr], NULL, multiplication, (void*) arguments);
    }

    // Starting threads
    for (int thr = 0; thr < num_threads; ++thr){
        pthread_join(threads[thr], NULL);
    }
    std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();

    auto timer = (STOP - START);

    double final_time = std::chrono::duration<double> (timer).count();

    if(write_matrix(product.matrix, "thread", product.row_size, product.col_size, 
                num_threads, final_time)){
        std::cout << product.row_size << " " << final_time << std::endl;
        delete product.matrix;
        return EXIT_SUCCESS;
    }
    // else
    std::cerr << "Something goes wrong on writing..." << std::endl;
    return EXIT_FAILURE;

}
