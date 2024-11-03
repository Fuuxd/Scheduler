#include "config.h"
#include "node.h"
#include "semesterVector.h"
#include "toposorts.h"
#include "layeredBinning.h"
#include "bruteForce.h"

bool nextPermutationIndicesHere(std::vector<std::vector<size_t>>& indices) {
    for (size_t i = 0; i < indices.size(); ++i) {
        if (std::next_permutation(indices[i].begin(), indices[i].end())) {
            return true;
        } else {
            std::sort(indices[i].begin(), indices[i].end());
        }
    }
    return false;
}


int main(){
    std::vector<std::vector<size_t>> indices = {{0, 1}, {0, 1, 2}};

    do{
        printVectorOfVectors(indices);
    } while( nextPermutationIndicesHere(indices));
}