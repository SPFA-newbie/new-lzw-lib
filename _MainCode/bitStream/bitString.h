#pragma once

#include<string>

#include"bitBuffer.h"

//这个类只能依靠BitBuffer进行赋值取值
class BitString{
	private:
/*--------------------------------------------
作用：
    指示一个字节的大小
--------------------------------------------*/ 
		static const int ByteLen;

/*--------------------------------------------
作用：
	将比特数转化为字节数（上取整）
参数：
	bitLen - 比特数
返回值：
	byteLen - 字节数
--------------------------------------------*/
		inline int getByteLen(int bitLen){
			return bitLen%ByteLen==0 ? bitLen/ByteLen : bitLen/ByteLen+1;
		}
	
	private:
		char* data;		
		int  bitLength;

		friend InputBitBuffer& InputBitBuffer::operator>>(BitString bits);
		friend OutputBitBuffer& OutputBitBuffer::operator<<(BitString bits);
	
	public:
/*--------------------------------------------
作用：
    BitString的构造函数
参数：
    bitLength - 比特串的长度，为0时表示未初始化
返回值：
    无
--------------------------------------------*/ 
		BitString(int bitLength=0);
		
/*--------------------------------------------
作用：
    设置一个未初始化的BitString类的比特串长度
参数：
    bitLength - 比特串的长度
返回值：
    无
--------------------------------------------*/
		void setLength(int bitLength);

/*--------------------------------------------
作用：
    获取BitString类的比特串长度
参数：
    无
返回值：
    bitLength - 比特串长度，为0时表示未初始化
--------------------------------------------*/
		int getLength();

/*--------------------------------------------
作用：
    - 调试时使用
    - 将比特串中的数据以01串的形式返回
参数：
    无
返回值：
    bits - 一个01串，表示比特串中的数据
--------------------------------------------*/
		std::string getBitString();

/*--------------------------------------------
作用：
    - 调试时使用
    - 将01串代表的数据写入比特串
参数：
    bits - 一个01串，表示要写入比特串的数据
返回值：
    bool - 设置是否成功
--------------------------------------------*/
		bool setBitString(std::string bits);
		
/*--------------------------------------------
作用：
    BitString的析构函数
参数：
    无
返回值：
    无
--------------------------------------------*/
		~BitString();
		
};
