#ifndef DV_PR_COMPONENT_H
#define DV_PR_COMPONENT_H
#include <fstream>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

//define the function:get_graph return
struct Returns {
    vector<vector<vector<int>>> matrix;
    vector<string> index2name;
    map<string, int> name2index;
    int node_number;
};

//return the index of minimum value in vector.
int min_index(vector<int> vector1) {
  int temp = vector1[0];
  int temp_index = 0;
  for (int i = 1; i < vector1.size(); i++) {
    if (vector1[i] < temp) {
      temp = vector1[i];
      temp_index = i;
    }
  }
  return temp_index;
}

// return initialized adjacent_matrix of route network;
// print the sent route massage to neighbor when t = 0;
Returns get_graph(const string& configFileName) {
  ifstream input(configFileName);
  string str;
  getline(input, str);
  //get the number of routing nodes
  int N = stoi(str);
  vector<string> index2name(N);
  for(int i = 0; i < N; i++) {
    getline(input, str);
    index2name[i] = str;
  }
  //sort the nodes to alphabetical order by router name
  sort(index2name.begin(), index2name.end());
  //initialize the map of router name and index
  map<string, int> name2index;
  for(int i = 0; i < N; i++) {
    name2index[index2name[i]] = i;
  }

  vector<vector<vector<int>>> adjacent_matrix(N, vector<vector<int>>(N, vector<int>(N, INT16_MAX)));
  //init the node of router to itself
  for(int i = 0; i < N; i++) {
    adjacent_matrix[i][i][i] = 0;
  }
  getline(input, str);
  // edge of the routing network
  int E = stoi(str);
  //init the count of updated node
  int t = 0;
  cout << "#START" << endl;
  for(int i = 0; i < E; i++) {
    getline(input, str);
    istringstream iss(str);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    //tokens has 3 elements: source_node, destination_node, value of the edge
    int from = name2index[tokens[0]];
    int to = name2index[tokens[1]];
    cout << "t=" << t << " distance from " << tokens[0] << " to " << tokens[1] << " via " << tokens[1] << " is " << tokens[2] << endl;
    cout << "t=" << t << " distance from " << tokens[1] << " to " << tokens[0] << " via " << tokens[0] << " is " << tokens[2] << endl;
    adjacent_matrix[from][to][to] = stoi(tokens[2]);
    adjacent_matrix[to][from][from] = stoi(tokens[2]);
  }
  input.close();
  return {adjacent_matrix, index2name, name2index, N};
}

// exchange the updated routing message until no routing table need to be updated
void send_routing(vector<vector<vector<int>>>& adjacent_matrix,
                  int N,
                  map<string, int>& name2index, vector<string>& index2name,
                  bool PR) {
  int t = 1;
  bool changed = true;
  while(changed) {
    changed = false;
    // to avoid the impact of synchronous updates on other routes
    vector<vector<vector<int>>> new_matrix = adjacent_matrix;
    // Traverse all nodes in matrix
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i == j) continue;
        for (int k = 0; k < N; k++) {
          if (k == i) continue;
          //if i, j, k is reachable, calculate the shortest path from all changed neighbor.
          if (adjacent_matrix[i][k][k] != INT16_MAX) {
            int via = min_index(adjacent_matrix[k][j]);
            int dis;
            // PR for i to j via k, if the path of k to j contain i, let i knows k to j is unreachable.
            if(PR && via == i) {
              dis = adjacent_matrix[i][k][k] + INT16_MAX;
            } else {
              dis = adjacent_matrix[i][k][k] + adjacent_matrix[k][j][via];
            }
            if(dis > INT16_MAX) dis = INT16_MAX;
            if(dis != adjacent_matrix[i][j][k]) {
              changed = true;
              cout << "t=" << t << " distance from " << index2name[i] << " to " <<
                   index2name[j] << " via " << index2name[k] << " is " << (dis == INT16_MAX? "INF": to_string(dis)) << endl;
              new_matrix[i][j][k] = dis;
            }
          }
        }
      }
    }
    adjacent_matrix = new_matrix;
    t++;
  }
}
// update the value and statement for each router node.
void update_graph(vector<vector<vector<int>>>& adjacent_matrix,
                  const string& changedConfigName,
                  map<string, int>& name2index, const vector<string>& index2name) {
  int t = 0;
  cout << "#UPDATE" << endl;
  string str;
  ifstream change_config(changedConfigName);
  getline(change_config, str);
  // get the number of updated edges.
  int change_E = stoi(str);
  for (int i = 0; i < change_E; i++) {
    getline(change_config, str);
    istringstream iss(str);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    cout << "t=" << t << " distance from " << tokens[0] << " to " << tokens[1] << " via " << tokens[1] << " is "
         << tokens[2] << endl;
    cout << "t=" << t << " distance from " << tokens[1] << " to " << tokens[0] << " via " << tokens[0] << " is "
         << tokens[2] << endl;
    int from = name2index[tokens[0]];
    int to = name2index[tokens[1]];
    adjacent_matrix[from][to][to] = stoi(tokens[2]);
    adjacent_matrix[to][from][from] = stoi(tokens[2]);
  }
}

void print_routing(vector<vector<vector<int>>>& adjacent_matrix, const string& name,
                   int N,
                   map<string, int>& name2index, const vector<string>& index2name) {
  cout << name << endl;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) continue;
      int via = min_index(adjacent_matrix[i][j]);
      if (adjacent_matrix[i][j][via] != INT16_MAX) {
        cout << "router " << index2name[i] << ": " << index2name[j] << " is " << adjacent_matrix[i][j][via] << " routing through " << index2name[via] << endl;
      }
    }
  }
}

#endif