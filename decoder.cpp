#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
   
   vector<vector<bool>> visited(mapImg.width(), vector<bool> (mapImg.height(), false));
   vector<vector<int>> distance(mapImg.width(), vector<int> (mapImg.height(), 0));
   vector<vector<pair<int,int>>> prev(mapImg.width(), vector<pair<int,int>> (mapImg.height(), pair<int, int> (-1, -1)));
   Queue<pair<int,int>> toExplore;

   visited[start.first][start.second] = true;
   distance[start.first][start.second] = 0;
   toExplore.enqueue(start);

   pair<int,int> curr;
   int d = 0;
   pair<int,int> p = pair<int, int> (0,0);

   while(!toExplore.isEmpty()){
      curr = toExplore.dequeue();
      for(pair<int,int> neighbor : neighbors(curr)){
         if(good(visited, distance, curr, neighbor)) {
            mazePts.push_back(neighbor);
            visited[neighbor.first][neighbor.second] = true;
            distance[neighbor.first][neighbor.second] = distance[curr.first][curr.second] + 1;
            prev[neighbor.first][neighbor.second] = curr;
            toExplore.enqueue(neighbor);

            if(distance[neighbor.first][neighbor.second]>=d){
               d = distance[neighbor.first][neighbor.second];
               p = neighbor;
            }
         }
      }
   }

   while(p != pair<int,int> (-1, -1)){
      pathPts.push_back(p);
      p = prev[p.first][p.second];
   }
}

PNG decoder::renderSolution(){
   PNG mapCopy = PNG(mapImg);

   for(pair<int,int> curr : pathPts){
      RGBAPixel* temp = mapCopy.getPixel(curr.first, curr.second);
      temp->r = 255;
      temp->g = 0;
      temp->b = 0;
   }
   return mapCopy;
}

PNG decoder::renderMaze(){
   PNG mapCopy = PNG(mapImg);

   for(pair<int,int> temp : mazePts){
      setGrey(mapCopy, temp);
   }

   for(int i = start.first - 3; i <= start.first + 3; i++){
      for(int j = start.second - 3; j <= start.second + 3; j++){
         if(i < mapCopy.width() && j < mapCopy.height() && i >= 0 && j >= 0){
            RGBAPixel* temp = mapCopy.getPixel(i, j);
            temp->r = 255;
            temp->g = 0;
            temp->b = 0;
         }
      }
   }
   return mapCopy;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){
   RGBAPixel* temp = im.getPixel(loc.first, loc.second);
   temp -> r = 2*(temp->r/4);
   temp -> g = 2*(temp->g/4);
   temp -> b = 2*(temp->b/4);
}

pair<int,int> decoder::findSpot(){
   return pathPts[pathPts.size() - 1];
}

int decoder::pathLength(){
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
   if (next.first >= mapImg.width() || next.first < 0) {
      return false;
   }

   if (next.second >= mapImg.height() || next.second < 0) {
      return false;
   }

   if (v[next.first][next.second]) {
      return false;
   }

   RGBAPixel * temp = mapImg.getPixel(next.first, next.second);
   if (! (compare(* temp, d[curr.first][curr.second]))) {
      return false;
   }

   return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
   pair<int, int> left (curr.first-1, curr.second);
   pair<int, int> right (curr.first+1, curr.second);
   pair<int, int> above (curr.first, curr.second-1);
   pair<int, int> below (curr.first, curr.second+1);
   vector <pair <int, int>> fourNeighbors = {left, below, right, above};
   return fourNeighbors;
}

bool decoder::compare(RGBAPixel temp, int d){
   if (((temp.r % 4) * 16 + (temp.g % 4) * 4 + (temp.b % 4)) % 64 == (d + 1) % 64) {
      return true;
   }
   return false;
}
