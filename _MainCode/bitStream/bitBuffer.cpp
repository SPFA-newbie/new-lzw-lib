#include<string>
#include<fstream>
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
    设置游标位置
参数：
    pos - 目标位置，以位计，从0开始编号
返回值：
    无
--------------------------------------------*/        
void BitBuffer::setPosition(int pos){
    if(pos>bufferLength)
        throw BufferOverflowed();
    position=pos;
}

/*--------------------------------------------
作用：
    获取游标位置
参数：
    position - 游标当前位置，以位计，从0开始编号
返回值：
    无
--------------------------------------------*/
int BitBuffer::getPosition(){
    return position;
}

/*--------------------------------------------
作用：
    将游标偏移一段距离
参数：
    offset - 进行偏移的距离
             为负数时代表向左偏移
返回值：
    无
--------------------------------------------*/
void BitBuffer::offsetPosition(int offset){
    if(position+offset>bufferLength)
        throw BufferOverflowed();
    if(position+offset<0)
        throw BufferOverflowed();
    position+=offset;
}

/*--------------------------------------------
作用：
    将游标移动到下一个位置
参数：
    无
返回值：
    hasNext - 是否还能继续移动
              失败表示游标已经移动到尾部
--------------------------------------------*/
bool BitBuffer::nextPosition(){
    if(position==bufferLength)return false;
    position+=fitLength;
    if(position==bufferLength)return false;
    return true;
}

/*--------------------------------------------
作用：
    将游标移动到缓冲区头部
参数：
    无
返回值：
    无
--------------------------------------------*/
void BitBuffer::resetPosition(){
    position=0;
}

/*--------------------------------------------
作用：
    生成一个1字节长度的掩码
    用于获取某个字符中连续的几位
参数：
    begin - 欲提取部分的开头位序号
    end - 欲提取部分的结尾位序号
    序号从0开始计算，第0位为最高位
    规定的区间为前闭后开区间
    当begin≥end时，掩码将会被构建成为0
返回值：
    mask - 构造得到的掩码
--------------------------------------------*/
char BitBuffer::makeMask(int begin, int end){
    if(begin>=end)return (char)0;
    char mask=0;
    bool bit0Mask=(begin==0);

    if(bit0Mask)begin++;
    for(int i=begin;i<end;i++)
        mask+=(1<<(ByteLen-i-1));

    if(bit0Mask){
        char tool=0;
        for(int i=1;i<ByteLen;i++)
            tool+=(1<<(i-1));
        mask|=((-1)^tool);
    }
    return mask;
}

/*--------------------------------------------
作用：
    向缓冲区内写入一个字节的数据
参数：
    data - 将要写入的数据
    autoNext - 写入后是否移动游标，默认值为true
返回值：
    无
--------------------------------------------*/
void BitBuffer::setByteData(char data, bool autoNext){
    if(position>bufferLength)
        throw BufferOverflowed();
    if(position+ByteLen>bufferLength)
        throw BufferOverflowed();

    if(position%ByteLen!=0){
        char mask=makeMask(0, position%ByteLen);
        char now=(this->data[position/ByteLen])&mask;
        char leftData=data>>(position%ByteLen);
        char rightData=data<<(ByteLen-position%ByteLen);
        leftData&=(~mask);
        rightData&=mask;
        
        this->data[position/ByteLen]=now|leftData;
        this->data[position/ByteLen+1]=rightData;
    }else this->data[position/ByteLen]=data;

    if(autoNext)
        offsetPosition(ByteLen);
}

/*--------------------------------------------
作用：
    从缓冲区中读取一个字节的数据
参数：
    autoNext - 读取后是否移动游标，默认值为true
返回值：
    data - 读取到的一个字节的数据
--------------------------------------------*/        
char BitBuffer::getByteData(bool autoNext=true){
    if(position>bufferLength)
        throw BufferOverflowed();
    if(position+ByteLen>bufferLength)
        throw BufferOverflowed();
    
    char data;
    if(position%ByteLen!=0){
        char left=(this->data[position/ByteLen])<<(position%ByteLen);
        char right=this->data[position/ByteLen+1]>>(ByteLen-position%ByteLen);
        char mask=makeMask(0, ByteLen-position%ByteLen);
        data=(left&mask)|(right&(~mask));
    }else data=this->data[position/ByteLen];

    if(autoNext)
        offsetPosition(ByteLen);
    return data;
}

/*--------------------------------------------
作用：
    向缓冲区内写入一个比特的数据
参数：
    data - 将要写入的数据
    autoNext - 写入后是否移动游标，默认值为true
返回值：
    无
--------------------------------------------*/
void BitBuffer::setBitData(char data, bool autoNext){

}

