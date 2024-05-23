#include "syncwindows.hpp"

using namespace std;
using namespace homecare;

SyncWindows::SyncWindows() {}

SyncWindows::~SyncWindows() {}

/**
 * Adds a new sync window for the specified patient.
 *
 * @param patient The patient identifier.
 * @param start The start time of the sync window.
 * @param end The end time of the sync window.
 */
void SyncWindows::addSyncWindow(string patient, int start, int end) {
    m_syncWindows[patient] = pair<int, int>(start, end);
}

/**
 * Adds a sync window for the specified patient.
 *
 * @param patient The patient identifier.
 * @param syncWindow A pair representing the start and end of the sync window.
 */
void SyncWindows::addSyncWindow(string patient, pair<int, int> syncWindow) {
    m_syncWindows[patient] = syncWindow;
}

/**
 * Gets the sync window for the specified patient.
 *
 * @param patient The patient identifier.
 * @return The sync window for the specified patient as a pair of start and end times.
 * @throws runtime_error if no sync window is found for the specified patient.
 */
pair<int, int> SyncWindows::getSyncWindow(string patient) const {
    if (m_syncWindows.find(patient) == m_syncWindows.end()) {
        throw runtime_error("Sync window not found for service " + patient);
    }
    else {
        return m_syncWindows.at(patient);
    }
}

int SyncWindows::getOpenSyncTime(string patient) const noexcept(false) {
    if (m_syncWindows.find(patient) == m_syncWindows.end()) {
        throw runtime_error("Sync window not found for service " + patient);
    }
    else {
        return m_syncWindows.at(patient).first;
    }
}

int SyncWindows::getCloseSyncTime(string patient) const noexcept(false) {
    if (m_syncWindows.find(patient) == m_syncWindows.end()) {
        throw runtime_error("Sync window not found for service " + patient);
    }
    else {
        return m_syncWindows.at(patient).second;
    }
}

string SyncWindows::toString() const {
    stringstream ss;
    ss << " Sync Win\n";
    for (const auto &el : m_syncWindows) {
        ss <<el.first <<" -> "<<el.second.first <<" -> "<<el.second.second<< '\n';
    }
    return ss.str();
}