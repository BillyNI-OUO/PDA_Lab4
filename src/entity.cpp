#include "../inc/entity.hpp"
#include <algorithm>
#include <vector>
void Channel::addPin(int netIndex, bool isTop){
    if(isTop){
        this->topPin.push_back(netIndex);
    }else{
        this->botPin.push_back(netIndex);
    }
    
}

void Channel::init(){
    this->channelLength = topPin.size();
    this->maxNetIndex = std::max(*std::max_element(topPin.begin(), topPin.end()), *std::max_element(botPin.begin(), botPin.end()));
    this->lastPin.resize(this->maxNetIndex+1);
    this->isTop.resize(this->maxNetIndex+1);
    this->netLists.resize(this->maxNetIndex+1);
    for(int i = 0; i < this->channelLength; i ++){
        lastPin[topPin[i]] = i;
        isTop[topPin[i]]++;
        lastPin[botPin[i]] = i;
        isTop[botPin[i]]--;
    }
}