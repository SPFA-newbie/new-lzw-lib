#include"bitBuffer.h"
#include"bitBufferControllerFlag.h"

/*--------------------------------------------
作用：
    记录控制字符对象的数量
--------------------------------------------*/
int BitBufferControllerFlag::idNum=0;

BitBufferControllerFlag::BitBufferControllerFlag(ControllerFunction work){
    id=idNum;
    idNum++;
    this->work=work;
}

const BitBufferControllerFlag refresh(
    [](OutputBitBuffer& out)->void{
        
    }
);

const BitBufferControllerFlag discard(
    [](OutputBitBuffer& out)->void{
        
    }    
);

