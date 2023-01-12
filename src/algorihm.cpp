#include "../inc/algorithm.hpp"
#include "../inc/entity.hpp"
#include <map>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iostream>
int findTrackFromNet(std::map<int, int> curTrack, int netID, bool isTop);
void insertVertical(int netID, int startX, int startY, int segment, std::vector<std::vector<vias>> &vertical, std::vector<bool> &verticalMap);
int findEmptyTrack(std::map<int, int> curTrack, bool isTop, int tracksHeight);
int findTop(int netID, int col, Channel channel, std::map<int, int> curTrack, int nowTrack);
void algor::greedyRouting(Channel *channel){
    int tracksHeight = 30;
    if(channel->maxNetIndex > 30)tracksHeight = 40;
    std::vector<bool>checkStart(channel->maxNetIndex+1, false);
    std::vector<std::vector<vias>> vertical;
    std::vector<std::vector<vias>> horizonal;
    
    vertical.resize(channel->maxNetIndex+1);
    horizonal.resize(channel->maxNetIndex+1);

    std::vector<std::map<int, int>> tracks; // colIndex
    std::map<int, int> curTrack; //trackIndex, netIndex
    tracks.push_back(curTrack); // init tracks
    
    for(int i = 0; i < channel->channelLength+10; i ++){
        if(i!=0 && !curTrack.size())break;
        std::vector<bool> verticalMap(tracksHeight+1, false);
        std::vector<bool> reachedTrackMap(channel->maxNetIndex+1, false);
        for(auto j: curTrack){
            reachedTrackMap[j.first] = true;
            vias newVias;
            newVias.X = i-1;
            newVias.Y = j.first;
            newVias.segment = 1;
			horizonal[j.second].push_back(newVias);
        }
        int topNetID = i < channel->channelLength ? channel->topPin.at(i) : 0;
        int botNetID = i < channel->channelLength ? channel->botPin.at(i) : 0;
        //a
        int upperBound = tracksHeight+1; 
        int lowerBound = 0;
        if(topNetID == botNetID && channel->lastPin[topNetID] == i){ //case E
            if(topNetID != 0) insertVertical(topNetID, i, 0, tracksHeight+1, vertical, verticalMap);
            topNetID = 0, botNetID = 0;
            upperBound = 0;
		}else{
            int topTrack = findTrackFromNet(curTrack, topNetID, true);
            int botTrack = findTrackFromNet(curTrack, botNetID, false);
            int emptyBotTrack = findEmptyTrack(curTrack, false, tracksHeight);
            int emptyTopTrack = findEmptyTrack(curTrack, true, tracksHeight);
            
            int placeTopTrack = std::max(topTrack, emptyTopTrack);
            int placeBotTrack = std::min(emptyBotTrack, botTrack);
            if (placeBotTrack == -1)
                placeBotTrack = std::max(emptyBotTrack, botTrack);
        
            if(placeBotTrack < placeTopTrack){
                if(topNetID != 0){
                    if(placeTopTrack == -1){
                        //do e
                    }else{
                        insertVertical(topNetID, i, placeTopTrack, tracksHeight-placeTopTrack+1, vertical, verticalMap);
                        curTrack.insert(std::pair<int, int>(placeTopTrack, topNetID));
                        upperBound = placeTopTrack;
                    }
                }
                if(botNetID != 0){
                    if(placeBotTrack == -1){
                        //do e
                    }else{
                        insertVertical(botNetID, i, 0, placeBotTrack, vertical, verticalMap);
                        curTrack.insert(std::pair<int, int>(placeBotTrack, botNetID));
                        lowerBound = placeBotTrack;
                        
                    }
                }
            }else if(placeBotTrack == placeTopTrack){
                //do e
                if(placeTopTrack == -1){
                    //do e
                }else{
                    placeTopTrack += 1;
                    insertVertical(topNetID, i, placeTopTrack, tracksHeight-placeTopTrack+1, vertical, verticalMap);
                    curTrack.insert(std::pair<int, int>(placeTopTrack, topNetID));
                    upperBound = placeTopTrack;
                }
            }else{
                
                
                if(tracksHeight-placeTopTrack+1 < placeBotTrack){
                    insertVertical(topNetID, i, placeTopTrack, tracksHeight-placeTopTrack+1, vertical, verticalMap);
                    curTrack.insert(std::pair<int, int>(placeTopTrack, topNetID));
                    upperBound = placeTopTrack;
                }else{
                    insertVertical(botNetID, i, 0, placeBotTrack, vertical, verticalMap);
                    curTrack.insert(std::pair<int, int>(placeBotTrack, botNetID));
                    lowerBound = placeBotTrack;
                }
            }
            // channel->isTop[topNetID]--;
            // channel->isTop[botNetID]++;
            


            
            // if(topNetID == 0){
            //     //do bot
            //     int emptyTrack = findEmptyTrack(curTrack, false, tracksHeight);
            //     if(emptyTrack == -1 && botTrack == -1){
            //         //do step e
            //     }else if(emptyTrack == -1 && botTrack != -1){
            //         insertVertical(botNetID, i, 0, botTrack, vertical);
            //     }else if((emptyTrack <= botTrack || botTrack == -1)){
            //         insertVertical(botNetID, i, 0, emptyTrack, vertical);
            //         curTrack.insert(emptyTrack, botNetID);
            //     }
            // }else if(botNetID == 0){
            //     //do top
            //     int emptyTrack = findEmptyTrack(curTrack, true, tracksHeight);
            //     if(emptyTrack == -1 && topTrack == -1){
            //         //do step e
            //     }else if(emptyTrack == -1 && topTrack != -1){
            //         insertVertical(topNetID, i, topTrack, tracksHeight-topTrack+1, vertical);
            //     }else if((emptyTrack >= topTrack || topTrack == -1)){
            //         insertVertical(topNetID, i, emptyTrack, tracksHeight-emptyTrack+1, vertical);
            //         curTrack.insert(emptyTrack, topNetID);
            //     }
            // }else{
            //     int botEmptyTrack = findEmptyTrack(curTrack, false, tracksHeight);
            //     int topEmptyTrack = findEmptyTrack(curTrack, true, tracksHeight);
            //     int placeBotTrack = std::min(botEmptyTrack, botTrack);
            //     if (botEmptyTrack == -1)
            //         placeBotTrack = botTrack;
            //     int placeTopTrack = std::max(topEmptyTrack, topTrack);
            //     if(placeBotTrack >= placeTopTrack && placeTopTrack != -1 ){ // chose 1
            //         if(placeBotTrack <= tracksHeight-placeTopTrack+1 && placeBotTrack != -1){
            //             insertVertical(botNetID, i, 0, placeBotTrack, vertical);
                    
            //             if(placeBotTrack == botEmptyTrack)
            //                 curTrack.insert(placeBotTrack, botNetID);
            //         }else if (placeTopTrack != -1){
            //             insertVertical(topNetID, i, placeTopTrack, tracksHeight-placeTopTrack+1, vertical);
            //             if(placeTopTrack == topEmptyTrack)
            //                 curTrack.insert(placeTopTrack, topNetID);
            //         }else{
            //             //do e
            //         }
            //     }else if(placeTopTrack != -1 && placeTopTrack != -1){ //do both
            //         insertVertical(botNetID, i, 0, placeBotTrack, vertical);
                    
            //             if(placeBotTrack == botEmptyTrack)
            //                 curTrack.insert(placeBotTrack, botNetID);

            //         insertVertical(topNetID, i, placeTopTrack, tracksHeight-placeTopTrack+1, vertical);
            //             if(placeTopTrack == topEmptyTrack)
            //                 curTrack.insert(placeTopTrack, topNetID);
            //     }else{
            //         //do double e
            //     }
            }
        

        //b
        std::vector<std::tuple<int, int, int>> points; //start, end, netID
        std::unordered_map<int, int> highest;
        for(auto &t: curTrack){
            if(highest.count(t.second)){
				points.push_back({highest[t.second], t.first, t.second});
				highest[t.second] = t.first;
		    }else{
	    		highest[t.second] = t.first;
			}
        }
        std::sort(points.begin(), points.end(), [](std::tuple<int, int, int> a, std::tuple<int, int, int> b){
            if(std::get<1>(a) != std::get<1>(b) )
                return std::get<1>(a) < std::get<1>(b);
            else std::get<0>(a) < std::get<0>(b);
        });
		std::vector<int> selected;  
        if(points.size()){
            int index;
            for(index = 0; index < points.size(); index ++ ){
                auto [r1, r2, id] = points[index];
                if(r1 >= lowerBound && r2 <= upperBound){
                    selected.push_back(index);
                    break;
                }
            }
            //selected.push_back(0);
            for(int k=index+1; k<points.size(); k++){
                auto [r1, r2, id] = points[k];
                if(std::get<1>(points[selected.back()]) <= r1 && r2 <= upperBound){
                    selected.push_back(k);
                }else if(std::get<1>(points[selected.back()]) > r2){
                    selected.pop_back();
                    selected.push_back(k);
                }
            }

            for(int j=0; j<selected.size(); j++){	
                auto [r1, r2, id] = points[selected[j]];
                insertVertical(id, i, r1, r2-r1, vertical, verticalMap);
            }
            tracks.push_back(curTrack);
			//next col's track
			for(int ii=0; ii<selected.size(); ii++){
                //do better
                
                if(findTop(std::get<2>(points[selected[ii]]), i, *channel, curTrack, -1) > 0){
				    curTrack.erase(std::get<0>(points[selected[ii]]));
                }else{
                   curTrack.erase(std::get<1>(points[selected[ii]])); 
                }
			}
        }else{
            tracks.push_back(curTrack);
        }


        std::unordered_map<int, std::vector<int>> reached;
		for(auto [col, id] : curTrack){
			reached[id].push_back(col);
		}
		for(auto [id, s] : reached){
			if(s.size() != 1 || channel->lastPin[id] > i) continue;
            curTrack.erase(*s.begin());
            
		}

    
        std::vector<int> eraseList;
        for(int time = 0; time < 5; time ++){
            for(auto [col, id] : curTrack){
                int start=1;
                int len = start;
                int direction = findTop(id, i, *channel, curTrack, col);
                // if(checkStart[id]){
                //     continue;
                // }
                for(;start<tracksHeight/5;start ++){
                    if(direction > 0){
                        if(start+col >= tracksHeight){
                            break;
                        }
                        if(verticalMap[col+start]){               
                            break;
                        }
                        if(curTrack.find(col+start) != curTrack.end() || reachedTrackMap[col+start]){
                            continue;
                        }
                        len = start;
                    }else if(direction < 0){
                        if(col-start <= 0){
                            break;
                        }
                        if(verticalMap[col-start]){
                            break;
                        }
                        if(curTrack.find(col-start) != curTrack.end() || reachedTrackMap[col-start]){
                            continue;
                        }
                        len = start;
                    }
                }
                if(len <= 2){
                    continue;
                }else{
                    if(direction > 0){
                        eraseList.push_back(col);
                        curTrack.insert(std::pair<int, int>(col+len, id));
                        insertVertical(id, i, col, len, vertical, verticalMap);
                    }else if(direction < 0){
                        eraseList.push_back(col);
                        curTrack.insert(std::pair<int, int>(col-len, id));
                        insertVertical(id, i, col-len, len, vertical, verticalMap);
                    }
                }
            }
            for(auto erase:eraseList){
                curTrack.erase(erase);
            }
        }
        //

        //insert horizonal
        
        checkStart[topNetID] = true;
        checkStart[botNetID] = true;
    }
    tracks.push_back(std::map<int, int>());

	// for(int i=0; i < tracks.size() - 1; i++){
	// 	int start = i;
        
	// 	while(tracks[i].size()){
            


	// 		auto [row, id] = *tracks[i].begin();
	// 		tracks[i].erase(tracks[i].begin());

	// 		int len = 0;
	// 		while(tracks[i + len+1].count(row) && tracks[i + len+1][row] == id){
	// 			tracks[i + len+1].erase(row);
	// 			len++;
	// 		}
    //         vias newVias;
    //         newVias.X = start-1;
    //         newVias.Y = row;
    //         newVias.segment = len;
    //         if(len >= 1)
	// 		    horizonal[id].push_back(newVias);
	// 	}
	// }

    for(int i = 0; i < vertical.size(); i ++){
        for(auto via : vertical[i]){
            via.direction = 'V';
            channel->netLists[i].push_back(via);
        }
        std::sort(channel->netLists[i].begin(), channel->netLists[i].end(), [](vias a, vias b){
                if (a.X != b.X)
                    return a.X < b.X;
                else return a.Y < b.Y;
        });
    }
    for(int j = 0; j < horizonal.size(); j ++){
        for(auto via : horizonal[j]){
            via.direction = 'H';
            channel->netLists[j].push_back(via);
        }
    }

    // for(auto netlist:channel->netLists){
    //     for(int i = 0; i < netlist.size(); i ++){
    //         for(int j = i+1; j < netlist.size(); j ++){
    //             if(netlist[i].X == netlist[j].X ){

    //             }
    //         }
            
    //     }
    // }

}

