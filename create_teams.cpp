//createteams.cpp
#include "priorityqueue.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include "json.hpp"
#include <fstream>
#include <cmath>
#include <unordered_set>

using namespace std;

int main(int argc, char** argv){ //if wrong # of arguments
  if(argc != 2){
    cerr << "Usage: createteams file.json" << endl;
    return -1;
  }
  
  ifstream inFile;
  string fileName = argv[1];
  inFile.open(fileName);
  
  if(inFile.fail()){ //if file not open
    cerr << "Could not open file." << endl;
    return -1;
  }

  unordered_set <int> pickedPlayers; //a set of player already in a team
  nlohmann::json result, input;
  int player1, player2;
  input = nlohmann::json::parse(inFile);
  size_t num = input["metadata"]["numPlayers"];
  size_t maxSize = num * (num - 1) / 2; //max size of the priority queue = unique combinations
  input = input["teamStats"];
  double winPercentage;
  //maxsize = total combination of teams
  PriorityQueue teamWinpercentage = PriorityQueue(maxSize);
  KeyValuePair poppedTeam;
  
  //first iteration build the heap
  for (auto itr = input.begin(); itr != input.end(); ++itr) {
    //get player1, get player 2, get winpercentage, calculate 50-winpercentage absolute value
    player1 = itr.value()["playerOne"];
    player2 = itr.value()["playerTwo"];
    winPercentage = itr.value()["winPercentage"];
    winPercentage = fabs(winPercentage - 50);
    teamWinpercentage.insert(winPercentage, player1, player2);
  }
  //second iteration extract the team
  pair <int, int> team;
  vector <pair<int, int>> teams;
  for(size_t i = 0; i < maxSize; ++i){
    poppedTeam = teamWinpercentage.removeMin();
    player1 = poppedTeam.second.first;
    player2 = poppedTeam.second.second;

    // always remove min but only make a team if piayer isn't already picked
    if(!(pickedPlayers.count(player1) || pickedPlayers.count(player2))){
      pickedPlayers.insert(poppedTeam.second.first);
      pickedPlayers.insert(poppedTeam.second.second);
      team.first = player1;
      team.second = player2;
      teams.push_back(team);
    }
  
  }
  result["teams"] = teams;
  cout << result.dump(2) << endl;
}
