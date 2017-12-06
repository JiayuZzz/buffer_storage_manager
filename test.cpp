//
// Created by wujy on 2017/11/17.
//


#include "buffer.h"
#include <iostream>

using namespace std;

extern int readPage;
extern int writeDirty;
extern int hit;

int main(){
    BMgr buffer;
    buffer.dsmgr.CreateNPagesFile(MAXPAGES,"data.dbf");      // create file for experiment
    buffer.dsmgr.OpenFile("data.dbf");
    FILE* trace = fopen("/home/wujy/work/buffer_storage_manager/zipfian.txt","r");
    int operation;
    int page;
    int frame_id;
    int cnt = 0;

    while(fscanf(trace,"%d,%d",&operation,&page)!=EOF){

        frame_id = buffer.FixPage(page);
        if(operation==1){
            //if write, set frame to dirty
            buffer.SetDirty(frame_id);
        }
        buffer.UnfixPage(page);
        printf("operation count:%d\r",++cnt);
    }
    //write all dirty pages to disk before closing database
    buffer.WriteDirtys();
    buffer.dsmgr.CloseFile();
    printf("\n\nComplete!\n\n");
    printf("Total cache hit counts: %d\t hit ratio:%.3lf\n\n",hit,hit*1.0/cnt);
    printf("IO counts:\nRead pages from disk: %d\nWrite dirty pages to disk:%d\n",readPage,writeDirty);
    printf("Total IO counts:%d\n",readPage+writeDirty);


    return 0;
}