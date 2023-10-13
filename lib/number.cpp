#include "number.h"

bool IsConvertedToBin(const char* array_of_symbols, const int size) {
    for (int i = 0; i < size; ++i) {
        if (array_of_symbols[i] != '0') {
            return false;
        }
    }
    return true;
}

void Bin(char* array_of_symbols, int size, int2023_t& result) {
    int index = kBytes-1;
    int count = 0;
    int cache = 0;
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
        if (count == kBits) {
            result.array[index] = result.array[index] | cache;
            cache = 0;
            count = 0;
            --index;
        }
    }
    result.array[index] = result.array[index] | cache;
}

int2023_t abs(const int2023_t& value) {
    if (((value.array[0] >> (kBits-1)) & 1) == 0) return value;
    int2023_t result = from_int(0);
    for (int i = 0; i < kBytes; ++i) {
        int cache = 0;
        for (int j = 0; j < kBits; ++j) {
            if (((value.array[i] >> j) & 1) == 0) {
                cache += static_cast<int>(pow(2, j));
            }
        }
        result.array[i] = result.array[i] | cache;
    }
    return result + from_int(1);
}

void TwosComplements(int2023_t& num) {
    for (int i = 0; i < kBytes; ++i) {
        int cache = 0;
        for (int j = 0; j < kBits; ++j) {
            if (((num.array[i] >> j) & 1) == 0) {
                cache += static_cast<int>(static_cast<int>(pow(2, j)));
            }
        }
        num.array[i] = (num.array[i] & 0) | cache;
    }
    num = num + from_int(1);
}

int2023_t from_int(int32_t i) {
    int2023_t result;
    for (int j = 0; j < kBytes; ++j) {
        result.array[j] = static_cast<char>(0);
    }
    bool isNegative = false;
    if (i < 0) {
        isNegative = true;
        i = abs(i);
    }
    int index = kBytes-1;
    int cache = 0;
    int count = 0;
    while (i > 0) {
        if (i % 2 != 0) {
            cache += static_cast<int>(pow(2, count));
        }
        i = i >> 1;
        ++count;
        if (count == kBits) {
            result.array[index] = result.array[index] | cache;
            count = 0;
            cache = 0;
            --index;
        }
    }
    result.array[index] = result.array[index] | cache;
    if (isNegative) TwosComplements(result);
    return result;
}

int2023_t from_string(const char* buff) {
    int2023_t result;
    result = from_int(0);
    bool isNegative = false;
    int size = 0;
    while (buff[size] != '\0') {
        ++size;
    }
    if (buff[0] == '-') {
        isNegative = true;
    }
    char *array_of_symbols = new char[size];
    for (int i = 0; i < size; ++i) {
        array_of_symbols[i] = '0';
    }
    int index;
    isNegative ? index = 1 : index = 0;
    while (index != size) {
        array_of_symbols[isNegative ? index - 1 : index] = buff[index];
        ++index;
    }
    Bin(array_of_symbols, isNegative ? size - 1 : size, result);
    if (isNegative) TwosComplements(result);
    delete[] array_of_symbols;
    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    result = from_int(0);
    int addition = 0;
    for (int i = kBytes-1; i >= 0; --i) {
        int cache = 0;
        for (int j = 0; j < kBits; ++j) {
            int a = ((lhs.array[i] >> j) & 1) + ((rhs.array[i] >> j) & 1) + addition;
            a >= 2 ? addition = 1 : addition = 0;
            cache += static_cast<int>(static_cast<int>(pow(2, j))) * (a % 2);
        }
        result.array[i] = result.array[i] | cache;
    }
    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t right;
    for (int i = 0; i < kBytes; ++i) {
        right.array[i] = rhs.array[i];
    }
    TwosComplements(right);
    result = lhs + right;
    return result;
}


int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result = from_int(0);
    int2023_t right;
    int2023_t left;
    if (abs(lhs) < abs(rhs)) {
        right = abs(rhs);
        left = abs(lhs);
    } else {
        right = abs(lhs);
        left = abs(rhs);
    }
    int first_one_index = kBytes-1;
    for (int i = 0; i < kBytes; ++i) {
        if (right.array[i] != '\000') {
            first_one_index = i;
            break;
        }
    }
    int2023_t *temp = new int2023_t[(kBytes - first_one_index) * kBits];
    for (int i = 0; i < (kBytes - first_one_index) * kBits; ++i) {
        int offset = i;
        int start = offset / kBits;
        offset %= kBits;
        if ((right.array[kBytes-1 - i / kBits] >> i % kBits) & 1 == 1) {
            int cache = 0;
            for (int k = kBytes-1; k >= 0; --k) {
                for (int m = 0; m < kBits - offset; ++m) {
                    cache += pow(2, m + offset) * (static_cast<int>((left.array[k] >> m)) & 1);
                }
                temp[i].array[k - start] = temp[i].array[k - start] | cache;
                cache = 0;
                for (int m = kBits - offset; m < kBits; ++m) {
                    cache += pow(2, m + offset - kBits) * (static_cast<int>((left.array[k] >> m)) & 1);
                }
            }
        }
    }
    for (int i = 0; i < (kBytes - first_one_index) * kBits; ++i) {
        result = result + temp[i];
    }
    delete[] temp;
    if ((lhs != abs(lhs) || rhs != abs(rhs)) && !(abs(lhs) != lhs && abs(rhs) != rhs)) {
        TwosComplements(result);
    }
    return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result = from_int(0);
    if (abs(lhs) < abs(rhs)) return from_int(0);
    int2023_t remainder = from_int(0);
    int2023_t right = abs(rhs);
    int2023_t left = abs(lhs);
    int first_one_index = kBytes-1;
    for (int i = 0; i < kBytes; ++i) {
        if (left.array[i] != '\000') {
            first_one_index = i;
            break;
        }
    }
    for (int i = first_one_index; i < kBytes; ++i) {
        int cache = 0;
        for (int j = kBits-1; j >= 0; --j) {
            remainder = remainder + remainder + from_int((left.array[i] >> j) & 1);
            if (right < remainder || right == remainder) {
                cache += static_cast<int>(pow(2, j));
                remainder = remainder - right;
            }
        }
        result.array[i] = result.array[i] | cache;
    }
    if ((lhs != abs(lhs) || rhs != abs(rhs)) && !(abs(lhs) != lhs && abs(rhs) != rhs)) {
        TwosComplements(result);
    }
    return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < kBytes; ++i) {
        if (lhs.array[i] != rhs.array[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < kBytes; ++i) {
        if (lhs.array[i] != rhs.array[i]) {
            return true;
        }
    }
    return false;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    if (lhs != abs(lhs)) {
        if (rhs != abs(rhs)) {
            if ((((abs(lhs) - abs(rhs)).array[0] >> (kBits-1)) & 1) == 0) {
                return true;
            } else {
                return false;
            }
        }
        else return true;
    } else {
        if (rhs != abs(rhs)) return false;
        else {
            if ((((rhs - lhs).array[0] >> (kBits-1)) & 1) == 0 && (rhs - lhs) != from_int(0)) {
                return true;
            } else {
                return false;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (int i = 0; i < kBytes; ++i) {
        for (int j = kBits-1; j >= 0; --j) {
            stream << ((value.array[i] >> j) & 1);
        }
    }
    return stream;
}