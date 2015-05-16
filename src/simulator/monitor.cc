#include "monitor.h"

void Monitor::setValue(unsigned int pointId, bool value) {
  _log[pointId].push_back(value);
}

unsigned int Monitor::addMonitorPoint(void) {
  unsigned int pointId = _log.size();
  _log.push_back(std::vector<bool>());
  return pointId;
}

std::vector<bool> Monitor::getLog(unsigned int pointId) {
  std::vector<bool> l = _log[pointId];
  return l;
}
