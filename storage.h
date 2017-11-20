//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_STORAGE_H
#define LAB_STORAGE_H

#include <cstdio>
#include <vector>

#define FRAMESIZE 4096
#define PAGESIZE 4096
#define MAXPAGES 50000

struct bFrame{
    char filed[FRAMESIZE];
};

class DSMgr{
public:
    DSMgr();
    int OpenFile(const char* filename);
    int CloseFile();
    int CreateNPagesFile(int n,const char* filename);        //create a n pages file for experiment
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    int Seek(long offset);
    FILE* GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index, char use_bit);
    long PageOffset(int page_id);
    char GetUse(int index);

private:
    FILE* currFile_;
    int numPages_;
    int basePages_;             //file pages num for store usage array
    char pages_[MAXPAGES];      //page usage array
};

#endif //LAB_STORAGE_H
