//
// Created by wujy on 2017/11/17.
//


#include "buffer.h"
#include "storage.h"
#include <cstdio>
#include <sstream>
#include "timer.h"

using namespace std;

int main(){
    BMgr buffer;
    buffer.dsmgr.CreateNPagesFile(MAXPAGES,"data.dbf");      // create file for experiment
    buffer.dsmgr.OpenFile("data.dbf");
    FILE* trace = fopen("zipfian.txt","r");
    int operation;
    int page;
    int frame_id;
    uint64_t time = 0;
    int cnt = 0;
    Timer timer;                  //get cost

    while(fscanf(trace,"%d,%d",&operation,&page)!=EOF){
        timer.Start();                 // timing
        frame_id = buffer.FixPage(page);
        if(operation==1){
            buffer.SetDirty(frame_id);
        }
        buffer.UnfixPage(page);
        time+=timer.Elapsed();       // get time of this request
            printf("operation count : %d\n",cnt++);
    }
    timer.Start();
    buffer.WriteDirtys();
    buffer.dsmgr.CloseFile();
    time+=timer.Elapsed();
    printf("Completed!\nTime cost: %.2lfs\n",time/1000000.0);

    return 0;
}