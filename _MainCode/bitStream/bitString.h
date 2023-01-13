#pragma once

#include<string>

#include"bitBuffer.h"

//这个类只能依靠BitBuffer进行赋值取值
class BitString{
	private:
		char* data;		
		int  bitLength;

		friend InputBitBuffer& InputBitBuffer::operator>>(BitString bits);
		friend OutputBitBuffer& OutputBitBuffer::operator<<(BitString bits);
	
	public:
		//构造函数
		BitString(int bitLength=0);
		
		//串长度相关
		void setLength(int bitLength);
		int getLength();
		//调试函数，不要使用
		std::string getBitString();
		bool setBitString(std::string bits);
		
		//析构函数
		~BitString();
		
};
