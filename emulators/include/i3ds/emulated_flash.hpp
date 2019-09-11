///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_FLASH_HPP
#define __I3DS__EMULATED_FLASH_HPP

#include <i3ds/flash.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

namespace i3ds
{

class EmulatedFlash : public Flash
{
public:

  // Shared pointer for flash interfaces.
  typedef std::shared_ptr<EmulatedFlash> Ptr;

  static Ptr Create(NodeID id)
  {
    return std::make_shared<EmulatedFlash>(id);
  }

  // Constructor for flash.
  EmulatedFlash(NodeID node);

  // Destructor for flash.
  virtual ~EmulatedFlash();

  ShutterTime duration() const {return duration_;}

  FlashStrength strength() const {return strength_;}

protected:

  // Handler for flash setup command, must be overloaded.
  virtual void handle_flash(FlashService::Data& command);

private:

  ShutterTime duration_;
  FlashStrength strength_;
};

} // namespace i3ds

#endif
