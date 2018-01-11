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
#define BUFSIZE 1024

struct bFrame{
    char filed[FRAMESIZE];
};

class DSMgr{
public:
    DSMgr();
    int OpenFile(const char* filename);                  //打开文件，若文件不存在则创建新文件
    int CloseFile();                                     //关闭文件
    int CreateNPagesFile(int n,const char* filename);    //为实验创建一个n页的数据库
    bFrame ReadPage(int page_id);                        //将某页读入内存
    int WritePage(int page_id, bFrame frm);              //将frame中的内容写回文件页中
    long PageOffset(int page_id);                        //计算页号在文件中的偏移量
    int Seek(long offset);                               //根据页偏移量定位页位置
    FILE* GetFile();                                     //获得数据库文件指针
    void IncNumPages();                                  //增加已使用的总页数
    int GetNumPages();                                   //获得总页数
    void SetUse(int index, char use_bit);                //将某位标记成已使用或未使用，1为已使用0为未使用
    char GetUse(int index);                              //返回某页是否已被使用

private:
    FILE* currFile_;                                     //该manager所打开的数据库文件指针
    int numPages_;                                       //总页数
    int basePages_;                                      //usage array在文件开头所占的页数
    char pages_[MAXPAGES];                               //usage array，记录各页是否被占用
};

#endif //LAB_STORAGE_H
