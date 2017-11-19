//
// Created by wujy on 2017/11/16.
//

#include "storage.h"
#include <cstdio>
#include <unistd.h>

DSMgr::DSMgr(){
    //to be completed
}

int DSMgr::OpenFile(std::string filename) {
    //to be completed
}

int DSMgr::Seek(long offset) {
    return fseek(currFile_,offset,SEEK_SET);
}

int DSMgr::CloseFile() {
    fclose(currFile_);
    return 0;
}

long DSMgr::PageOffset(int page_id) {
    return page_id*PAGESIZE;
}

bFrame DSMgr::ReadPage(int page_id) {
    bFrame frm;
    //find page
    Seek(PageOffset(page_id));
    fread(frm.filed,FRAMESIZE,1,currFile_);
    return frm;
}

int DSMgr::WritePage(int page_id, bFrame frm) {
    //find page
    Seek(PageOffset(page_id));
    int bytes = fwrite(frm.filed,FRAMESIZE,1,currFile_);
    fflush(currFile_);
    return bytes;
}

FILE* DSMgr::GetFile(){
    return currFile_;
}

void DSMgr::IncNumPages(){
    pages_.push_back(0);
    numPages_++;
}

int DSMgr::GetNumPages(){
    return numPages_;
}

void DSMgr::SetUse(int page_id, char use_bit) {
    pages_[page_id] = use_bit;
}

int DSMgr::GetUse(int page_id) {
    return pages_[page_id];
}

