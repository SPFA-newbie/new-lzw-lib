#pragma once

#include<exception>
using namespace std;

class BitOperationException : public exception{
    public:
        char* what(){return "进行比特操作时发生异常";}
};

//长度相关异常
class ResetStringLengthException : public BitOperationException{
    public:
        char* what(){return "试图重新设置比特串长度";}
};
class ResetBufferLengthException : public BitOperationException{
    public:
        char* what(){return "试图重新设置比特串缓冲区长度";}
};
class IllegalLengthException : public BitOperationException{
    public:
        char* what(){return "设置的长度不合法 (小于0或等于0) ";}
};
class MismatchLengthException : public BitOperationException{
    public:
        char* what(){return "缓冲区与比特串长度不匹配";}
};

//文件流连接相关异常
class MultipleConnectionException : public BitOperationException{
    public:
        char* what(){return "试图为缓冲区创建多个连接";}
};
class UnconnectException : public BitOperationException{
    public:
        char* what(){return "对未连接的缓冲区进行流操作";}
};

//输入输出相关异常
class BufferOverflowed : public BitOperationException{
    public:
        char* what(){return "游标指向缓冲区之外";}
};