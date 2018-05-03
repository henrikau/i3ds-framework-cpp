///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SERVER_HPP
#define __I3DS_SERVER_HPP

#include <memory>
#include <unordered_map>
#include <functional>

#include <i3ds/receiver.hpp>
#include <i3ds/communication.hpp>
#include <i3ds/exception.hpp>
#include <i3ds/service.hpp>

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Server for request/response pattern.
////////////////////////////////////////////////////////////////////////////////

class Server : public Receiver
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

    virtual void Handle(const Message& request, Message& response) = 0;
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

    virtual void Handle(const Message& request, Message& response)
    {
      T::ResponseCodec::Initialize(data_.response);

      Decode<typename T::RequestCodec>(request, data_.request);
      operation_(data_);
      Encode<typename T::ResponseCodec>(response, data_.response);
    }

  private:

    const Operation operation_;

    typename T::Data data_;
  };

  Server(Context::Ptr context);
  virtual ~Server();

  // Attach service handler for endpoint ID.
  template<typename T>
  void Attach(NodeID node, typename Wrapper<T>::Operation operation)
  {
    attach_handler(Address(node, T::endpoint), Wrapper<T>::Create(operation));
  }

  // Attach generic handler for address.
  void attach_handler(Address address, Handler::Ptr handler);

  // Detach handler for address.
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

void set_response(CommandResponse& response, ResultCode result, std::string message);
void set_response(CommandResponse& response, const CommandError& e);

} // namespace i3ds

#endif
