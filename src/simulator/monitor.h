#ifndef MONITOR_H_
#define MONITOR_H_

#include <vector>

class Monitor {
  friend class RootNetwork;
  public:
    void setValue(unsigned int pointId, bool value);
    unsigned int addMonitorPoint(void);
    std::vector<bool> getLog(unsigned int pointId);
  private:
    std::vector<std::vector<bool> > _log;
};

#endif
