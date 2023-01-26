#pragma once

class OutputBitBuffer;
typedef void(*ControllerFunction)(OutputBitBuffer&);

class BitBufferControllerFlag{
    private:
/*--------------------------------------------
作用：
    记录控制字符对象的数量
--------------------------------------------*/
        static int idNum;
/*--------------------------------------------
作用：
    当前控制字符对象的id
--------------------------------------------*/
        int id;
    public:
        ControllerFunction work;
        BitBufferControllerFlag(ControllerFunction work);
};

namespace bstr{
    extern const BitBufferControllerFlag refresh;
    extern const BitBufferControllerFlag discard;
};