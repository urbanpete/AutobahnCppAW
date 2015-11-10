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

#ifndef AUTOBAHN_WAMP_MESSAGE_HPP
#define AUTOBAHN_WAMP_MESSAGE_HPP

#include <msgpack.hpp>
#include <vector>
#include <autobahn/wamp_message_type.hpp>
#include <autobahn/exceptions.hpp>

namespace autobahn {

class wamp_message{
public:
    wamp_message(){}
    wamp_message(msgpack::unpacked &Input)
    {
        init(Input);
    }
    void init(msgpack::unpacked &Input){
        msgpack::object obj = Input.get();

        if (obj.type != msgpack::type::ARRAY) {
            throw protocol_error("invalid message structure - message is not an array");
        }
        obj.convert(m_fields);
        m_zone = std::move(Input.zone());


        if (m_fields.size() < 1) {
            throw protocol_error("invalid message structure - missing message code");
        }

        if (m_fields[0].type != msgpack::type::POSITIVE_INTEGER) {
            throw protocol_error("invalid message code type - not an integer");
        }
    }
     const msgpack::object &operator [](size_t idx) const{
        return m_fields[idx];
    }

    size_t size() const{
        return m_fields.size();
    }

    msgpack::unique_ptr<msgpack::zone> &zone(){
        return m_zone;
    }

    message_type type() const{
        return static_cast<message_type>(m_fields[0].as<int>());
    }

    std::string to_string(){
        std::stringstream ss;
        ss << '[';
        for(size_t i = 0; i<m_fields.size(); ++i){
            if(i>0){
                ss<< ", ";
            }
            ss << m_fields[0];
        }
        ss << ']';
        return ss.str();
    }
private:
    std::vector<msgpack::object> m_fields;
    msgpack::unique_ptr<msgpack::zone> m_zone;
};

} // namespace autobahn

#endif // AUTOBAHN_WAMP_MESSAGE_HPP
