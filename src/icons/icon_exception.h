/*GRB*

Gerbera - https://gerbera.io/

    icon_exception.h - this file is part of Gerbera.

    Copyright (C) 2016-2019 Gerbera Contributors

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
/// \file icon_exception.h

#ifndef GERBERA_ICON_EXCEPTION_H
#define GERBERA_ICON_EXCEPTION_H
#include <string>

class IconException : public std::exception {
public:
    explicit IconException(std::string message): m_message(std::move(message)) { }

    const char* what() const noexcept override {
      return m_message.c_str();
    }
private:
    std::string m_message;
};

#endif //GERBERA_ICON_EXCEPTION_H
