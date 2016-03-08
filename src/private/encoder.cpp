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

#include "encoder.h"
#include "endian.h"

namespace cbor {


encoder::encoder(output &out) {
    _out = &out;
}

encoder::~encoder() {

}

inline
void encoder::write_type_value(int major_type, unsigned int value) {
    major_type <<= 5;
    if (value < 24) {
        _out->put_byte((unsigned char) (major_type | value));
    } else if (value < 256) {
        _out->put_byte((unsigned char) (major_type | 24));
        _out->put_byte((unsigned char) value);
    } else if (value < 65536) {
        _out->put_byte((unsigned char) (major_type | 25));
        _out->put_byte((unsigned char) (value >> 8));
        _out->put_byte((unsigned char) value);
    } else {
        _out->put_byte((unsigned char) (major_type | 26));
#if 1
        _out->put_byte((unsigned char) (value >> 24));
        _out->put_byte((unsigned char) (value >> 16));
        _out->put_byte((unsigned char) (value >> 8));
        _out->put_byte((unsigned char) value);
#else
        uint32_t t = htobe32(value);
        _out->put_bytes((unsigned char*)&t, sizeof(t));
#endif


    }
}

inline
void encoder::write_type_value(int major_type, unsigned long long value) {
    major_type <<= 5;
    if (value < 24ULL) {
        _out->put_byte((unsigned char) (major_type | value));
    } else if (value < 256ULL) {
        _out->put_byte((unsigned char) (major_type | 24));
        _out->put_byte((unsigned char) value);
    } else if (value < 65536ULL) {
        _out->put_byte((unsigned char) (major_type | 25));
        _out->put_byte((unsigned char) (value >> 8));
        _out->put_byte((unsigned char) value);
    } else if (value < 4294967296ULL) {
        _out->put_byte((unsigned char) (major_type | 26));
#if 1
        _out->put_byte((unsigned char) (value >> 24));
        _out->put_byte((unsigned char) (value >> 16));
        _out->put_byte((unsigned char) (value >> 8));
        _out->put_byte((unsigned char) value);
#else
        uint32_t t = htobe32(value);
        _out->put_bytes((unsigned char*)&t, sizeof(t));
#endif
    } else {
#if 0
        _out->put_byte((unsigned char) (major_type | 27));
        _out->put_byte((unsigned char) (value >> 56));
        _out->put_byte((unsigned char) (value >> 48));
        _out->put_byte((unsigned char) (value >> 40));
        _out->put_byte((unsigned char) (value >> 32));
        _out->put_byte((unsigned char) (value >> 24));
        _out->put_byte((unsigned char) (value >> 16));
        _out->put_byte((unsigned char) (value >> 8));
        _out->put_byte((unsigned char) value);
#else
        uint64_t t = htobe64(value);
        _out->put_bytes((unsigned char*)&t, sizeof(t));
#endif
    }
}

void encoder::write_int(unsigned int value) {
    write_type_value(0, value);
}

void encoder::write_int(unsigned long long value) {
    write_type_value(0, value);
}

void encoder::write_int(long long value) {
    if (value < 0) {
        write_type_value(1, (unsigned long long) -value);
    } else {
        write_type_value(0, (unsigned long long) value);
    }
}

void encoder::write_int(int value) {
    if (value < 0) {
        write_type_value(1, (unsigned int) -value);
    } else {
        write_type_value(0, (unsigned int) value);
    }
}

void encoder::write_bytes(const unsigned char *data, unsigned int size) {
    write_type_value(2, size);
    _out->put_bytes(data, size);
}

void encoder::write_string(const char *data, unsigned int size) {
    write_type_value(3, size);
    _out->put_bytes((const unsigned char *) data, size);
}

void encoder::write_string(const std::string str) {
    write_type_value(3, (unsigned int) str.size());
    _out->put_bytes((const unsigned char *) str.c_str(), (int) str.size());
}


void encoder::write_array(int size) {
    write_type_value(4, (unsigned int) size);
}

void encoder::write_map(int size) {
    write_type_value(5, (unsigned int) size);
}

void encoder::write_tag(const unsigned int tag) {
    write_type_value(6, tag);
}

void encoder::write_special(int special) {
    write_type_value(7, (unsigned int) special);
}

void encoder::write_bool(bool value) {
    if (value == true) {
        _out->put_byte((unsigned char) 0xf5);
    } else {
        _out->put_byte((unsigned char) 0xf4);
    }
}

void encoder::write_null() {
    _out->put_byte((unsigned char) 0xf6);
}

void encoder::write_undefined() {
    _out->put_byte((unsigned char) 0xf7);
}

void encoder::write_float(float value) {
    static_assert(sizeof(uint32_t) == sizeof(float), "float is not 32 bit");
    uint8_t major_type = (7 << 5);
    _out->put_byte(major_type | 26);

#if 0
    uint32_t *t = (uint32_t*)&value;
    _out->put_byte((unsigned char) (*t >> 24));
    _out->put_byte((unsigned char) (*t >> 16));
    _out->put_byte((unsigned char) (*t >> 8));
    _out->put_byte((unsigned char) *t);
#else
    uint32_t t = htobe32(*(uint32_t*)&value);
    _out->put_bytes((unsigned char*)&t, sizeof(t));
#endif
}

void encoder::write_double(double value) {
    static_assert(sizeof(uint64_t) == sizeof(double), "double is not 64 bit");
    uint8_t major_type = (7 << 5);
    _out->put_byte(major_type | 27);

#if 0
    uint64_t *t = (uint64_t*)&value;
    _out->put_byte( major_type | 27);
    _out->put_byte((unsigned char) (*t >> 56));
    _out->put_byte((unsigned char) (*t >> 48));
    _out->put_byte((unsigned char) (*t >> 40));
    _out->put_byte((unsigned char) (*t >> 32));
    _out->put_byte((unsigned char) (*t >> 24));
    _out->put_byte((unsigned char) (*t >> 16));
    _out->put_byte((unsigned char) (*t >> 8));
    _out->put_byte((unsigned char) *t);
#else
    uint64_t t = htobe64(*(uint64_t*)&value);
    _out->put_bytes((unsigned char*)&t, sizeof(t));
#endif
}

}
