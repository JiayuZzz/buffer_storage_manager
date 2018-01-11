//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_BUFFER_H
#define LAB_BUFFER_H

#include <list>
#include <vector>
#include "storage.h"


struct BCB{
    BCB(int page_id, int frame_id);
    BCB();
    int page_id;
    int frame_id;
    bool latch;
    int count;
    char dirty;
    BCB *next;
};

struct NewPage{
    int page_id;
    int frame_id;
};

class BMgr{
public:
    BMgr();
    int FixPage(int page_id);         // 将数据库文件的指定页读入内存，为其分配frame
    NewPage FixNewPage();             // allocate a new page to record, and return page_id and frame_id
    int UnfixPage(int page_id);       // 
    int NumFreeFrames();              //返回空闲frame数量
    void SetDirty(int frame_id);      //将frame标记dirty，即内容被修改过，需要写回磁盘
    void UnsetDirty(int frame_id);    //取消frame的dirty位
    void WriteDirtys();               //将所有标记了dirty的frame写回磁盘
    void PrintFrame(int frame_id);    //输出frame内容
    DSMgr dsmgr;                      //为方便起见将DSMgr放在public里
private:
    //internal functions
    int SelectVictim();               //从LRU链表中选择一个frame淘汰
    void RemoveBCB(BCB* bcb);         //从BCB数组中删除指定BCB
    void RemoveLRUEle(int frame_id);  //从LRU链表中移除frame id
    BCB* ptob(int page_id);           //返回page id所对应的BCB
    int Hash(int page_id);            //将page id映射到BCB数组中的链表位置
    BCB* AllocFrame(int page_id);     //为page id分配frame

    BCB* bcbs_[BUFSIZE];              //BCB数组，每个元素是一个BCB链表
    BCB* ftob_[BUFSIZE];              //将frame映射到所在BCB
    std::list<int> lrulist_;          //lru链表，保存已使用的frame
    std::vector<int> free_frame_;     //空闲frame列表
    bFrame buf_[BUFSIZE];             //buffer
};

#endif //LAB_BUFFER_H
