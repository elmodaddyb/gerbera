/*GRB*
  Gerbera - https://gerbera.io/

  exception.h - this file is part of Gerbera.

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

/// \file exception.h

#ifndef GERBERA_EXCEPTION_H
#define GERBERA_EXCEPTION_H

#include <exception>
#include <string>
#include <vector>
#include "logger.h"

namespace gerbera {

class Exception : public std::exception {
 protected:
  std::string message;
  std::string file;
  std::string function;
  std::vector<std::string> stackTrace;
  int line;

 public:
  Exception(std::string message, const char* file, int line, const char* function);
  explicit Exception(std::string message);
  std::string getMessage() const;
  std::vector<std::string> getStackTrace() const;
  std::string getStackTraceStr() const;
#ifdef TOMBDEBUG
  void printStackTrace(FILE *file = LOG_FILE) const;
#else
  inline void printStackTrace(FILE *file = LOG_FILE) const {};
#endif

};

}

#endif //GERBERA_EXCEPTION_H
