#ifndef PORTAGENS_SEQUENTIALSEARCH_H
#define PORTAGENS_SEQUENTIALSEARCH_H

#include <vector>

using namespace std;

///Searches for pointer to Comparable in vector of pointers to Comparable
///
/// \tparam Comparable
/// \param v vector
/// \param x element to find
/// \return index, if found or -1, otherwise
template<class Comparable>
int sequentialSearch(const vector<Comparable *> &v, const Comparable *x) {
    if (!v.empty())
        for (unsigned int i = 0; i < v.size(); i++)
            if (*v[i] == *x)
                return i;   // encontrou
    return -1;     // não encontrou
}

#endif //PORTAGENS_SEQUENTIALSEARCH_H
