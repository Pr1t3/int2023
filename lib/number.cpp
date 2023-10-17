#include "number.h"

bool IsStringEmpty(const char* array_of_symbols, int size) {
    for (int i = 0; i < size; ++i) {
        if (array_of_symbols[i] != '0') {
            return false;
        }
    }
    return true;
}

void ConvertingFromString(char* array_of_symbols, size_t size, int2023_t& result) {
    int index = int2023_t::kBytes - 1;
    int8_t count = 0;
    int8_t cache = 0;
    while (!IsStringEmpty(array_of_symbols, size)) {
        int addition = 0;
        for (int i = 0; i < size; ++i) {
            if (i == size - 1) {
                cache += static_cast<int>(pow(2, count)) * (static_cast<int>(array_of_symbols[i] - '0') % 2);
                ++count;
            }
            if (static_cast<int>(array_of_symbols[i] - '0') % 2 != 0) {
                array_of_symbols[i] = static_cast<char>((static_cast<int>(array_of_symbols[i] - '0') + addition) / 2 + '0');
                addition = 10;
            } else {
                array_of_symbols[i] = static_cast<char>((static_cast<int>(array_of_symbols[i] - '0') + addition) / 2 + '0');
                addition = 0;
            }
        }
        if (count == int2023_t::kBits) {
            result.array[index] = result.array[index] | cache;
            cache = 0;
            count = 0;
            --index;
        }
    }
    result.array[index] = result.array[index] | cache;
}

uint8_t GetBitAfterShift(int8_t byte, int shift) {
    return ((byte >> shift) & 1);
}

bool IsPositive(int8_t first_byte) {
    return GetBitAfterShift(first_byte, int2023_t::kBits - 1) == 0;
}

int2023_t MakeTwosComplement(const int2023_t& num) {
    int2023_t result;
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        result.array[i] = ~num.array[i];
    }
    return result + from_int(1);
}

int2023_t abs(const int2023_t& value) {
    if (IsPositive(value.array[0])) {
        return value;
    }
    return MakeTwosComplement(value);
}

int2023_t from_int(int32_t i) {
    bool is_negative = false;
    if (i < 0) {
        is_negative = true;
        i = abs(i);
    }
    int index = int2023_t::kBytes - 1;
    int8_t cache = 0;
    int8_t count = 0;
    int2023_t result;
    while (i > 0) {
        if (i % 2 != 0) {
            cache += static_cast<int>(pow(2, count));
        }
        i = i >> 1;
        ++count;
        if (count == int2023_t::kBits) {
            result.array[index] = result.array[index] | cache;
            count = 0;
            cache = 0;
            --index;
        }
    }
    result.array[index] = result.array[index] | cache;
    if (is_negative) {
        result = MakeTwosComplement(result);
    }
    return result;
}

int2023_t from_string(const char* buff) {
    int2023_t result;
    bool is_negative = false;
    size_t size = std::strlen(buff);
    if (buff[0] == '-') {
        is_negative = true;
    }
    char* array_of_symbols = new char[size];
    memset(array_of_symbols, 0, size);
    size_t index;
    is_negative ? index = 1 : index = 0;
    while (index != size) {
        array_of_symbols[is_negative ? index - 1 : index] = buff[index];
        ++index;
    }
    ConvertingFromString(array_of_symbols, is_negative ? size - 1 : size, result);
    if (is_negative) {
        result = MakeTwosComplement(result);
    }
    delete[] array_of_symbols;
    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int addition = 0;
    for(int i = int2023_t::kBytes - 1; i >= 0; --i){
        if((lhs.array[i] + rhs.array[i] + addition) > 255){
            result.array[i] = lhs.array[i] + rhs.array[i] + addition - 256;
            addition = 1;
        } else {
            result.array[i] = lhs.array[i] + rhs.array[i] + addition;
            addition = 0;
        }
    }
    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t rhs_copy = MakeTwosComplement(rhs);
    return lhs + rhs_copy;
}


int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t lhs_copy = abs(lhs);
    int2023_t rhs_copy = abs(rhs);
    if (rhs_copy == from_int(0)) {
        return from_int(0);
    }
    int first_one_index = int2023_t::kBytes - 1;
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        if (rhs_copy.array[i] != 0) {
            first_one_index = i;
            break;
        }
    }
    int2023_t result;
    for (int i = 0; i < (int2023_t::kBytes - first_one_index) * int2023_t::kBits; ++i) {
        if (GetBitAfterShift(rhs_copy.array[int2023_t::kBytes - 1 - i / int2023_t::kBits], i % int2023_t::kBits) == 1) {
            result = result + lhs_copy;
        }
        lhs_copy = lhs_copy + lhs_copy;
    }
    if ((!IsPositive(lhs.array[0]) || !IsPositive(rhs.array[0])) && !(!IsPositive(lhs.array[0]) && !IsPositive(rhs.array[0]))) {
        result = MakeTwosComplement(result);
    }
    return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    if (rhs == from_int(0) || rhs == from_int(1)) {
        return lhs;
    }
    int2023_t lhs_copy = abs(lhs);
    int2023_t rhs_copy = abs(rhs);
    if (lhs_copy < rhs_copy) {
        return from_int(0);
    }
    int first_one_index = int2023_t::kBytes - 1;
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        if (lhs_copy.array[i] != 0) {
            first_one_index = i;
            break;
        }
    }
    int2023_t result;
    int2023_t remainder;
    for (int i = first_one_index; i < int2023_t::kBytes; ++i) {
        int8_t cache = 0;
        for (int j = int2023_t::kBits - 1; j >= 0; --j) {
            remainder = remainder + remainder;
            if (GetBitAfterShift(lhs_copy.array[i], j) == 1){
                remainder = remainder + from_int(1);
            }
            if (rhs_copy < remainder || rhs_copy == remainder) {
                cache += static_cast<int>(pow(2, j));
                remainder = remainder - rhs_copy;
            }
        }
        result.array[i] = result.array[i] | cache;
    }
    if ((!IsPositive(lhs.array[0]) || !IsPositive(rhs.array[0])) && !(!IsPositive(lhs.array[0]) && !IsPositive(rhs.array[0]))) {
        result = MakeTwosComplement(result);
    }
    return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        if (lhs.array[i] != rhs.array[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    if(lhs == rhs){
        return false;
    }
    if (!IsPositive(lhs.array[0])) {
        if (!IsPositive(rhs.array[0])) {
            for(int i = 0; i < int2023_t::kBytes; ++i){
                if(lhs.array[i] != rhs.array[i]){
                    return lhs.array[i] < rhs.array[i];
                }
            }
        } else {
            return true;
        }
    } else {
        if (!IsPositive(rhs.array[0])){
            return false;
        }
        else {
            for(int i = 0; i < int2023_t::kBytes; ++i){
                if(lhs.array[i] != rhs.array[i]){
                    return lhs.array[i] < rhs.array[i];
                }
            }
        }
    }
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (uint8_t i : value.array) {
        stream << static_cast<int>(i);
    }
    return stream;
}