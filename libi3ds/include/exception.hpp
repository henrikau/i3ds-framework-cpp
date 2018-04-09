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

class CommandException : public std::runtime_error
{
public:

  CommandException(ResultCode result, std::string what_arg) : std::runtime_error(what_arg), result_(result) {};
  virtual ~CommandException() {};

  inline ResultCode result() const {return result_;}

private:

  const ResultCode result_;
};

}

#endif
