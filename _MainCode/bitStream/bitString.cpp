#include<string>
#include"bitString.h"
#include"bitException.h"
using namespace std;

/*--------------------------------------------
作用：
    指示一个字节的大小
--------------------------------------------*/ 
const int BitString::ByteLen=8;

/*--------------------------------------------
作用：
    BitString的构造函数
参数：
    bitLength - 比特串的长度，为0时表示未初始化
返回值：
    无
--------------------------------------------*/ 
BitString::BitString(int bitLength){
    this->bitLength=bitLength;
    if(bitLength<0)
        throw new IllegalLengthException();
    if(bitLength!=0)
        this->data=new _Byte[getByteLen(bitLength)];
}

/*--------------------------------------------
作用：
    设置一个未初始化的BitString类的比特串长度
参数：
    bitLength - 比特串的长度
返回值：
    无
--------------------------------------------*/ 
void BitString::setLength(int bitLength){
    if(this->bitLength!=0)
        throw new ResetStringLengthException();
    if(bitLength<=0)
        throw new IllegalLengthException();
    this->bitLength=bitLength;
    this->data=new _Byte[getByteLen(bitLength)];
}

/*--------------------------------------------
作用：
    获取BitString类的比特串长度
参数：
    无
返回值：
    bitLength - 比特串长度，为0时表示未初始化
--------------------------------------------*/
int BitString::getLength(){
    return this->bitLength;
}

/*--------------------------------------------
作用：
    - 重载比较相关的运算符
    - 当一个BitString为另一个的前缀时，它更小
参数：
    bstr - 欲进行比较的另一个BitString
返回值：
    bool - 比较的结果
--------------------------------------------*/
bool BitString::operator>(BitString bstr){
    int len = getLength();
    if(len > bstr.getLength())
        len=bstr.getLength();
    for(int i=0;i<getByteLen(len);i++)
        if(data[i]!=bstr.data[i])
            return data[i]>bstr.data[i];
    return getLength()>bstr.getLength();
}
bool BitString::operator<(BitString bstr){
    return !((*this)>=bstr);
}
bool BitString::operator>=(BitString bstr){    
    return (((*this)>bstr) || ((*this)==bstr));
}
bool BitString::operator<=(BitString bstr){
    return !((*this)>bstr);
}
bool BitString::operator==(BitString bstr){
    if(getLength()!=bstr.getLength())return false;
    int len=getLength();
    for(int i=0;i<getByteLen(len);i++)
        if(data[i]!=bstr.data[i])
            return false;
    return true;
}
bool BitString::operator!=(BitString bstr){
    return !((*this)==bstr);
}

/*--------------------------------------------
作用：
    - 调试时使用
    - 将比特串中的数据以01串的形式返回
参数：
    无
返回值：
    bits - 一个01串，表示比特串中的数据
--------------------------------------------*/
string BitString::getBitString(){
    _Byte mask=1<<(ByteLen-1);
    int position=-1;
    string s="";
    for(int i=0;i<bitLength;i++) {
        if(i%ByteLen==0)position++;
		if((mask&data[position])!=0)s=s+"1";
            else s=s+"0";
        mask>>=1;
        if(mask==0)
            mask=1<<(ByteLen-1);
    }
    return s;
}

/*--------------------------------------------
作用：
    - 调试时使用
    - 将01串代表的数据写入比特串
参数：
    bits - 一个01串，表示要写入比特串的数据
返回值：
    bool - 设置是否成功
--------------------------------------------*/
bool BitString::setBitString(string bits){
    if(bits.length()!=bitLength)
        return false;
    _Byte now=0;
    int position=0;
    for(int i=0;i<bits.length();i++){
        now<<=1;
        if(bits[i]=='1') now++;
        position++;
        if(position%ByteLen==0) {
            data[position/ByteLen-1]=now;
            now=0;
        }
    }
    if(position%ByteLen!=0)
        data[position/ByteLen]=now<<(ByteLen-position%ByteLen);
    return true;
}

/*--------------------------------------------
作用：
    BitString的析构函数
参数：
    无
返回值：
    无
--------------------------------------------*/
BitString::~BitString(){
    if(bitLength!=0)
        delete[] data;
}

// Debug
// #include<iostream>
// int main(){
//     BitString bitstr;
//     bitstr.setLength(26);

//     cout<<bitstr.setBitString("10101010010101011111111110")<<endl;
//     cout<<bitstr.getBitString()<<endl;

//     BitString bitstr2;
//     bitstr2.setLength(28);
//     bitstr2.setBitString("1010101001010101111111111011");
//     cout<<(bitstr>=bitstr2);//true 
//     cout<<(bitstr<bitstr2);//false
//     return 0;
// }
