//
// Created by wujy on 2017/11/15.
//

#include "buffer.h"
#include <cstdio>

using namespace std;

int readPage = 0;             //记录读IO
int writeDirty = 0;           //记录写IO
int hit = 0;                  //记录buffer命中

BCB::BCB(int p, int f) :page_id(p),frame_id(f),next(nullptr),dirty(0),latch(false),count(0){
}

BCB::BCB():page_id(-1),frame_id(-1),next(nullptr),dirty(0),latch(false),count(0){}

BMgr::BMgr(){
    for(int i = 0;i<BUFSIZE;i++) {
        free_frame_.push_back(i);        //init free frame
    }
    //to be complemented
}

int BMgr::Hash(int page_id) {
    return page_id%BUFSIZE;
}

BCB* BMgr::ptob(int page_id) {
    return bcbs_[Hash(page_id)];
}

void BMgr::RemoveLRUEle(int frame_id) {
    lrulist_.remove(frame_id);
}

BCB* BMgr::AllocFrame(int page_id) {
    int frame_id;
    if(free_frame_.empty()){
        frame_id = SelectVictim();
        RemoveLRUEle(frame_id);
    } else {
        //get free frame
        frame_id = free_frame_.back();
        free_frame_.pop_back();
    }
    BCB* bcb = ptob(page_id);
    BCB* newBcb = new BCB(page_id,frame_id);
    if(bcb== nullptr) {
        bcbs_[Hash(page_id)] = newBcb;
        bcb = bcbs_[Hash(page_id)];
    }
    else {
        while(bcb->next!= nullptr)
            bcb = bcb->next;
        bcb->next = newBcb;
        bcb = bcb->next;
    }
    ftob_[frame_id] = bcb;
    lrulist_.push_front(frame_id);
    buf_[frame_id] = dsmgr.ReadPage(page_id);
    ++readPage;
    return bcb;
}

int BMgr::FixPage(int page_id) {
    BCB* bcb = ptob(page_id);
    int frame_id;
    while(bcb!= nullptr){
        if(bcb->page_id==page_id) break;
        bcb = bcb->next;
    }
    if(bcb!= nullptr){
        hit++;
        frame_id = bcb->frame_id;
        RemoveLRUEle(frame_id);
        lrulist_.push_front(frame_id);
    } else {
        bcb = AllocFrame(page_id);             //为page分配frame
        frame_id = bcb->frame_id;
    }
    if(++bcb->count==1) bcb->latch= true;
    return frame_id;
}

NewPage BMgr::FixNewPage(){
    NewPage np;
    int page_id;
    int num_pages = dsmgr.GetNumPages();
    for(page_id = 0;page_id<num_pages;page_id++){   //寻找未使用的page
        if(dsmgr.GetUse(page_id)=='0')
            break;
    }
    if(page_id==num_pages) {
        dsmgr.IncNumPages();
        dsmgr.SetUse(page_id,'1');
    }
    np.page_id = page_id;
    np.frame_id = FixPage(page_id);
    return np;
}

int BMgr::UnfixPage(int page_id) {
    BCB* bcb = ptob(page_id);
    while(bcb!= nullptr&&bcb->page_id!=page_id) bcb = bcb->next;
    if(--(bcb->count)==0) bcb->latch = false;
    return bcb->frame_id;
}

int BMgr::SelectVictim() {

    list<int>::iterator it = --lrulist_.end();   //从lru末尾开始寻找latch为false的frame
    BCB* bcb;
    while(true){
        bcb = ftob_[*it];
        if(bcb->latch) --it;
        else {
            int victim = *it;
            if(bcb->dirty) {
                dsmgr.WritePage(bcb->page_id, buf_[victim]);
                ++writeDirty;
            }
            RemoveBCB(bcb);                                     //删除该frame的BCB
            return victim;
        }
    }
}

void BMgr::SetDirty(int frame_id) {
    BCB* bcb = ftob_[frame_id];
    if(bcb== nullptr) {
        return;
    }
    bcb->dirty = 1;
}

//Write all dirty frames into storage
void BMgr::WriteDirtys() {
    BCB* bcb;
    for(int i = 0;i<BUFSIZE;i++){
        if(bcbs_[i]!= nullptr){
            bcb = bcbs_[i];
            while(bcb!= nullptr){
                if(bcb->dirty==1) {
                    dsmgr.WritePage(bcb->page_id, buf_[bcb->frame_id]);
                    writeDirty++;
                }
                bcb = bcb->next;
            }
        }
    }
}

void BMgr::PrintFrame(int frame_id) {
    printf("%s\n",buf_[frame_id].filed);
}

int BMgr::NumFreeFrames() {
    return (int)free_frame_.size();
}

void BMgr::RemoveBCB(BCB *bcb) {
    BCB* head = ptob(bcb->page_id);
    if(head==bcb){
        bcbs_[Hash(bcb->page_id)]=bcb->next;
        delete bcb;
    } else {
        while(head->next!=bcb)
            head = head->next;
        head->next = bcb->next;
        delete bcb;
    }
}