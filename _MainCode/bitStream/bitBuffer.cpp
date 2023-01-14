#include<string>
#include"bitBuffer.h"
#include"bitException.h"
using namespace std;

/*--------------------------------------------
作用：
    指示一个字节的大小
--------------------------------------------*/ 
const int BitBuffer::ByteLen=8;

/*--------------------------------------------
作用：
    求解两数的最大公约数，辅助求缓冲区长度
参数：
    n1, n2 - 要求最大公约数的两个数
返回值：
    num - 两个数的最大公约数
--------------------------------------------*/ 
int BitBuffer::gcd(int n1, int n2){
    return n2==0? n1 : gcd(n2, n1%n2);
}

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
void BitBuffer::setBufferLength(){
    bufferLength=fitLength*ByteLen/gcd(fitLength, ByteLen);
    data=new char[bufferLength/ByteLen];
}

/*--------------------------------------------
作用：
    BitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
返回值：
    无
--------------------------------------------*/ 
BitBuffer::BitBuffer(int fitLength){
    this->fitLength=fitLength;
    if(fitLength!=0)
        setBufferLength();
}

/*--------------------------------------------
作用：
    获取缓冲区的适配长度，单位为bit
参数：
    无
返回值：
    fitLength - 缓冲区适配长度
--------------------------------------------*/ 
int BitBuffer::getFitLength(){
    return fitLength;
}

/*--------------------------------------------
作用：
    获取缓冲区的总长度，单位为bit
参数：
    无
返回值：
    bufferLength - 缓冲区适配长度
--------------------------------------------*/ 
int BitBuffer::getBufferLength(){
    return bufferLength;
}

/*--------------------------------------------
作用：
   设置一个未初始化的缓冲区的适配长度
参数：
    fitLength - 缓冲区适配长度
返回值：
    无
--------------------------------------------*/
void BitBuffer::setFitLength(int fitLength){
    if(this->fitLength!=0)
        throw ResetBufferLengthException();
    if(fitLength<=0)
        throw IllegalLengthException();
    this->fitLength=fitLength;
    setBufferLength();
}

/*--------------------------------------------
作用：
    - 调试时使用
    - 将缓冲区中的数据以01串的形式返回
参数：
    无
返回值：
    bits - 一个01串，表示缓冲区中的数据
--------------------------------------------*/
string BitBuffer::getBuffer(){
    char mask=1;
    string bits="";
    int position=0;
    for(int i=0;i<bufferLength;i++){
        if(i%ByteLen==0){
            mask=(1<<(ByteLen-1));
            if(i!=0) position++;
        }else if(i%ByteLen==1){
        	mask=(1<<(ByteLen-2));
		}
        if((data[position]&mask)!=0)bits.append("1");
        	else bits.append("0");
        mask=mask>>1;
    }
    return bits;
}

/*--------------------------------------------
作用：
    - 调试时使用
    - 将01串代表的数据写入缓冲区
参数：
    bits - 一个01串，表示要写入缓冲区的数据
返回值：
    bool - 设置是否成功
--------------------------------------------*/
bool BitBuffer::setBuffer(string bits){
    if(bits.length()!=bufferLength)
        return false;
    char now=0;
    int position=0;
    for(int i=0;i<bits.length();i++){
        if(bits[i]=='1')now+=(1<<(ByteLen-i%ByteLen-1));
        if(i%ByteLen==ByteLen-1){
            data[position]=now;
            now=0;
            position++;
        }
    }
    return true;
}

/*--------------------------------------------
作用：
    BitBuffer的析构函数
参数：
    无
返回值：
    无
--------------------------------------------*/
BitBuffer::~BitBuffer(){
    if(fitLength!=0)
        delete[] data;
}

// Debug - BitBuffer基类
// #include<iostream>
// int main(){
//     BitBuffer buffer;
//     buffer.setFitLength(12);
//     cout<<buffer.setBuffer("101010100101010111111111")<<endl;
//     cout<<buffer.getBuffer();
//     return 0;
// }