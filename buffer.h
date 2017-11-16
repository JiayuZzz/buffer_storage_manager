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
    int page_id;
    int frame_id;
    int latch;
    int count;
    int dirty;
    BCB *next;
};

class BMgr{
public:
    BMgr();
    int FixPage(int page_id);
    void FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();
private:
    //internal functions
    int SelectVictim();
    int Hash(int page_id);
    void RemoveBCB(BCB* bcb);
    void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);

    int ftop_[BUFSIZE];             //frame_id to page_id
    BCB* ptof_[BUFSIZE];            //page_ed to bcb
    std::list<int> lrulist_;       //used frames
    std::vector<int> free_frame;  //free frames
    bFrame buf_[BUFSIZE];         //buffer
    DSMgr dsmgr_;                  //dsmgr
};

#endif //LAB_BUFFER_H
