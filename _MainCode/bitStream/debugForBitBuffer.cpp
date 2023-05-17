#include<iostream>
using namespace std;

#define DEBUG_FOR_BITBUFFER
#include"bitBuffer.h"
#include"bitString.h"
#include"bitException.h"
#include"bitBufferControllerFlag.h"

void BitBuffer::debug() {
	_Byte* in = new _Byte[2];
	cout << sizeof(*in) << endl;
	in[0] = 128, in[1] = 128;
	setFitData(in);
	setFitData(in);
	cout << getBuffer() << endl;
	delete[] in;
	position = 0;
	in = getFitData();
	cout << (int)in[0] << " " << (int)in[1] << endl;
}

#undef DEBUG_FOR_BITBUFFER

int main() {	
	BitBuffer buffer=BitBuffer();
	buffer.setFitLength(12);
	buffer.debug();
	return 0;
	//BitBuffer* buffer;
	//for (int i = 0; i < 10000; i++) {
	//	buffer = new BitBuffer(16);
	//	delete buffer;
	//}
	//cout << buffer->getFitLength();
	//return 0;
}