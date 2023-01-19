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
        this->data=new char[getByteLen(bitLength)];
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
    this->data=new char[getByteLen(bitLength)];
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
    - 比较为逐位比较
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
    for(int i=0;i<len;i++)
        if(data[i]!=bstr.data[i]){
            if(bstr.data[i]<0){
                if(data[i]<bstr.data[i])return true;
                    else return false;
            }else{
                if(data[i]<bstr.data[i])return false;
                    else return true;
            }
        }
    return getLength()>bstr.getLength();
}
bool BitString::operator<(BitString bstr){
    return !((*this)>=bstr);
}
bool BitString::operator>=(BitString bstr){
    int len = getLength();
    if(len > bstr.getLength())
        len=bstr.getLength();
    for(int i=0;i<len;i++)
        if(data[i]!=bstr.data[i]){
            if(bstr.data[i]<0){
                if(data[i]<bstr.data[i])return true;
                    else return false;
            }else{
                if(data[i]<bstr.data[i])return false;
                    else return true;
            }
        }
    return getLength()>=bstr.getLength();
}
bool BitString::operator<=(BitString bstr){
    return !((*this)>bstr);
}
bool BitString::operator==(BitString bstr){
    if(getLength()!=bstr.getLength())return false;
    int len=getLength();
    for(int i=0;i<len;i++)
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
    char mask=1;
    string bits="";
    int position=0;
    for(int i=0;i<bitLength;i++){
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
    - 将01串代表的数据写入比特串
参数：
    bits - 一个01串，表示要写入比特串的数据
返回值：
    bool - 设置是否成功
--------------------------------------------*/
bool BitString::setBitString(string bits){
    if(bits.length()!=bitLength)
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
//     cout<<bitstr.setBitString("10101010010101011111111100")<<endl;
//     cout<<bitstr.getBitString();
//     return 0;
// }
