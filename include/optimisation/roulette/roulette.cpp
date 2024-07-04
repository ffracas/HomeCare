#include "roulette.hpp"

using namespace std;
using namespace homecare;

Roulette::Roulette() {
    for (int i = 0; i < PAIRS_NUM; i++) {
        m_scores.push_back(1);
        m_prec_scores.push_back(1);
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
    vector<int> weight;
    for (int i = 0; i < m_scores.size(); ++i) {
        weight.push_back(m_prec_scores[i] * (1 - m_r) + m_r * m_scores[i] / max(m_occurrences[i], 1));
    }
    int totScore = accumulate(weight.begin(), weight.end(), 0);
    m_probabilities.clear();
    m_probabilities.push_back(weight[0] / totScore);
    for (int i = 1; i < PAIRS_NUM; ++i) {
        m_probabilities.push_back(m_probabilities[i - 1] + (m_scores[i] / (double) totScore));
    }
    for (int i = 0; i < PAIRS_NUM; ++i) {
        m_prec_scores[i] = m_scores[i];
        m_occurrences[i] = 0;
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