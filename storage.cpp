//
// Created by wujy on 2017/11/16.
//

#include "storage.h"
#include <cstdio>

DSMgr::DSMgr():basePages_((MAXPAGES+PAGESIZE-1)/PAGESIZE){
    printf("Max pages:%d, buffer size: %d, frame size: %d\n",MAXPAGES,BUFSIZE,FRAMESIZE);
    printf("Base pages for usage array: %d\n",basePages_);
}

int DSMgr::OpenFile(const char* filename) {
    currFile_ = fopen(filename,"r+");
    if(currFile_== nullptr) return -1;
    long fileSize = ftell(currFile_);
    if(fileSize==0){                         //new file, create use array
        numPages_ = 0;
        return 0;
    }
    numPages_ = (fileSize+PAGESIZE-1)/PAGESIZE - basePages_;
    fread(pages_,numPages_,1,currFile_);     //read usage array
    return 0;
}

int DSMgr::Seek(long offset) {
    return fseek(currFile_,offset,SEEK_SET);
}

int DSMgr::CloseFile() {
    Seek(0);
    fwrite(pages_,numPages_,1,currFile_);         //write usage array into file
    fclose(currFile_);
    return 0;
}

int DSMgr::CreateNPagesFile(int n, const char *filename) {
    FILE* file = fopen(filename,"w");
    if(file== nullptr) return -1;
    char usage[MAXPAGES];
    for(int i = 0;i<n;i++)
        usage[i] = '1';
    fwrite(usage,MAXPAGES,1,file);            //write base pages
    fseek(file,PageOffset(0),SEEK_SET);      //seek to begin of pages
    for(int i = 0;i<MAXPAGES;i++){           // write pages
        fwrite("1",1,PAGESIZE,file);
    }
    fclose(file);
    return 0;
}

long DSMgr::PageOffset(int page_id) {
    return (page_id+basePages_)*PAGESIZE;
}

bFrame DSMgr::ReadPage(int page_id) {
    bFrame frm;
    //find page
    Seek(PageOffset(page_id));
    fread(frm.filed,PAGESIZE,1,currFile_);
    return frm;
}

int DSMgr::WritePage(int page_id, bFrame frm) {
    //find page
    Seek(PageOffset(page_id));
    int bytes = fwrite(frm.filed,PAGESIZE,1,currFile_);
    fflush(currFile_);
    return bytes;
}

FILE* DSMgr::GetFile(){
    return currFile_;
}

void DSMgr::IncNumPages(){
    pages_[numPages_] = '0';
    numPages_++;
}

int DSMgr::GetNumPages(){
    return numPages_;
}

void DSMgr::SetUse(int page_id, char use_bit) {
    pages_[page_id] = use_bit;
}

char DSMgr::GetUse(int page_id) {
    return pages_[page_id];
}

