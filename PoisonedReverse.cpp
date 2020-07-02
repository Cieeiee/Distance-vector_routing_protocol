#include "component.h"
using namespace std;

int main(int argc, char * argv[]) {
  //check arguments
  if (argc != 3) {
    cout << "Bad or missing arguments" <<endl;
    return -1;
  }
  string configFileName = argv[1];
  string changedConfigName = argv[2];
  ifstream test(configFileName);
  ifstream test2(changedConfigName);
  if(!test || !test2) {
    cout << "Bad or missing arguments" <<endl;
    return -1;
  }
  test.close();
  test2.close();
  //initialize the cost adjacent matrix
  Returns r = get_graph(configFileName);

  vector<vector<vector<int>>> adjacent_matrix = r.matrix; // adjacent matrix of network
  int N = r.node_number; // number of routers
  map<string, int> name2index = r.name2index; // map from router name to index
  vector<string> index2name = r.index2name; // map from router index to name

  // exchange the updated routing message until no routing table need to be updated
  send_routing(adjacent_matrix, N, name2index, index2name, true);

  // print the current routing table
  print_routing(adjacent_matrix, "#INITIAL", N, name2index, index2name);

  // update the matrix based on changedConfigName
  update_graph(adjacent_matrix, changedConfigName, name2index, index2name);

  //exchange the updated routing message again.
  send_routing(adjacent_matrix, N, name2index, index2name, true);

  // print the current routing table
  print_routing(adjacent_matrix, "#FINAL", N, name2index, index2name);
  return 0;
}
