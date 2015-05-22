#ifndef MONITOR_H_
#define MONITOR_H_

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

struct MonitorPoint {
  unsigned int appendData(unsigned int time, bool value);

  std::string nickname;
  std::vector<std::string> signature;
  std::vector<std::pair<unsigned int, bool> > log;
};

class Monitor {
  public:
    Monitor();
    ~Monitor();

    void setValue(unsigned int pointId, unsigned int time, bool value);

    const unsigned int addPoint(std::vector<std::string> signature);
    const unsigned int addPoint(std::vector<std::string> signature, std::string nickname);

    void renamePoint(unsigned int pointId, std::string newName);
    void renamePoint(std::string oldName, std::string newName);

    const std::vector<std::pair<unsigned int, bool> >& getLog(unsigned int pointId);
    const std::vector<std::pair<unsigned int, bool> >& getLog(std::string nickname);
    const std::vector<std::pair<unsigned int, bool> >& getLog(std::vector<std::string> signature);

    unsigned int findPoint(std::string nickname);
    unsigned int findPoint(std::vector<std::string> signature);
    unsigned int maxLength;

  private:
    unsigned int nextId;
    std::map<unsigned int, MonitorPoint*> _points;
};

#endif
