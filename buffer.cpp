//
// Created by wujy on 2017/11/15.
//

#include "buffer.h"
#include <cstdio>

using namespace std;


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
/*
BCB* BMgr::AllocFrame(int page_id) {
    int frame_id;
    BCB* bcb;
    if(free_frame_.empty()){
        frame_id = SelectVictim();
        RemoveLRUEle(frame_id);
    } else {
        //get free frame
        frame_id = free_frame_.back();
        free_frame_.pop_back();
    }
    bcb = new BCB(page_id,frame_id);
    lrulist_.push_front(frame_id);
    ftob_[frame_id] = bcb;
    ftop_[frame_id] = page_id;
    //if first bcb in bucket is victim and be removed
    if(ptob(page_id)== nullptr)
        bcbs_[Hash(page_id)] = bcb;
    buf_[frame_id] = dsmgr.ReadPage(page_id);
    return bcb;
}
*/
BCB* BMgr::AllocFrame(int page_id) {
    int frame_id;
    BCB* bcb = ptob(page_id);
    if(free_frame_.empty()){
        frame_id = SelectVictim();
        RemoveLRUEle(frame_id);
    } else {
        //get free frame
        frame_id = free_frame_.back();
        free_frame_.pop_back();
    }

    BCB* newBcb = new BCB(page_id,frame_id);
    if(bcb== nullptr) bcbs_[Hash(page_id)] = newBcb;
    else {
        while(bcb->next!= nullptr)
            bcb = bcb->next;
        bcb->next = newBcb;
    }
    ftob_[frame_id] = newBcb;
    lrulist_.push_front(frame_id);
    buf_[frame_id] = dsmgr.ReadPage(page_id);
    return newBcb;
}

int BMgr::FixPage(int page_id) {
    BCB* bcb = ptob(page_id);
    int frame_id;
    while(bcb!= nullptr){
        if(bcb->page_id==page_id) break;
        bcb = bcb->next;
    }
    if(bcb!= nullptr){
        frame_id = bcb->frame_id;
        RemoveLRUEle(frame_id);
        lrulist_.push_front(frame_id);
    } else {
        bcb = AllocFrame(page_id);
        frame_id = bcb->frame_id;
    }
    if(++bcb->count==1) bcb->latch= true;
    return frame_id;
}
/*
int BMgr::FixPage(int page_id) {
    BCB* bcb = ptob(page_id);
    int frame_id;
    //page alread in buffer?
    if(bcb== nullptr){
        bcbs_[Hash(page_id)] = AllocFrame(page_id);
        bcb = ptob(page_id);
        frame_id = bcb->frame_id;
    } else {
        while (bcb->next!= nullptr) {
            if (bcb->page_id == page_id)
                break;
            bcb = bcb->next;
        }
        if (bcb->page_id == page_id) {
            frame_id = bcb->frame_id;
            RemoveLRUEle(frame_id);
            lrulist_.push_front(frame_id);
        } else {
            //any free frames?
            bcb->next = AllocFrame(page_id);
            bcb = bcb->next;
            frame_id = bcb->frame_id;
        }
    }
    if(++bcb->count==1) bcb->latch= true;
    return frame_id;
}
 */



NewPage BMgr::FixNewPage(){
    NewPage np;
    int page_id;
    int num_pages = dsmgr.GetNumPages();
    for(page_id = 0;page_id<num_pages;page_id++){
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
    //int victim = lrulist_.back();
    //lrulist_.pop_back();
    list<int>::iterator it = --lrulist_.end();     //search victim whitch lauch is false from end of lru
    BCB* bcb;
    while(true){
        bcb = ftob_[*it];
        if(bcb->latch) --it;
        else {
            int victim = *it;
            if(bcb->dirty)
                dsmgr.WritePage(bcb->page_id,buf_[victim]);
            RemoveBCB(bcb);
            return victim;
        }
    }
    /*  no latch
    BCB* bcb = ptob(ftop_[victim]);
    while(bcb->frame_id!=victim)
        bcb = bcb->next;
    if(bcb->dirty){
        dsmgr.WritePage(bcb->page_id,buf_[victim]);
    }
    RemoveBCB(bcb);
    return victim;
     */
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
                if(bcb->dirty==1)
                    dsmgr.WritePage(bcb->page_id,buf_[bcb->frame_id]);
                bcb = bcb->next;
            }
        }
    }
    dsmgr.CloseFile();
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