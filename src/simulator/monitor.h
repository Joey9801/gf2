#ifndef MONITOR_H_
#define MONITOR_H_

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>

struct MonitorPoint {
  MonitorPoint();

  void appendData(unsigned int time, bool value);

  std::string nickname;
  std::vector<std::string> signature;
  std::vector<std::pair<unsigned int, bool> > log;

  bool _lastValue;
};

class Monitor {
  public:
    Monitor();
    ~Monitor();

    template <typename T>
      unsigned int removePoint(T ident) {
        unsigned int pointId = findPoint(ident);
        delete _points[pointId];
        _points.erase(pointId);

        return pointId;
      }

    template <typename T>
      const std::vector<std::pair<unsigned int, bool> >& getLog(T ident) {
        unsigned int pointId = findPoint(ident);
        return _points[pointId]->log;
      }

    void setValue(unsigned int pointId, unsigned int time, bool value);

    const unsigned int addPoint(std::vector<std::string> signature);
    const unsigned int addPoint(std::vector<std::string> signature, std::string nickname);

    void renamePoint(unsigned int pointId, std::string newName);
    void renamePoint(std::string oldName, std::string newName);

    unsigned int numPoints(void);
    std::vector<unsigned int> getPoints(void);
    std::string getNickname(unsigned int pointId);
    std::vector<std::string> getSignature(unsigned int pointId);

    unsigned int findPoint(unsigned int pointId);
    unsigned int findPoint(std::string nickname);
    unsigned int findPoint(std::vector<std::string> signature);
    unsigned int maxTime;

    void clearAllData();
  private:
    unsigned int nextId;
    std::map<unsigned int, MonitorPoint*> _points;
};

#endif
