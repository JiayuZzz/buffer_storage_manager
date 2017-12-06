//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_BUFFER_H
#define LAB_BUFFER_H

#include <list>
#include <vector>
#include "storage.h"

#define BUFSIZE 1024

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
    int FixPage(int page_id);
    NewPage FixNewPage();           // allocate a new page to record, and return page_id and frame_id
    int UnfixPage(int page_id);
    int NumFreeFrames();
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);
    DSMgr dsmgr;                  //dsmgr
private:
    //internal functions
    int SelectVictim();
    void RemoveBCB(BCB* bcb);
    void RemoveLRUEle(int frame_id);
    BCB* ptob(int page_id);         //hash page_id to bcb
    int Hash(int page_id);
    BCB* AllocFrame(int page_id);

    BCB* bcbs_[BUFSIZE];            //block control block
    BCB* ftob_[BUFSIZE];            //map frame_id to bcb
    std::list<int> lrulist_;       //used frames
    std::vector<int> free_frame_;  //free frames
    bFrame buf_[BUFSIZE];         //buffer
};

#endif //LAB_BUFFER_H
