#ifndef WAMP_ERROR_HPP
#define WAMP_ERROR_HPP
///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2014 Tavendo GmbH
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#include "wamp_message.hpp"
#include "autobahn/exceptions.hpp"
#include <stdexcept>

namespace autobahn {
class wamp_error : public std::runtime_error {
  public:
    wamp_error(message_type type,
               uint64_t id,
               const std::string& uri,
               const msgpack::object& details,
               const msgpack::object& args,
               const msgpack::object& kwargs,
               msgpack::zone&& zone)
             : std::runtime_error(uri)
             , m_type(type)
             , m_id(id)
             , m_details(details)
             , m_args(args)
             , m_kw_args(kwargs)
             , m_zone(std::make_shared<msgpack::zone>(std::move(zone)))
    {
    }

    wamp_error(const wamp_error &other)
        : std::runtime_error(other.uri())
        , m_type(other.m_type)
        , m_id(other.m_id)
        , m_details(other.m_details)
        , m_args(other.m_args)
        , m_kw_args(other.m_kw_args)
        , m_zone(other.m_zone)
    {
    }

    message_type type(){
        return m_type;
    }

    uint64_t id() const{
        return m_id;
    }

    const char *uri() const{
        return what();
    }

    template<typename List>
    inline List args() const{
        return m_args.as<List>();
    }

    template<typename Map>
    inline Map kw_args() const{
        return m_kw_args.as<Map>();
    }

    template<typename T>
    inline T details() const{
        return m_details.as<T>();
    }
  private:
    message_type m_type;
    uint64_t m_id;
    msgpack::object m_details;
    msgpack::object m_args;
    msgpack::object m_kw_args;
    std::shared_ptr<msgpack::zone> m_zone;
  };
}

#endif // WAMP_ERROR_HPP
