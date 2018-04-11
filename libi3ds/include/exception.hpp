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

class CommandError : public std::runtime_error
{
public:

  CommandError(ResultCode result, const char* message) : std::runtime_error(message), result_(result) {};
  virtual ~CommandError() {};

  inline ResultCode result() const {return result_;}

private:

  const ResultCode result_;
};

}

#endif
