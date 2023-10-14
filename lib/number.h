#pragma once
#include <cmath>
#include <cinttypes>
#include <iostream>
#include <cstring>

struct int2023_t {
    static const int kBytes = 253;
    static const int kBits = 8;
    uint8_t array[kBytes] = {0};
    int2023_t(char* array) {
        for (int i = 0; i < 253; ++i) {
            this->array[i] = array[i];
        }
    }
    int2023_t(){};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);