#include "number.h"

bool IsConvertedToBin(const char* array_of_symbols, int size) {
    for (int i = 0; i < size; ++i) {
        if (array_of_symbols[i] != '0') {
            return false;
        }
    }
    return true;
}

void Bin(char* array_of_symbols, size_t size, int2023_t& result) {
    int index = int2023_t::kBytes - 1;
    int8_t count = 0;
    int8_t cache = 0;
    while (!IsConvertedToBin(array_of_symbols, size)) {
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

int GetBitAfterShift(int8_t byte, int shift) {
    return (static_cast<int>(byte >> shift) & 1);
}

bool IsPositive(int8_t first_byte) {
    if (GetBitAfterShift(first_byte, int2023_t::kBits - 1) == 0) {
        return true;
    }
    return false;
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
    Bin(array_of_symbols, is_negative ? size - 1 : size, result);
    if (is_negative) {
        result = MakeTwosComplement(result);
    }
    delete[] array_of_symbols;
    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int addition = 0;
    for (int i = int2023_t::kBytes - 1; i >= 0; --i) {
        int8_t cache = 0;
        for (int j = 0; j < int2023_t::kBits; ++j) {
            int a = GetBitAfterShift(lhs.array[i], j) + GetBitAfterShift(rhs.array[i], j) + addition;
            a >= 2 ? addition = 1 : addition = 0;
            cache += static_cast<int>(pow(2, j)) * (a % 2);
        }
        result.array[i] = result.array[i] | cache;
    }
    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t rhs_copy = MakeTwosComplement(rhs);
    return lhs + rhs_copy;
}


int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t lhs_abs = abs(lhs);
    int2023_t rhs_abs = abs(rhs);
    int2023_t rhs_copy;
    int2023_t lhs_copy;
    if (rhs_abs < lhs_abs) {
        rhs_copy = rhs_abs;
        lhs_copy = lhs_abs;
    } else {
        rhs_copy = lhs_abs;
        lhs_copy = rhs_abs;
    }
    int first_one_index = int2023_t::kBytes - 1;
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        if (rhs_copy.array[i] != 0) {
            first_one_index = i;
            break;
        }
    }
    int2023_t result;
    int2023_t *temp = new int2023_t[(int2023_t::kBytes - first_one_index) * int2023_t::kBits];
    for (int i = 0; i < (int2023_t::kBytes - first_one_index) * int2023_t::kBits; ++i) {
        int offset = i;
        int start = offset / int2023_t::kBits;
        offset %= int2023_t::kBits;
        if (GetBitAfterShift(rhs_copy.array[int2023_t::kBytes - 1 - i / int2023_t::kBits], i % int2023_t::kBits) == 1) {
            int cache = 0;
            for (int k = int2023_t::kBytes - 1; k >= 0; --k) {
                for (int m = 0; m < int2023_t::kBits - offset; ++m) {
                    cache += static_cast<int>(pow(2, m + offset)) * (GetBitAfterShift(lhs_copy.array[k], m));
                }
                temp[i].array[k - start] = temp[i].array[k - start] | cache;
                cache = 0;
                for (int m = int2023_t::kBits - offset; m < int2023_t::kBits; ++m) {
                    cache += static_cast<int>(pow(2, m + offset - int2023_t::kBits)) * (GetBitAfterShift(lhs_copy.array[k], m));
                }
            }
        }
    }
    for (int i = 0; i < (int2023_t::kBytes - first_one_index) * int2023_t::kBits; ++i) {
        result = result + temp[i];
    }
    if ((!IsPositive(lhs.array[0]) || !IsPositive(rhs.array[0])) && !(!IsPositive(lhs.array[0]) && !IsPositive(rhs.array[0]))) {
        result = MakeTwosComplement(result);
    }
    delete[] temp;
    return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t lhs_copy = abs(lhs);
    int2023_t rhs_copy = abs(rhs);
    if (lhs_copy < rhs_copy) {
        return from_int(0);
    }
    if (rhs == from_int(0) || rhs == from_int(1)) {
        return lhs;
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
            remainder = remainder + remainder + from_int(GetBitAfterShift(lhs_copy.array[i], j));
            if (rhs_copy < remainder || rhs_copy == remainder) {
                cache += static_cast<int>(pow(2, j));
                remainder = remainder - rhs_copy;
            }
        }
        result.array[i] = result.array[i] | cache;
    }
    std::cout << remainder << '\n';
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
    int2023_t lhs_abs = abs(lhs);
    int2023_t rhs_abs = abs(rhs);
    if (!IsPositive(lhs.array[0])) {
        if (!IsPositive(rhs.array[0])) {
            if (!IsPositive((lhs_abs - rhs_abs).array[0])) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else {
        if (!IsPositive(rhs.array[0])){
            return false;
        }
        else {
            if (IsPositive((rhs - lhs).array[0]) && (rhs - lhs) != from_int(0)) {
                return true;
            } else {
                return false;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (int i = 0; i < int2023_t::kBytes; ++i) {
        for (int j = int2023_t::kBits - 1; j >= 0; --j) {
            stream << GetBitAfterShift(value.array[i], j);
        }
    }
    return stream;
}