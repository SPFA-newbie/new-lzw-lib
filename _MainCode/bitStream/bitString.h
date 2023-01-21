#pragma once

#include<string>

#include"bitBuffer.h"

//这个名称空间中存放一些特殊的BitString对象
namespace bstr{
	//这个BitString用来刷新缓冲区
	//缓冲区接受到这个对象时，将强制进行刷新
	//缓冲区会将已有的数据输出到文件中
	extern const BitString refresh;
	//这个BitString用来刷新缓冲区
	//缓冲区接受到这个对象时，将强制进行刷新
	//缓冲区会将已有的数据直接丢弃
	extern const BitString discard; 
};

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

        //这个友元函数用来构造特殊的比特串，不应当随意使用这个函数
        //所有的ID都应该为负数，此时，bitLength即为比特串的id
        friend BitString makeSpecialBitString(int id);
	
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
    - 重载比较相关的运算符
    - 比较为逐位比较
    - 当一个BitString为另一个的前缀时，它更小
参数：
    bstr - 欲进行比较的另一个BitString
返回值：
    bool - 比较的结果
--------------------------------------------*/
        bool operator>(BitString bstr);
        bool operator<(BitString bstr);
        bool operator>=(BitString bstr);
        bool operator<=(BitString bstr);
        bool operator==(BitString bstr);
        bool operator!=(BitString bstr);        

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
