#pragma once
/*
   Copyright 2014-2015 Stanislav Ovsyannikov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.
*/


#include "listener.h"
#include "input.h"

namespace cbor {
    typedef enum {
        STATE_TYPE,
        STATE_PINT,
        STATE_NINT,
        STATE_BYTES_SIZE,
        STATE_BYTES_DATA,
        STATE_STRING_SIZE,
        STATE_STRING_DATA,
        STATE_ARRAY,
        STATE_MAP,
        STATE_TAG,
        STATE_SPECIAL,
        STATE_ERROR //< A state
    } decoder_state;


    enum class majorType
    {
        unsignedInteger,
        signedInteger,
        byteString,
        utf8String,
        array,
        map,
        tag,
        floatingPoint,
        simpleValue
    };

    enum class simpleValue
    {
        False,
        True,
        Null,
        Undefined
    };


    class type
    {
        majorType m_major;
        size_t m_size;
        uint8_t m_value;
    public:
        type(const majorType& mt, const size_t& size, uint8_t v = 0):m_major(mt), m_size(size), m_value(v) {}
        majorType major() const { return m_major; }
        size_t size() const { return m_size; }
        uint8_t directValue() const { return m_value; }
    };

/*
0: uint8_t, uint16_t, uint32_t, uint64_t
1: int8_t, int16_t, int32_t, int64_t
2: string
3: string // utf-8
4: vector
5: map (struct)
6: tag
7: half, float, double, bool, nullptr, break
*/

    class decoder {
    private:
        listener *_listener;
        input *_in;
        decoder_state _state;
        int _currentLength;
    public:
        decoder(input &in);
        decoder(input &in, listener &listener);
        ~decoder();
        void run();
        void set_listener(listener &listener_instance);
        void traverse();

        size_t offset() const { return _in->offset(); }

        type peekType() const;

        size_t read_map();
        size_t read_array();

        uint32_t read_uint();
        uint64_t read_ulong();

        int32_t read_int();
        int64_t read_long();

        float read_float();
        double read_double();

        std::string read_string();

        bool read_bool();

        void skip();
    };
}

