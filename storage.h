//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_STORAGE_H
#define LAB_STORAGE_H

#include <string>
#include <cstdio>

#define FRAMESIZE 4096
#define MAXPAGES 50000

struct bFrame{
    char filed[FRAMESIZE];
};

class DSMgr{
public:
    DSMgr();
    int OpenFile(std::string filename);
    int CreateFile(std::string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    int Seek(long offset);
    FILE* GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index,int use_bit);
    int GetUse(int index);

private:
    FILE* currFile_;
    int numPages_;
    int poffset[MAXPAGES];
    char pages_[MAXPAGES];   //page used?
};

#endif //LAB_STORAGE_H
