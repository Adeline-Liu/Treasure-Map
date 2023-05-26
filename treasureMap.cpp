#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);

}


void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);

    int dNew = d%64;
    int b6 = dNew % 2;
    int b5 = (dNew/2) % 2;
    int b4 = (dNew/4) % 2;
    int b3 = (dNew/8) % 2;
    int b2 = (dNew/16) % 2;
    int b1 = (dNew/32) % 2;

    pixel->r = pixel->r - pixel->r % 4 + b2 + b1 * 2;
    pixel->g = pixel->g - pixel->g % 4 + b4 + b3 * 2;
    pixel->b = pixel->b - pixel->b % 4 + b6 + b5 * 2;

}

PNG treasureMap::renderMap(){

    PNG baseCopy = PNG(base);

    vector<vector<bool>> visited(base.width(), vector<bool> (base.height(), false));
    vector<vector<int>> distance(base.width(), vector<int> (base.height(), 0));
    Queue<pair<int, int>> toExplore;

    visited[start.first][start.second] = true;
    distance[start.first][start.second] = 0;
    setLOB(baseCopy, start, 0);
    toExplore.enqueue(start);

    while (!toExplore.isEmpty()) {
        pair<int, int> curr = toExplore.peek();
        toExplore.dequeue();

        vector <pair <int, int>> neighbors4 = neighbors(curr);
        for (int i =0; i< neighbors4.size(); i++) {
            pair<int, int> p = neighbors4[i];
            if(good(visited, curr, p)){
                visited[p.first][p.second] = true;
                distance[p.first][p.second] = distance[curr.first][curr.second] + 1;
                setLOB(baseCopy, p, distance[p.first][p.second]);
                toExplore.enqueue(p);
            }
        }
    }
    return baseCopy;
}

PNG treasureMap::renderMaze(){
    PNG baseCopy = PNG(base);

    for(int i = 0; i < baseCopy.width(); i++){
        for(int j = 0; j < baseCopy.height(); j++){
            RGBAPixel * temp1 = maze.getPixel(start.first, start.second);
            RGBAPixel * temp2 = maze.getPixel(i, j);
            if(* temp1 == * temp2){
                setGrey(baseCopy, pair<int,int> (i,j));
            }
        }
    }

    for(int i = start.first-3; i <= start.first+3; i++){
      for(int j = start.second-3; j <= start.second+3; j++){
         if(i < baseCopy.width() && i >= 0 && j < baseCopy.height() && j >= 0){
            RGBAPixel* temp = baseCopy.getPixel(i, j);
            temp->r = 255;
            temp->g = 0;
            temp->b = 0;
         }
      }
    }
    return baseCopy;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
      if (next.first >= base.width() || next.first < 0) {
        return false;
    }

    if (next.second >= base.height() || next.second < 0) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;
    }

    RGBAPixel * temp1 = maze.getPixel(curr.first, curr.second);
    RGBAPixel * temp2 = maze.getPixel(next.first, next.second);
    if (* temp1 != * temp2) {
        return false;
    }

    return true;  
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
   pair<int, int> left (curr.first-1, curr.second);
   pair<int, int> right (curr.first+1, curr.second);
   pair<int, int> above (curr.first, curr.second-1);
   pair<int, int> below (curr.first, curr.second+1);
   vector <pair <int, int>> fourNeighbors = {left, below, right, above};
   return fourNeighbors;
}

