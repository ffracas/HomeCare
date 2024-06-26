#include "roulette.hpp"

using namespace std;
using namespace homecare;

Roulette::Roulette() {
    for (int i = 0; i < PAIRS_NUM; i++) {
        m_scores.push_back(1);
        m_occurrences.push_back(0);
    }
    updateProbabilities();
}

Roulette::~Roulette() {}

double Roulette::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

void Roulette::updatePoints(int pairIndex, int score) {
    if (pairIndex < 0 || pairIndex >= PAIRS_NUM || score < 0) {
        throw invalid_argument("pairIndex out of bounds");
    }
    m_scores[pairIndex] += score;
}

void Roulette::updateProbabilities() {
    int totScore = accumulate(m_scores.begin(), m_scores.end(), 0);
    m_probabilities.clear();
    m_probabilities.push_back(m_scores[0] / totScore);
    for (int i = 1; i < PAIRS_NUM; ++i) {
        m_probabilities.push_back(m_probabilities[i - 1] + (m_scores[i] / (double) totScore));
    }
}

int Roulette::selectPair() {
    double random = generateRandom();
    for (int i = 0; i < PAIRS_NUM; ++i) {
        if (random < m_probabilities[i]) {
            m_occurrences[i]++;
            return i;
        }
    }
    m_occurrences[CLUSTER_REGRET]++;
    return CLUSTER_REGRET;
}

vector<int> Roulette::getOccurrences() const {
    return m_occurrences;
}