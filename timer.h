//
// Created by wujy on 2017/11/21.
//

#ifndef LAB_TIMER_H
#define LAB_TIMER_H

#include <sys/time.h>
#include <stdlib.h>

class Timer{
private:
    timeval start_;
    timeval end_;
    timeval result_;

public:
    void Start(){
      gettimeofday(&start_,NULL);
    }
    //return usec
    uint64_t Elapsed(){
      gettimeofday(&end_,NULL);
      timersub(&end_,&start_,&result_);
      return result_.tv_sec*1000000+result_.tv_usec;
    }
};

#endif //LAB_TIMER_H