int findTrackFromNet(std::map<int, int> curTrack, int netID, bool isTop){
    int res = -1;
    for(auto i:curTrack){
        if(i.second == netID){
            if(!isTop)
                return i.first;
            res = i.first;
        }
    }
    return res;
}
int findEmptyTrack(std::map<int, int> curTrack, bool isTop, int tracksHeight){
    
    for(int i = 1; i <= tracksHeight; i ++){
        int cur = i;
        if(isTop){
            cur = tracksHeight+1-i;
        }
        std::map<int, int>::iterator iter = curTrack.find(cur);
        if(iter == curTrack.end()){
            return cur;
        }else{
           continue;
        }
    }
    return -1;
}

void insertVertical(int netID, int startX, int startY, int segment, std::vector<std::vector<vias>> &vertical, std::vector<bool>&verticalMap ){
    vias newVias;
    newVias.X = startX;
    newVias.Y = startY;
    newVias.segment = segment;
    vertical[netID].push_back(newVias);
    for(int i = 0; i <= segment; i ++){
        verticalMap[startY+i] = true;
    }
}

int findTop(int netID, int col, Channel channel, std::map<int, int> curTrack, int nowTrack){
    if(nowTrack != -1){
        for(auto [track, id] :curTrack){
            if(id == netID && track != nowTrack){
                if(track > nowTrack){
                    return 1;
                }else{
                    return -1;
                }
            }
        }
    }
    if(col < channel.lastPin[netID]){
        for(int i = col+1; i < channel.topPin.size(); i ++){
            if(channel.topPin.at(i) == netID)return 1;
            if(channel.botPin.at(i) == netID)return -1;
            if(i-col > 10)return 0;
        }
    }
    return 0;
}

