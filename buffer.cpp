//
// Created by wujy on 2017/11/15.
//

#include "buffer.h"
#include <iostream>

using namespace std;


BCB::BCB(int p, int f) :page_id(p),frame_id(f){

}

BMgr::BMgr(){
    //to be complemented
}

int BMgr::Hash(int page_id) {
    return page_id%BUFSIZE;
}

BCB* BMgr::ptob(int page_id) {
    return bcbs_[Hash(page_id)];
}

int BMgr::FixPage(int page_id) {
    BCB* bcb = ptob(page_id);
    int frame_id;

    //page alread in buffer?
    while(bcb!= nullptr){
        if(bcb->page_id==page_id)
            break;
    }
    if(bcb!= nullptr){
        frame_id = bcb->frame_id;
        lrulist_.remove(frame_id);
        lrulist_.push_front(frame_id);
        ftop_[frame_id] = page_id;
    }else{
        //any free frames?
        if(free_frame.empty()){
            frame_id = SelectVictim();
            bcb = new BCB(page_id,frame_id);
            lrulist_.remove(frame_id);
            lrulist_.push_front(frame_id);
        } else {
            //get free frame
            frame_id = free_frame.back();
            free_frame.pop_back();
            bcb = new BCB(page_id,frame_id);
        }
        buf_[frame_id] = dsmgr_.ReadPage(page_id);
    }
    return frame_id;
}

int BMgr::SelectVictim() {
    int victim = lrulist_.back();
    lrulist_.pop_back();
    BCB* bcb = ptob(ftop_[victim]);
    while(bcb->frame_id!=victim)
        bcb = bcb->next;
    if(bcb->dirty){
        dsmgr_.WritePage(bcb->page_id,buf_[victim]);
    }
    RemoveBCB(bcb);
    return victim;
}

void BMgr::SetDirty(int frame_id) {
    BCB* bcb = ptob(ftop_[frame_id]);
    while(bcb!= nullptr&&bcb->frame_id!=frame_id)
        bcb = bcb->next;
    if(bcb== nullptr) {
        cerr<<"no such frame!"<<endl;
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
                    dsmgr_.WritePage(bcb->page_id,buf_[bcb->frame_id]);
                bcb = bcb->next;
            }
        }
    }
    dsmgr_.CloseFile();
}

void BMgr::PrintFrame(int frame_id) {
    //to be completed
}

int BMgr::NumFreeFrames() {
    return (int)free_frame.size();
}

void BMgr::RemoveBCB(BCB *bcb) {
    BCB* head = ptob(bcb->page_id);
    if(head==bcb){
        head==bcb->next;
        delete bcb;
    } else {
        while(head->next!=bcb)
            head = head->next;
        head->next = bcb->next;
        delete bcb;
    }
}