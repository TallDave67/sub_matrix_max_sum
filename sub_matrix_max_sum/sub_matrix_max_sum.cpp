#include "sub_matrix_max_sum.h"

#include <vector>
#include <random>
#include <tuple>
#include <map>
#include <limits.h>

std::vector<std::vector<int>> g_mat;
std::map<std::tuple<int, int, int, int>, int> g_sub_mat_sums;

// Function to print the elements of a tuple
template<typename Tuple, std::size_t N>
struct TuplePrinter {
    static void print(const Tuple& t) {
        TuplePrinter<Tuple, N-1>::print(t);
        std::cout << ", " << std::get<N-1>(t);
    }
};

template<typename Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple& t) {
        std::cout << std::get<0>(t);
    }
};

template<typename... Args>
void printTuple(const std::tuple<Args...>& t) {
    std::cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    std::cout << ")";
}

void initializeMatrix(std::vector<std::vector<int>>& matrix, int N) {
    // Resize the matrix to N x N
    matrix.resize(N, std::vector<int>(N));

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(-N * 100, N * 100);

    // Fill the matrix with random values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = distribution(gen);
        }
    }
}

// sum[t, b, l, r] = sum(0, b, 0, r) - (sum[0, t-1, 0, r] + sum[0, b, 0, l-1]) + sum(0, t-1, 0, l-1)
int computeSubMatrixSum(int top, int bottom, int left, int right)
{
    std::tuple<int, int, int, int> key { top, bottom, left, right };
    int value = g_sub_mat_sums[{0, bottom, 0, right}]; // so far up to sub-matrix

    int num_subtractions = 0;
    if (top != 0)
    {
        // subtract out sub-matrix above
        value -= g_sub_mat_sums[{0, top - 1, 0, right}];
        num_subtractions++;
    }
    if (left != 0)
    {
        // subtract out sub-matrix to the left
        value -= g_sub_mat_sums[{0, bottom, 0, left - 1}];
        num_subtractions++;
    }

    // compensate for sub-matrix subracted out twice
    if (num_subtractions == 2)
    {
        value += g_sub_mat_sums[{0, top - 1, 0, left - 1}];
    } 
    g_sub_mat_sums.insert({ key,value });
    return value;
}

// sum[0, b, 0, r] = sum(0, b-1, 0, r) + row_sum)
int computeOriginSubMatrixSum(int bottom, int right, int row_sum)
{
    std::tuple<int, int, int, int> key { 0, bottom, 0, right };
    int value_above = 0;
    if (bottom > 0)
    {
        value_above = g_sub_mat_sums[{0, bottom - 1, 0, right}];
    }
    int value = value_above + row_sum;
    g_sub_mat_sums.insert({ key,value });
    return value;
}

std::tuple<std::tuple<int, int, int, int>, int> getMaxSubMatrixSum(int N)
{
    std::tuple<int, int, int, int> max_key { 0, 0, 0, 0 };
    int max_sum = INT_MIN;

    // Firstly compute sum for all the sub-matrices whose (top, left) is (0, 0)
    // these are the origin sub-matrices
    for(int i = 0; i < N; i++)
    {
        int row_sum = 0;
        for(int j = 0; j < N; j++)
        {
            row_sum += g_mat[i][j];
            int sub_sum = computeOriginSubMatrixSum(i, j, row_sum);
            if (sub_sum > max_sum)
            {
                max_key = { 0, i, 0, j };
                max_sum = sub_sum;
            }
        }
    }

    // Print out all sub_matrix sums
    std::cout << "******************* ORIGIN " << g_sub_mat_sums.size() << " *******************" << std::endl;
    for (const auto& sum : g_sub_mat_sums)
    {
        std::cout << "key = ";
        printTuple(std::get<0>(sum));
        std::cout << ", sum = " << std::get<1>(sum) << std::endl;
    }

    // Secondly compute sum for all the sub-matrices in a given origin sub-matrix
    // whose (bottom, right) is always the (bottom, right) of the origin sub-matrix
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (!(i == 0 && j == 0))
            {
                int bottom = i;
                int right = j;
                for (int top = 0; top <= bottom; top++)
                {
                    for (int left = 0; left <= right; left++)
                    {
                        int sub_sum = computeSubMatrixSum(top, bottom, left, right);
                        if (sub_sum > max_sum)
                        {
                            max_key = { top, bottom, left, right };
                            max_sum = sub_sum;
                        }
                    }
                }
            }
        }
    }

    return { max_key, max_sum };
}

// Test the function
int main() {
    int N = 5; // Change N to the desired size of the matrix
    initializeMatrix(g_mat, N);

    // Print the initialized matrix
    std::cout << "******************* " << N << " by " << N << " *******************" << std::endl;
    for (const auto& row : g_mat) {
        for (int val : row) {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }

    // Get the sub-matrix with the max sum
    std::tuple<std::tuple<int, int, int, int>, int> max = getMaxSubMatrixSum(N);

    // Print out all sub_matrix sums
    std::cout << "******************** ALL " << g_sub_mat_sums.size() << " ********************" << std::endl;
    for (const auto& sum : g_sub_mat_sums)
    {
        std::cout << "key = ";
        printTuple(std::get<0>(sum));
        std::cout << ", sum = " << std::get<1>(sum) << std::endl;
    }

    // Print out the max
    std::cout << "==================== MAX of " << g_sub_mat_sums.size() << " ====================" << std::endl;
    std::cout << "key = ";
    printTuple(std::get<0>(max));
    std::cout << ", sum = " << std::get<1>(max) << std::endl;

    return 0;
}
