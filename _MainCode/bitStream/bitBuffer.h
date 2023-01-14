#pragma once

#include<string>
#include<ostream>
#include<istream>

class BitString;

//使用BitBuffer时不要直接使用这个类，而是使用它的两个子类（Input/OutputBitBuffer）
class BitBuffer{
	private:
		//辅助常量和函数
		static const int ByteLen;
		int gcd(int n1, int n2);
		void setBufferLength();
	
	protected:
		char* data;
		int fitLength;
		int bufferLength;
		
		int position;
		bool streamConnect;
	
	public:
		//构造函数
		BitBuffer(int fitLength=0);
		
		//获取信息
		int getFitLength();
		int getBufferLength();
		//设置适配长度
		void setFitLength(int fitLength);
		//调试函数
		std::string getBuffer();
		bool setBuffer(std::string bits);
		
		//析构函数
		~BitBuffer(); 
};

class InputBitBuffer : public BitBuffer{
	private:		
		std::ifstream& read;

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
		std::ofstream& write;
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
