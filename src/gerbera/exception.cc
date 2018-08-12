/*GRB*
  Gerbera - https://gerbera.io/

  exception.cc - this file is part of Gerbera.

  Copyright (C) 2018 Gerbera Contributors

  Gerbera is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2
  as published by the Free Software Foundation.

  Gerbera is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.

  $Id$
*/

/// \file exception.cc


#include "exception.h"
#include <vector>
#include <sstream>

#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

using namespace gerbera;

Exception::Exception(std::string message, const char* file, int line, const char* function) {
  this->message = std::move(message);
  this->file = file;
  this->function = function;
  this->line = line;
  this->stackTrace = captureBacktrace();
}

Exception::Exception(std::string message) {
  this->message = std::move(message);
  this->file = "";
  this->function = "";
  this->line = -1;
  this->stackTrace = captureBacktrace();
}

std::string Exception::getMessage() const {
  return message;
}

std::vector<std::string> Exception::getStackTrace() const {
  return stackTrace;
}

std::string Exception::getStackTraceStr() const {
  std::ostringstream ss;
  for(auto const& value: stackTrace) {
    ss << value << "\n";
  }
  return ss.str();
}

#ifdef TOMBDEBUG
void Exception::printStackTrace(FILE *file) const {
  if (line >= 0) {
    fprintf(file, "Exception raised in [%s:%d] %s(): %s\n",
            this->file.c_str(), line, function.c_str(), message.c_str());
  } else {
    fprintf(file, "Exception: %s\n", message.c_str());
  }
#if defined HAVE_BACKTRACE
  for(auto const& value: stackTrace) {
    fprintf(file, "%s\n", value.c_str());
  }
  fflush(file);
#endif // HAVE_BACKTRACE
}
#endif // TOMBDEBUG

std::vector<std::string> Exception::captureBacktrace() const {
  std::vector<std::string> stackTrace;
#if defined HAVE_BACKTRACE
  void *b[100];
  int size = backtrace(b, 100);
  char **s = backtrace_symbols(b, size);

  stackTrace.reserve(static_cast<size_t>(size));
  for (int i = 0; i < size; i++) {
    stackTrace.emplace_back(s[i]);
  }

  if (s) {
    free(s);
  }
#endif
  return stackTrace;
}