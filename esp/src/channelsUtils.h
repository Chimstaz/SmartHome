#ifndef CHANNELSUTILS_H
#define CHANNELSUTILS_H

#include "collections.h"
#include "globals.h"

int findChannel(const char* name, Array<String*> &channelsList){
  for(int j = 0; ; j++){
    if(channelsList[j] == NULL){
      return -1;
    }
    if(channelsList[j]->equals(name)){
      //c[CHANNEL_ID] = j;
      return j;
    }
  }
}

#endif //CHANNELSUTILS_H
