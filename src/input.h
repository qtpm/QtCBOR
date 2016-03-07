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

#include <cstdint>

namespace cbor {
    class input {
    private:
        unsigned char *_data;
        int _size;
        int _offset;
    public:
        input(void *data, int size);

        ~input();

        int offset() const { return _offset; }

        void advance(int bytes);

        uint8_t peek_byte() const;

        bool has_bytes(int count);

        unsigned char get_byte();
        unsigned short get_short();
        unsigned int get_int();
        unsigned long long get_long();

        float get_float();

        double get_double();

        void get_bytes(void *to, int count);
    };
}
