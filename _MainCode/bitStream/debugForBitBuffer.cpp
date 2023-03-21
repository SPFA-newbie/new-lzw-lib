#include<iostream>
using namespace std;

#define DEBUG_FOR_BITBUFFER
#include"bitBuffer.h"
#include"bitString.h"
#include"bitException.h"
#include"bitBufferControllerFlag.h"

void BitBuffer::debug() {
	cout << hex << (unsigned)makeMask(0, 1);
}

int main() {
	BitBuffer buffer;
    buffer.setFitLength(12);
	buffer.debug();
}

#undef DEBUG_FOR_BITBUFFER