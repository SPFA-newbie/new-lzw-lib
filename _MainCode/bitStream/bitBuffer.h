#pragma once

#include<string>
#include<ostream>
#include<istream>

class BitString;

//使用BitBuffer时不要直接使用这个类，而是使用它的两个子类（Input/OutputBitBuffer）
class BitBuffer{
	private:
/*--------------------------------------------
作用：
	指示一个字节的大小
--------------------------------------------*/ 
		static const int ByteLen;

/*--------------------------------------------
作用：
    求解两数的最大公约数，辅助求缓冲区长度
参数：
    n1, n2 - 要求最大公约数的两个数
返回值：
    num - 两个数的最大公约数
--------------------------------------------*/
		int gcd(int n1, int n2);

/*--------------------------------------------
作用：
    构造缓冲区
    缓冲区长度为适配长度与字节长度的最小公倍数
    这个函数不判断适配长度是否合法
参数：
    无，直接使用函数内数据
返回值：
    无，直接改变函数内数据
--------------------------------------------*/ 
		void setBufferLength();
	
	protected:
		char* data;
		int fitLength;
		int bufferLength;
		
		int position;
		bool streamConnect;
	
	public:
/*--------------------------------------------
作用：
    BitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
返回值：
    无
--------------------------------------------*/ 
		BitBuffer(int fitLength=0);
		
/*--------------------------------------------
作用：
    获取缓冲区的适配长度，单位为bit
参数：
    无
返回值：
    fitLength - 缓冲区适配长度
--------------------------------------------*/
		int getFitLength();

/*--------------------------------------------
作用：
    获取缓冲区的总长度，单位为bit
参数：
    无
返回值：
    bufferLength - 缓冲区适配长度
--------------------------------------------*/
		int getBufferLength();

/*--------------------------------------------
作用：
   设置一个未初始化的缓冲区的适配长度
参数：
    fitLength - 缓冲区适配长度
返回值：
    无
--------------------------------------------*/
		void setFitLength(int fitLength);

/*--------------------------------------------
作用：
    - 调试时使用
    - 将缓冲区中的数据以01串的形式返回
参数：
    无
返回值：
    bits - 一个01串，表示缓冲区中的数据
--------------------------------------------*/
		std::string getBuffer();

/*--------------------------------------------
作用：
    - 调试时使用
    - 将01串代表的数据写入缓冲区
参数：
    bits - 一个01串，表示要写入缓冲区的数据
返回值：
    bool - 设置是否成功
--------------------------------------------*/		
		bool setBuffer(std::string bits);
		
/*--------------------------------------------
作用：
    BitBuffer的析构函数
参数：
    无
返回值：
    无
--------------------------------------------*/
		~BitBuffer(); 
};

class InputBitBuffer : public BitBuffer{
	private:		
		std::ifstream* read;

	public:
		//构造函数
		InputBitBuffer(int fitLength=0);
		InputBitBuffer(int fitLength, std::ifstream& readDocument);
		
		//Stream操作
		void connectStream(std::ifstream& readDocument);
		void disconnectStream(bool closeStream=true);
		//Buffer操作
		InputBitBuffer& operator>>(BitString bits);
		bool close(bool closeStream=true);

		//析构函数
		~InputBitBuffer();
};

class OutputBitBuffer : public BitBuffer{
	private:
		std::ofstream* write;
	public:
		//构造函数
		OutputBitBuffer(int fitLength=0);
		OutputBitBuffer(int fitLength, std::ofstream& writeDocument);

		//Stream操作
		void connectStream(std::ofstream& writeDocument);
		void disconnectStream(bool closeStream=true);
		//Buffer操作
		OutputBitBuffer& operator<<(BitString bits);
		bool close(bool closeStream=true);

		//析构函数
		~OutputBitBuffer();
};
