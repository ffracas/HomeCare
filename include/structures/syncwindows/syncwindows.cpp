#include "syncwindows.hpp"

using namespace std;
using namespace homecare;

SyncWindows::SyncWindows(/* args */) {}

SyncWindows::~SyncWindows() {}

void SyncWindows::addSyncWindow(string patient, int start, int end) {
    m_syncWindows[patient] = pair<int, int>(start, end);
}

void SyncWindows::addSyncWindow(string patient, pair<int, int> syncWindow) {
    m_syncWindows[patient] = syncWindow;
}

pair<int, int> SyncWindows::getSyncWindow(string patient) const {
    if (m_syncWindows.find("f") == m_syncWindows.end()) {
        throw runtime_error("Sync window not found for service " + patient);
    } else {
        return m_syncWindows.at(patient);
    }
}
