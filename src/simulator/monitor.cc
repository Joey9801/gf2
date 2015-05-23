#include "monitor.h"

unsigned int MonitorPoint::appendData(unsigned int time, bool value) {
  log.push_back(std::pair<unsigned int, bool>(time, value));
  return log.size();
}

Monitor::Monitor() :
  maxLength(0),
  nextId(0)
{
}

Monitor::~Monitor() {
  for(std::map<unsigned int, MonitorPoint*>::iterator it = _points.begin();
      it != _points.end();
      it++) {
    delete it->second;
  }
}

unsigned int Monitor::findPoint(unsigned int pointId) {
  if(_points.find(pointId) == _points.end())
    throw 1;

  return pointId;
}
unsigned int Monitor::findPoint(std::string nickname) {
  for(std::map<unsigned int, MonitorPoint*>::iterator it = _points.begin();
      it != _points.end();
      it++ ) {
    if( it->second->nickname == nickname )
      return it->first;
  }

  //TODO a proper error
  throw 1;
}
unsigned int Monitor::findPoint(std::vector<std::string> signature) {
  for(std::map<unsigned int, MonitorPoint*>::iterator it = _points.begin();
      it != _points.end();
      it++ ) {
    if( it->second->signature == signature )
      return it->first;
  }

  //TODO a proper error
  throw 1;
}


void Monitor::setValue(unsigned int id, unsigned int time, bool value) {
  unsigned int pointId = findPoint(id);
  unsigned int length = _points[pointId]->appendData(time, value);
  if(length > maxLength) {
    maxLength = length;
  }
  return;
}

const unsigned int Monitor::addPoint(std::vector<std::string> signature) {
  unsigned int newId = nextId;
  nextId++;

  _points[newId] = new MonitorPoint();
  _points[newId]->signature = signature;

  return newId;
}
const unsigned int Monitor::addPoint(std::vector<std::string> signature, std::string nickname) {
  unsigned int newId = nextId;
  nextId++;

  _points[newId] = new MonitorPoint();
  _points[newId]->signature = signature;
  _points[newId]->nickname = nickname;

  return newId;
}

void Monitor::renamePoint(unsigned int id, std::string newName) {
  unsigned int pointId = findPoint(id);
  _points[pointId]->nickname = newName;
  return;
}

const std::vector<std::pair<unsigned int, bool> >& Monitor::getLog(unsigned int id) {
  unsigned int pointId = findPoint(id);
  return _points[pointId]->log;
}

const std::vector<std::pair<unsigned int, bool> >& Monitor::getLog(std::string nickname) {
  unsigned int pointId = findPoint(nickname);
  return getLog(pointId);
}

const std::vector<std::pair<unsigned int, bool> >& Monitor::getLog(std::vector<std::string> signature) {
  unsigned int pointId = findPoint(signature);
  return getLog(pointId);
}

unsigned int Monitor::numPoints(void) {
  return _points.size();
}

std::vector<unsigned int> Monitor::getPoints(void) {
  std::vector<unsigned int> points;
  points.reserve(_points.size());
  for(std::map<unsigned int, MonitorPoint*>::iterator it=_points.begin();
      it != _points.end();
      it++)
    points.push_back(it->first);

  return points;
}

std::string Monitor::getNickname(unsigned int id) {
  unsigned int pointId = findPoint(id);
  return _points[pointId]->nickname;
}

std::vector<std::string> Monitor::getSignature(unsigned int id) {
  unsigned int pointId = findPoint(id);
  return _points[pointId]->signature;
}
