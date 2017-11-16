//
// Created by wujy on 2017/11/15.
//

#ifndef LAB_STORAGE_H
#define LAB_STORAGE_H

#include <string>

#define MAXPAGES 50000

using std::string;

#define FRAMESIZE 4096

struct bFrame{
    char filed[FRAMESIZE];
};

class DSMgr{
public:
    DSMgr();
    int OpenFile(string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    int Seek(int offset, int pos);
    FILE* GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index,int use_bit);
    int GetUse(int index);

private:
    FILE* currFile;
    int numPages;
    int pages[MAXPAGES];
};

#endif //LAB_STORAGE_H
