/*GRB*
  Gerbera - https://gerbera.io/

  test_exception.cc - this file is part of Gerbera.

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

/// \file test_exception.cc


#include <common.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gerbera/exception.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>
#ifdef BSD_NATIVE_UUID
#include <uuid.h>
#else
#include <uuid/uuid.h>
#endif

using namespace ::testing;
using namespace gerbera;

class ExceptionTest : public ::testing::Test {

 public:

  ExceptionTest() {};

  virtual ~ExceptionTest() {};

  virtual void SetUp() {
    logFileName = createLogFileName();
    logFile = fopen(logFileName.c_str() , "w");
  }

  virtual void TearDown() {
    fclose(logFile);
    if(remove(logFileName.c_str()) < 0) {
        perror("ERROR: remove");
    }
  }

  std::string createLogFileName() {
    uuid_t uuid;
#ifdef BSD_NATIVE_UUID
    char *uuid_str;
    uint32_t status;
    uuid_create(&uuid, &status);
    uuid_to_string(&uuid, &uuid_str, &status);
#else
    char uuid_str[37];
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
#endif
#ifdef BSD_NATIVE_UUID
    free(uuid_str);
#endif

    std::stringstream ss;
    ss << CMAKE_BINARY_DIR << DIR_SEPARATOR << "test" << DIR_SEPARATOR << "test_gerbera" << DIR_SEPARATOR << uuid_str << ".log";
    return ss.str();
  }

  std::string getLogContents(std::string logFileName) {
    std::ifstream t(std::move(logFileName));
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
  }

  std::string logFileName;
  FILE * logFile;

};

TEST_F(ExceptionTest, CreatesExceptionUsingStringMessageFileLineFunction) {
  std::unique_ptr<Exception> subject = std::make_unique<Exception>("message", "file", 200, "function");

  EXPECT_NE(subject, nullptr);
  EXPECT_STREQ(subject->getMessage().c_str(), "message");
}

TEST_F(ExceptionTest, CreatesExceptionUsingStringMessage) {
  std::unique_ptr<Exception> subject = std::make_unique<Exception>("message");

  EXPECT_NE(subject, nullptr);
  EXPECT_STREQ(subject->getMessage().c_str(), "message");
}

TEST_F(ExceptionTest, CanThrowException) {
  try {
    throw Exception("test exception");
  } catch(Exception const & err) {
    EXPECT_EQ(err.what(),std::string("std::exception"));
    EXPECT_STREQ(err.getMessage().c_str(), "test exception");
  }
}

#if defined HAVE_BACKTRACE

TEST_F(ExceptionTest, ProvidesStackTraceAsStringUsingBacktrace) {
  try {
    throw Exception("test exception");
  } catch(Exception const & err) {
    ASSERT_THAT(err.getStackTraceStr(), HasSubstr("ExceptionTest_ProvidesStackTraceAsStringUsingBacktrace"));
  }
}

TEST_F(ExceptionTest, PrintsStacktraceToTheFileGivenWithFileNameAndLine) {
  try {
    throw Exception("test exception", "file", 200, "function");
  } catch(Exception const & err) {
    if (logFile == NULL) {
      FAIL() << "Error creating temp exception log file";
    } else {
      err.printStackTrace(logFile);

      std::string contents = getLogContents(logFileName);
      ASSERT_THAT(contents, HasSubstr("Exception raised in [file:200] function(): test exception"));
      ASSERT_THAT(contents, HasSubstr("ExceptionTest_PrintsStacktraceToTheFileGivenWithFileNameAndLine"));
    }
  }
}

TEST_F(ExceptionTest, PrintsStacktraceToTheFileGivenWithMessageOnly) {
  try {
    throw Exception("test exception");
  } catch(Exception const & err) {
    if (logFile == NULL) {
      FAIL() << "Error creating temp exception log file";
    } else {
      err.printStackTrace(logFile);

      std::string contents = getLogContents(logFileName);
      ASSERT_THAT(contents, HasSubstr("Exception: test exception"));
      ASSERT_THAT(contents, HasSubstr("ExceptionTest_PrintsStacktraceToTheFileGiven"));
    }
  }
}


#endif

