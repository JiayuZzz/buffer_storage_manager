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
    printf("hello\n");
    buffer.dsmgr.CreateNPagesFile(MAXPAGES,"data.dbf");      // create file for experiment
    printf("hello2\n");
    buffer.dsmgr.OpenFile("data.dbf");
    printf("hello3\n");
    FILE* trace = fopen("zipfian.txt","r");
    printf("hello4\n");
    int operation;
    int page;
    int frame_id;
    uint64_t time = 0;
    Timer timer;                  //get cost

    while(fscanf(trace,"%d,%d",&operation,&page)){
        timer.Start();                 // timing
        printf("hello5\n");
        frame_id = buffer.FixPage(page);
        printf("hello6\n");
        if(operation==1){
            buffer.SetDirty(frame_id);
            printf("hello7\n");
        }
        buffer.UnfixPage(page);
        printf("hello8\n");
        time+=timer.Elapsed();       // get time of this request
    }
    timer.Start();
    buffer.WriteDirtys();
    buffer.dsmgr.CloseFile();
    time+=timer.Elapsed();
    printf("Completed!\nTime cost: %.2lfs",time/1000000.0);

    return 0;
}