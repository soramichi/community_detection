// Implementation of the 1st phase of "Fast unfolding of communities in large networks".
// The 2nd phase is not needed given that we do not consider hierarchies??
// This program uses eq. (1) of the paper because eq. (2) is more complex
// and using eq. (1) directly is sufficient for our need (small number of nodes)

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

inline int index(int i, int j, int n) {
  return i * n + j;
}

class community {
public:
  vector<int> members;
  
  community(const vector<int>& init_nodes) {
    for(int n: init_nodes) {
      members.push_back(n);
    }
  }

  void add(int node) {
    members.push_back(node);
  }

  bool remove(int node) {
    for(auto it = members.begin(); it != members.end(); it++) {
      // found the matching member, remove it
      if (*it == node) {
	members.erase(it);
	return true;
      }
    }

    // nothing has been removed
    return false;
  }

  int n_members() const {
    return members.size();
  }

  bool empty() const {
    return (this->n_members() == 0);
  }

  // eq. (1)
  double Q(int M /* == 2m */, const int* edges, int n, const int* k) {
    double Qval = 0;

    for(auto it1 = members.begin(); it1 != members.end(); it1++) {
      for(auto it2 = members.begin(); it2 != members.end(); it2++) {
	int i = *it1, j = *it2;
	Qval += edges[index(i, j, n)] - (k[i] * k[j] /(double)M);
      }
    }

    Qval /= M;
    return Qval;
  }

  bool included(int node) const {
    return (find(members.begin(), members.end(), node) != members.end());
  }

  void print() const {
    for(const auto& n: members) {
      cout << n << " ";
    }
    cout << endl;
  }
};

community* find_comm(vector<community>& comms, int node) {
  for(auto& c: comms) {
    if (c.included(node))
      return &c;
  }

  cerr << "Warn: nothing found in find_comm (node: " << node << ")" << endl;
  return NULL;
}

int main() {
  int* edges;
  int n_edges, n_nodes;

  cin >> n_nodes >> n_edges;
  cout << "number of nodes: " << n_nodes << endl;
  cout << "number of edges: " << n_edges << endl;

  edges = new int[n_nodes * n_nodes];

  for(int i = 0; i < n_edges; i++) {
    int start, end, weight;
    
    cin >> start >> end >> weight;
    cout << "edge " << i + 1 << ": " << start << " -> " << end << " (" << weight << ")" << endl;

    edges[index(start, end, n_nodes)] = weight;
    edges[index(end, start, n_nodes)] = weight;
  }

  vector<community> comms;
  for(int i = 0; i<n_nodes; i++) {
    comms.push_back(community({i}));
  }

  // M == 2m in eq. (1)
  int M = accumulate(&edges[0], &edges[n_nodes * n_nodes], 0);
  int* k = new int[n_nodes];
  for(int i = 0; i < n_nodes; i++) {
    k[i] = accumulate(&edges[index(i, 0, n_nodes)], &edges[index(i, n_nodes, n_nodes)], 0);
  }

  bool node_moved;

  do {
    node_moved = false;

    for(int i = 0; i < n_nodes; i++) {      
      community *from_comm, *to_comm;
      double Qgain_max = 0.0;

      from_comm = find_comm(comms, i);

      for(int j = 0; j < n_nodes; j++) {
	if (edges[index(i, j, n_nodes)] == 0) {
	  continue;
	}

	// new_comms is a deep copy of comms
	vector<community> new_comms;
	for(const auto& c: comms) {
	  new_comms.push_back(c);
	}

	// calculate the gain of Q
	// i_comm: the community that node i belongs to
	// j_comm: the community that node j belongs to (which is the one node i will be moved to)
	community* i_comm_old = from_comm;	// i_comm_old is always the same for a given i
	community* j_comm_old = find_comm(comms, j);
	community* i_comm_new = find_comm(new_comms, i);
	community* j_comm_new = find_comm(new_comms, j);

	i_comm_new->remove(i);
	j_comm_new->add(i);

	// Calculate Qgain
	// Note: Q of communities that neither i or j belong to are the same in the old and new networks,
	// so we do not need to consider them (even if we do, they just cancel each other).
	double Qold =  i_comm_old->Q(M, edges, n_nodes, k) + j_comm_old->Q(M, edges, n_nodes, k);
	double Qnew = i_comm_new->Q(M, edges, n_nodes, k) + j_comm_new->Q(M, edges, n_nodes, k);
	double Qgain = Qnew - Qold;
	
	if (Qgain > Qgain_max) {
	  to_comm = j_comm_old;
	  Qgain_max = Qgain;
	}
      }

      if (Qgain_max > 0) {
	from_comm->remove(i);
	to_comm->add(i);
	node_moved = true;
      }
    }
  } while(node_moved);

  cout << "\nResult:" << endl;
  for(auto& c: comms) {
    if (!c.empty()) {
      c.print();
    }
  }
  
  return 0;
}
