///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EXCEPTION_HPP
#define __I3DS_EXCEPTION_HPP

#include <string>
#include <stdexcept>

#include "Common.h"

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
// Timeout exception for connect or receive operations.
////////////////////////////////////////////////////////////////////////////////

class Timeout : public std::exception
{
public:

  Timeout() : std::exception() {};
};

////////////////////////////////////////////////////////////////////////////////
// General communication error.
////////////////////////////////////////////////////////////////////////////////

class CommunicationError : public std::runtime_error
{
public:

  CommunicationError(std::string message) : std::runtime_error(message) {};
  CommunicationError(const char* message) : std::runtime_error(message) {};
};

////////////////////////////////////////////////////////////////////////////////
// Codec encode/decode error.
////////////////////////////////////////////////////////////////////////////////

class CodecError : public std::runtime_error
{
public:

  CodecError(std::string message) : std::runtime_error(message) {};
  CodecError(const char* message) : std::runtime_error(message) {};
};

////////////////////////////////////////////////////////////////////////////////
// Error with commands containg a result code.
////////////////////////////////////////////////////////////////////////////////

class CommandError : public std::runtime_error
{
public:

  CommandError(ResultCode result, std::string message) : std::runtime_error(message), result_(result) {};
  CommandError(ResultCode result, const char* message) : std::runtime_error(message), result_(result) {};
  virtual ~CommandError() {};

  inline ResultCode result() const {return result_;}

private:

  const ResultCode result_;
};

}

#endif
