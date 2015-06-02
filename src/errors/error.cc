#include "error.h"

Location::Location() :
  known(false)
{}

std::string GF2Error::formatError(void) {
  return name + ": " + detail;
}

std::string ParseError::formatError(void) {
  return name + ": " + detail + " in file " + location.file;
}

ErrorList::ErrorList() :
  _recoverable(true) // Recoverable until we encounter one which isn't
{}

void ErrorList::addError(GF2Error * e) {
  _errors.push_back(e);
  _recoverable = _recoverable & e->recoverable;

  if(not _recoverable)
    LOG_WARNING << "Errorlist set to unrecoverable";

  return;
}

void ErrorList::addError(const GF2Error& e) {
  GF2Error * err = new GF2Error(e);
  addError(err);
  return;
}

void ErrorList::addList(const ErrorList& e) {
  _errors.insert(_errors.end(), e._errors.begin(), e._errors.end());
  _recoverable = _recoverable & e._recoverable;

  return;
}

void ErrorList::addList(ErrorList * e) {
  _errors.insert(_errors.end(), e->_errors.begin(), e->_errors.end());
  _recoverable = _recoverable & e->_recoverable;

  return;
}

bool ErrorList::recoverable(void) {
  return _recoverable;
}

bool ErrorList::anyErrors(void) {
  return _errors.size() > 0;
}

std::vector<std::string> ErrorList::formatErrors(void) {
  std::vector<std::string> formattedErrors;
  formattedErrors.reserve(_errors.size());
  for(std::vector<GF2Error*>::iterator it = _errors.begin();
      it != _errors.end();
      it++) {
    formattedErrors.push_back((*it)->formatError());
  }

  return formattedErrors;
}
