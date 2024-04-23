#ifndef __SYNC_WINDOWS_HPP__
#define __SYNC_WINDOWS_HPP__

#include <map>
#include <sstream>

namespace homecare {

class SyncWindows {
private:
    std::map<std::string, std::pair<int, int>> m_syncWindows;
public:
    SyncWindows();
    ~SyncWindows();

    // setter
    void addSyncWindow(std::string, int, int);
    void addSyncWindow(std::string, std::pair<int, int>);
    // getter
    std::pair<int, int> getSyncWindow(std::string) const noexcept(false);
};

}

#endif