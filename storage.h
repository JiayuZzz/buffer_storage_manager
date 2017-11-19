//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_STORAGE_H
#define LAB_STORAGE_H

#include <string>
#include <cstdio>
#include <vector>

#define FRAMESIZE 4096
#define PAGESIZE 4096

struct bFrame{
    char filed[FRAMESIZE];
};

class DSMgr{
public:
    DSMgr();
    int OpenFile(std::string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    int Seek(long offset);
    FILE* GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index, char use_bit);
    long PageOffset(int page_id);
    int GetUse(int index);

private:
    FILE* currFile_;
    int numPages_;
    std::vector<char> pages_;   //page used?
};

#endif //LAB_STORAGE_H
