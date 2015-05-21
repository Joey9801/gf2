#include <iostream>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

//This is a dummy main, feel free to overwrite it in any merge
int main(void) {

  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::debug, &consoleAppender);

  return 0;
}
