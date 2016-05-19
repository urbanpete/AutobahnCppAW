///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Tavendo GmbH
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AUTOBAHN_WAMP_ARGUMENTS_HPP
#define AUTOBAHN_WAMP_ARGUMENTS_HPP

#include <array>
#include <msgpack.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace autobahn {

using wamp_arguments = std::vector<msgpack::object>;
using wamp_kw_arguments = std::unordered_map<std::string, msgpack::object>;

static const msgpack::object EMPTY_DETAILS(std::unordered_map<std::string, msgpack::object>(), nullptr);
static const msgpack::object EMPTY_ARGUMENTS(std::array<msgpack::object, 0>(), nullptr);
static const msgpack::object EMPTY_KW_ARGUMENTS(wamp_kw_arguments(), nullptr);


//msgpack map utilities.
//TODO: refactor event & invocation to used these
template <typename T>
inline T value_for_key(const msgpack::object& object, const std::string& key)
{
    if (object.type != msgpack::type::MAP) {
        throw msgpack::type_error();
    }
    for (std::size_t i = 0; i < object.via.map.size; ++i) {
        const msgpack::object_kv& kv = object.via.map.ptr[i];
        if (kv.key.type == msgpack::type::STR && key.size() == kv.key.via.str.size
            && key.compare(0, key.size(), kv.key.via.str.ptr, kv.key.via.str.size) == 0)
        {
            return kv.val.as<T>();
        }
    }
    throw std::out_of_range(key + " keyword argument doesn't exist");
}

template <typename T>
inline T value_for_key(const msgpack::object& object, const char* key)
{
    if (object.type != msgpack::type::MAP) {
        throw msgpack::type_error();
    }
    std::size_t key_size = strlen(key);
    for (std::size_t i = 0; i < object.via.map.size; ++i) {
        const msgpack::object_kv& kv = object.via.map.ptr[i];
        if (kv.key.type == msgpack::type::STR && key_size == kv.key.via.str.size
            && memcmp(key, kv.key.via.str.ptr, key_size) == 0)
        {
            return kv.val.as<T>();
        }
    }
    throw std::out_of_range(std::string(key) + " keyword argument doesn't exist");
}

template <typename T>
inline T value_for_key_or(const msgpack::object& object, const std::string& key, const T& fallback)
{
    if (object.type != msgpack::type::MAP) {
        throw msgpack::type_error();
    }
    for (std::size_t i = 0; i < object.via.map.size; ++i) {
        const msgpack::object_kv& kv = object.via.map.ptr[i];
        if (kv.key.type == msgpack::type::STR && key.size() == kv.key.via.str.size
            && key.compare(0, key.size(), kv.key.via.str.ptr, kv.key.via.str.size) == 0)
        {
            return kv.val.as<T>();
        }
    }
    return fallback;
}

template <typename T>
inline T value_for_key_or(const msgpack::object& object, const char* key, const T& fallback)
{
    if (object.type != msgpack::type::MAP) {
        throw msgpack::type_error();
    }
    std::size_t key_size = strlen(key);
    for (std::size_t i = 0; i < object.via.map.size; ++i) {
        const msgpack::object_kv& kv = object.via.map.ptr[i];
        if (kv.key.type == msgpack::type::STR && key_size == kv.key.via.str.size
            && memcmp(key, kv.key.via.str.ptr, key_size) == 0)
        {
            return kv.val.as<T>();
        }
    }
    return fallback;
}
} // namespace autobahn

#endif // AUTOBAHN_WAMP_ARGUMENTS_HPP
