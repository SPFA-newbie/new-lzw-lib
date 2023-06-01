#define DEBUG_FOR_BITBUFFER

#include<string>
#include<cassert>
#include<fstream>
#include"bitString.h"
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
    bufferLength = fitLength * ByteLen / gcd(fitLength, ByteLen);
    data = new _Byte[bufferLength / ByteLen];
    memset(this->data, 0, bufferLength / ByteLen);
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
    if(pos>bufferLength || pos<0)
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
    将游标移动到下一个匹配位置（移动fitLength位）
参数：
    无
返回值：
    success - 移动是否成功
              失败表示游标已经在尾部
--------------------------------------------*/
bool BitBuffer::nextPosition(){
    if(position==bufferLength)return false;
    position+=fitLength;
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
    this->position=0;
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
_Byte BitBuffer::makeMask(int begin, int end){
    _Byte mask=0;
    for(int i=0;i<ByteLen;i++) {
        if(i>=begin && i<end)mask++;
        if(i!=ByteLen-1)mask<<=1;
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
void BitBuffer::setByteData(_Byte data, bool autoNext){
    if(position>bufferLength)
        throw BufferOverflowed();
    if(position+ByteLen>bufferLength)
        throw BufferOverflowed();

    if(position%ByteLen!=0){
        _Byte mask=makeMask(0, position%ByteLen);
        _Byte now=(this->data[position/ByteLen])&mask;
        _Byte leftData=data>>(position%ByteLen);
        _Byte rightData=data<<(ByteLen-position%ByteLen);
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
_Byte BitBuffer::getByteData(bool autoNext){
    if(position>bufferLength)
        throw BufferOverflowed();
    if(position+ByteLen>bufferLength)
        throw BufferOverflowed();
    
    _Byte data;
    if(position%ByteLen!=0){
        _Byte left=(this->data[position/ByteLen])<<(position%ByteLen);
        _Byte right=this->data[position/ByteLen+1]>>(ByteLen-position%ByteLen);
        _Byte mask=makeMask(0, ByteLen-position%ByteLen);
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
           任何不为0的值均认为是1
    autoNext - 写入后是否移动游标，默认值为true
返回值：
    无
--------------------------------------------*/
void BitBuffer::setBitData(_Byte data, bool autoNext){
    _Byte mask=makeMask(0, position%ByteLen)|makeMask(position%ByteLen+1, ByteLen);
    this->data[position/ByteLen]&=mask;
    if(data!=0)
        this->data[position/ByteLen]|=makeMask(position%ByteLen, position%ByteLen+1);
    if(autoNext)
        offsetPosition(1);
}

/*--------------------------------------------
作用：
    从缓冲区中读取一个比特的数据
参数：
    autoNext - 读取后是否移动游标，默认值为true
返回值：
    data - 读取到的一个字节的数据
           为0时返回0，为1时返回255
--------------------------------------------*/          
_Byte BitBuffer::getBitData(bool autoNext){
    _Byte data;
    if(((this->data[position/ByteLen])&makeMask(position%ByteLen,position%ByteLen+1))!=0){
        data=(_Byte)-1;
    }else data=(_Byte)0;
    if(autoNext)
        offsetPosition(1);
    return data;
}

/*--------------------------------------------
作用：
    向缓冲区内写入一个适合长度的数据
参数：
    data - 指向将要写入的数据的指针（_Byte*）
    autoNext - 写入后是否移动游标，默认值为true
返回值：
    无
--------------------------------------------*/
void BitBuffer::setFitData(_Byte* data, bool autoNext){
    int nowPosition = position;
    int completeByte = fitLength / ByteLen;
    for (int i = 0; i < completeByte; i++)
        setByteData(data[i]);
    int fragmentBit = fitLength % ByteLen;
    _Byte mask = makeMask(0, 1);
    for (int i = 0; i < fragmentBit; i++) {
        setBitData(data[completeByte] & mask);
        mask >>= 1;
    }
    if (!autoNext)
        position = nowPosition;
    return;
}

/*--------------------------------------------
作用：
    从缓冲区中读取一个适合长度的数据
参数：
    autoNext - 读取后是否移动游标，默认值为true
返回值：
    data - 一个指向读取到的数据的指针（_Byte*）
--------------------------------------------*/
_Byte* BitBuffer::getFitData(bool autoNext){
    int nowPosition = position;
    int completeByte = fitLength / ByteLen;
    int fragmentBit = fitLength % ByteLen;
    _Byte* ret = new _Byte[fragmentBit == 0 ? completeByte : completeByte + 1];
    
    for (int i = 0; i < completeByte; i++)
        ret[i] = getByteData();
    
    _Byte mask = makeMask(0, 1);

    if (fragmentBit != 0)
        ret[completeByte] = 0;
    for (int i = 0; i < fragmentBit; i++) {
        ret[completeByte] += mask & getBitData();
        mask >>= 1;
    }
    if (!autoNext)
        position = nowPosition;
    return ret;
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
    this->fitLength = fitLength;
    this->bufferLength = 0;
    this->data = nullptr;
    this->position = 0;
    if (fitLength != 0)
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
    _Byte mask=1;
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
    _Byte now=0;
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
//#include<iostream>
//int main(){
//    BitBuffer buffer;
//    buffer.setFitLength(12);
//    cout<<buffer.setBuffer("101010100101010111111000")<<endl;
//    cout<<buffer.getBuffer();
//    return 0;
//}

/*--------------------------------------------
作用：
    测试InputBitBuffer是否读取到了足够的字节
    或到达了文件尾
参数：
    无
返回值：
    isEnd - 是否读取了足够字节或到达了末尾
            到达时返回true
--------------------------------------------*/
bool InputBitBuffer::isEnd() {
    if (this->readLength == 0)
        return read.eof();
    return read.tellg() >= this->beginPosition + this->readLength;
}

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
    resetPosition();
    offsetPosition(getBufferLength());
}

/*--------------------------------------------
作用：
    InputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
    readFile - 绑定了文件的文件输入流
               将会从这个文件中读取字节流
    pos - 文件指针初始位置
    readLength - 读取的最大字节数
                 0表示读到末尾
返回值：
    无
--------------------------------------------*/
InputBitBuffer::InputBitBuffer(int fitLength, const char* readFile, ifstream::pos_type pos = 0, ifstream::off_type readLength = 0) : BitBuffer(fitLength){
    open(readFile, pos, readLength);
}

/*--------------------------------------------
作用：
    打开一个文件
    一个缓冲区只能打开一个文件
    打开多个文件会抛出异常
参数：
    readFile - 欲读取文件的文件路径
    pos - 文件指针的初始位置
    readLength - 读取的最大字节数
                 0表示读到末尾
返回值：
    无
--------------------------------------------*/
void InputBitBuffer::open(const char* readFile, ifstream::pos_type pos, ifstream::off_type readLength = 0) {
    if (read.is_open())
        throw MultipleOpenFileException();
    read.open(readFile, ios::in | ios::binary);
    read.seekg(pos);
    this->readLength = readLength;
    this->beginPosition = pos;

    resetPosition();
    offsetPosition(getBufferLength());
}

/*--------------------------------------------
作用：
   关闭打开的文件
   关闭未打开的文件会抛出异常
   关闭文件后缓冲区会被清空
参数：
    无
返回值：
    无
--------------------------------------------*/
void InputBitBuffer::close() {
    if (read.is_open() == false)
        throw ClosedFileException();
    resetPosition();
    read.close();
}

/*--------------------------------------------
作用：
    测试InputBitBuffer是否打开了文件
参数：
    无
返回值：
    result - 缓冲区是否打开了文件
             打开时返回true
--------------------------------------------*/
bool InputBitBuffer::isOpen() {
    return read.is_open();
}

/*--------------------------------------------
作用：
    测试InputBitBuffer是否完成全部读取任务
    即：
    从文件中读取出了规定的字节数
    且自身缓存的字节全部被释放
参数：
    无
返回值：
    result - 缓冲区是否完成读取任务
             完成时返回true
--------------------------------------------*/
bool InputBitBuffer::finish() {
    return isEnd() && getPosition() / ByteLen > lastByte;
}

/*--------------------------------------------
作用：
    将缓冲区中的数据导出到BitString中
参数：
    bits - 导出目标
返回值：
    this - 缓冲区自身
--------------------------------------------*/
InputBitBuffer& InputBitBuffer::operator>>(BitString& bits) {
    if (getPosition() == getBufferLength()) {
        resetPosition();
        lastByte = -1;
        for (int i = 0; i < getBufferLength() / ByteLen; i++) {
            if (isEnd() == false) {
                _Byte nextByte;
                read.read((char*)&nextByte, 1);
                setByteData(nextByte);
                lastByte = i;
            } else setByteData((_Byte)0);
        }
        resetPosition();
    }
    bits.data = getFitData();
    return *this;
}

/*--------------------------------------------
作用：
    InputBitBuffer的析构函数
参数：
    无
返回值：
    无
--------------------------------------------*/
InputBitBuffer::~InputBitBuffer() {
    read.close();
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
    
}

/*--------------------------------------------
作用：
    OutputBitBuffer的构造函数
参数：
    fitLength - 匹配的比特串的长度
                为0时表示未初始化
    writeFile - 将要进行输出的文件名
返回值：
    无
--------------------------------------------*/
OutputBitBuffer::OutputBitBuffer(int fitLength, const char* writeFile) : BitBuffer(fitLength) {

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
