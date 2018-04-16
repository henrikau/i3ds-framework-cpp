///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SUBSCRIBER_HPP
#define __I3DS_SUBSCRIBER_HPP

#include <memory>
#include <unordered_map>
#include <functional>

#include "i3ds/core/receiver.hpp"
#include "i3ds/core/communication.hpp"
#include "i3ds/core/topic.hpp"
#include "i3ds/core/codec.hpp"

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Subscriber for publish/subscribe pattern.
////////////////////////////////////////////////////////////////////////////////

class Subscriber : public Receiver
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// Handler for request/response pattern.
  ////////////////////////////////////////////////////////////////////////////////

  class Handler
  {
  public:

    typedef std::unique_ptr<Handler> Ptr;

    virtual ~Handler() {};

    virtual void Handle(const Message& message) = 0;
  };
  
  ////////////////////////////////////////////////////////////////////////////////
  /// Codec wrapper for request/response pattern.
  ////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class Wrapper : public Handler
  {
  public:

    typedef std::function<void(typename T::Data&)> Operation;

    static inline Handler::Ptr Create(Operation operation)
    {
      return Handler::Ptr(new Wrapper<T>(operation));
    }

    Wrapper(Operation operation) : operation_(operation) {};

    virtual ~Wrapper() {};
    
    virtual void Handle(const Message& message)
    {
      Decode<typename T::Codec>(message, data_);
      operation_(data_);
    }

  private:

    const Operation operation_;
    
    typename T::Data data_;
  };

  ////////////////////////////////////////////////////////////////////////////////
  /// Constructor and destructor
  ////////////////////////////////////////////////////////////////////////////////

  Subscriber(Context::Ptr context);
  virtual ~Subscriber();

  // Register callback for topic.
  template<typename T>
  void Attach(NodeID node, typename Wrapper<T>::Operation operation)
  {
    attach_handler(Address(node, T::endpoint), Wrapper<T>::Create(operation));
  }

  // Attach generic handler for address.
  void attach_handler(Address address, Handler::Ptr handler);

  // Detach handler for endpoint address.
  void detach_handler(Address address);

protected:

  // Initialize the socket.
  virtual Socket::Ptr Create();

  // Handle message, may send using socket.
  virtual void Handle(Message& message, Socket& socket);

private:

  // Map with handlers for endpoints.
  std::unordered_map<Address, Handler::Ptr> handlers_;
};

} // namespace i3ds

#endif
