#ifndef __ROULETTE_HPP__
#define __ROULETTE_HPP__

#include <vector>
#include <numeric>
#include <random>
#include <iostream>

namespace homecare {

class Roulette {
    private:
        std::vector<double> m_probabilities; //probabilità accumulata
        std::vector<int> m_scores;
        std::vector<int> m_occurrences;
        static const int PAIRS_NUM = 8;

        double generateRandom();

    public:
        static const int RANDOM_GREEDY = 0;
        static const int WORST_GREEDY = 1;
        static const int RELATED_GREEDY = 2;
        static const int CLUSTER_GREEDY = 3;
        static const int RANDOM_REGRET = 4;
        static const int WORST_REGRET = 5;
        static const int RELATED_REGRET = 6;
        static const int CLUSTER_REGRET = 7;

        Roulette();
        ~Roulette();
        void updatePoints(int, int);
        void updateProbabilities();
        int selectPair();

        std::vector<int> getOccurrences() const;
};

}

#endif