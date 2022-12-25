#ifndef ENTITY_H
#define ENTITY_H 
#include "vector"
#include <stdio.h>
typedef struct{
    char direction;
    int X;
    int Y;
    int segment;        
}vias;
class Channel{
    
    public:
        std::vector<int> topPin;
        std::vector<int> botPin;
        std::vector<int> lastPin;
        std::vector<bool> isTop;
        int maxNetIndex;
        int channelLength;
        std::vector<std::vector<vias>> netLists;
        void addPin(int netIndex, bool isTop);
        void printContent(){
            for(auto i: topPin){
                printf("%3d ", i);
            }
            printf("\n");
            for(auto i: botPin){
                printf("%3d ", i);
            }
            printf("\n");
        };
        void init();
};


#endif