/*--------------------------------------------
作用：
    从缓冲区中读取一个比特的数据
参数：
    autoNext - 读取后是否移动游标，默认值为true
返回值：
    data - 读取到的一个字节的数据
--------------------------------------------*/        
char BitBuffer::getBitData(bool autoNext){

}

/*--------------------------------------------
作用：
    向缓冲区内写入一个适合长度的数据
参数：
    data - 指向将要写入的数据的指针（char*）
    autoNext - 写入后是否移动游标，默认值为true
返回值：
    无
--------------------------------------------*/
void BitBuffer::setFitData(char* data, bool autoNext){

}

/*--------------------------------------------
作用：
    从缓冲区中读取一个适合长度的数据
参数：
    autoNext - 读取后是否移动游标，默认值为true
返回值：
    data - 一个指向读取到的数据的指针（char*）
--------------------------------------------*/
char* BitBuffer::getFitData(bool autoNext){

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


/*--------------------------------------------
作用：
    InputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
返回值：
    无
--------------------------------------------*/
InputBitBuffer::InputBitBuffer(int fitLength) : BitBuffer(fitLength){
    read=nullptr;
}

/*--------------------------------------------
作用：
    InputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
    readDocument - 绑定了文件的文件输入流
                   将会从这个文件中读取字节流
返回值：
    无
--------------------------------------------*/
InputBitBuffer::InputBitBuffer(int fitLength, ifstream& readDocument) : BitBuffer(fitLength){
    read=(&readDocument);
}

/*--------------------------------------------
作用：
    将一个输入文件流与InputBitBuffer连接
    一个缓冲区只能有一个连接
    在断开原有连接前，重复连接会抛出异常
参数：
    readDocument - 绑定了文件的文件输入流
                   将会从这个文件中读取字节流
返回值：
    无
--------------------------------------------*/
void InputBitBuffer::connectStream(ifstream& readDocument){
    if(read!=nullptr)
        throw MultipleConnectionException();
    read=(&readDocument);
}

/*--------------------------------------------
作用：
    断开InputBitBuffer与输入文件流的连接
    断开不存在的连接会抛出异常
参数：
    closeStream - 是否在断开连接的同时关闭文件流
                  默认值为true
返回值：
    无
--------------------------------------------*/
void InputBitBuffer::disconnectStream(bool closeStream){
    if(read==nullptr)
        throw UnconnectException();
    if(closeStream)
        read->close();
    read=nullptr;
}

/*--------------------------------------------
作用：
    测试InputBitBuffer是否连接了文件流
参数：
    无
返回值：
    result - 缓冲区是否连接了文件流
             连接时返回true
--------------------------------------------*/
bool InputBitBuffer::isStreamConnected(){
    return read!=nullptr;
}



/*--------------------------------------------
作用：
    OutputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
返回值：
    无
--------------------------------------------*/
OutputBitBuffer::OutputBitBuffer(int fitLength) : BitBuffer(fitLength){
    write=nullptr;
}

/*--------------------------------------------
作用：
    OutputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
    writeDocument - 绑定了文件的文件输出流
                    将会向这个文件中写入字节流
返回值：
    无
--------------------------------------------*/
OutputBitBuffer::OutputBitBuffer(int fitLength, ofstream& writeDocument) : BitBuffer(fitLength){
    write=(&writeDocument);
}

/*--------------------------------------------
作用：
    将一个输出文件流与OutputBitBuffer连接
    一个缓冲区只能有一个连接
    在断开原有连接前，重复连接会抛出异常
参数：
    writeDocument - 绑定了文件的文件输出流
                    将会向这个文件中写入字节流
返回值：
    无
--------------------------------------------*/
void OutputBitBuffer::connectStream(ofstream& writeDocument){
    if(write!=nullptr)
        throw MultipleConnectionException();
    write=(&writeDocument);
}

/*--------------------------------------------
作用：
    断开OutputBitBuffer与输出文件流的连接
    断开时并不会输出缓冲区中已有的内容
    断开不存在的连接会抛出异常
参数：
    closeStream - 是否在断开连接的同时关闭文件流
                  默认值为true
返回值：
    无
--------------------------------------------*/
void OutputBitBuffer::disconnectStream(bool closeStream){
    if(write==nullptr)
        throw UnconnectException();
    if(closeStream)
        write->close();
    write=nullptr;
}

/*--------------------------------------------
作用：
    测试OutputBitBuffer是否连接了文件流
参数：
    无
返回值：
    result - 缓冲区是否连接了文件流
             连接时返回true
--------------------------------------------*/
bool OutputBitBuffer::isStreamConnected(){
    return write!=nullptr;
}