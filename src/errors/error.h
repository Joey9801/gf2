#ifndef ERROR_H_
#define ERROR_H_

#include <string>
#include <vector>

#include <plog/Log.h>

struct Location {
  Location();

  bool known;
  std::string file;
  unsigned int line;
  unsigned int column;
};

class GF2Error {
  public:
    virtual ~GF2Error() {}
    std::string name;
    std::string detail;
    bool recoverable;

    virtual std::string formatError(void);

};

class ParseError : public GF2Error {
  public:
    Location location;

    std::string formatError(void);
};

class ErrorList {
  public:
    ErrorList();
    void addError(GF2Error * e);
    void addError(const GF2Error& e);
    void addList(const ErrorList& e);
    void addList(ErrorList * e);
    bool recoverable(void);

    bool anyErrors(void);
    std::vector<std::string> formatErrors(void);

  private:
    std::vector<GF2Error*> _errors;
    bool _recoverable;
};

#endif
