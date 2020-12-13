/**
 * Program 5
 * Operator Overloading Lab
 * CS320
 * 25 November 2020
 * @author Brandon Nguyen cssc2650
 */

#include "intArray.h"

using namespace std;

IntArray::IntArray() {  // default constructor
    name = "";
    pA = new int[10];
    size = 10;
    lo = 0;
    hi = 9;
}

IntArray::IntArray(int size) {  // size constructor
    name = "";
    pA = new int[size];
    this->size = size;
    lo = 0;
    hi = size - 1;
}

IntArray::IntArray(int lowerIndex, int upperIndex) {  // lower/upper constructor
    if (lowerIndex > upperIndex) {
        name = "";
        size = -1;
        pA = NULL;
        lo = lowerIndex;
        hi = upperIndex;
        cout << "Upper index cannot be less than lower index." << endl;
    } else {
        name = "";
        size = (upperIndex - lowerIndex) + 1;
        this->pA = new int[size];
        lo = lowerIndex;
        hi = upperIndex;
    }
}

IntArray::IntArray(const IntArray& other) {  // copy constructor
    name = other.name;
    size = other.size;
    pA = new int[size];
    int i =0;
    while (i < size) {
        pA[i] = other.pA[i];
        i++;
    }
    hi = other.hi;
    lo = other.lo;
}

IntArray::~IntArray() {  // destructor
    delete[] this->pA;   // delete the pA dynamic array
}

int IntArray::operator==(const IntArray& other) {
    if (this->size != other.size) {
        return 0;
    }
    int i = 0;
    while (i < this->size) {
        if (this->pA[i] != other.pA[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

int IntArray::operator!=(const IntArray& other) {
    int result = *this == other;
    if (result == 1) {
        return 0;
    } else {
        return 1;
    }
}

int& IntArray::operator[](int idx) {
    if (idx > this->hi || idx < this->lo) {
        cout << "Given index is out of bounds." << endl;
        return this->pA[0]; // return 1st item if out of bounds
    } else {
        idx -= this->lo;  // map given idx to zero-index
        return this->pA[idx];
    }
}

IntArray& IntArray::operator=(const IntArray& other) {
    if (this->size != other.size) {
        cout << "Sizes do not match." << endl;
        return *this;
    }
    int i = 0;
    while (i < this->size) {
        this->pA[i] = other.pA[i];
        i++;
    }
    return *this;
}

IntArray IntArray::operator+(const IntArray& other) {
    if (this->size != other.size) {
        cout << "Sizes do not match." << endl;
        return *this;
    }
    IntArray newArr(other.size);
    int i = 0;
    while (i < newArr.size) {
        newArr.pA[i] = this->pA[i] + other.pA[i];
        i++;
    }

    return newArr;
}

IntArray& IntArray::operator+=(const IntArray& other) {
    if (this->size != other.size) {
        cout << "Sizes do not match." << endl;
        return *this;
    }
    int i = 0;
    while (i < this->size) {
        this->pA[i] += other.pA[i];
        i++;
    }
    return *this;
}

ostream& operator<<(ostream& os, const IntArray& other) {
    int i = 0;
    while (i < other.size) {
        int other_idx = other.lo + i;
        os << other.name << '[' << other_idx << "] = " << other.pA[i]
           << endl;
        i++;
    }
    return os;
}

void IntArray::setName(char* name) { this->name = name; }

void IntArray::getName() { cout << this->name; }

int IntArray::high() { return this->hi; }

int IntArray::low() { return this->lo; }
