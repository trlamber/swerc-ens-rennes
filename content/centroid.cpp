
/** Décomposition en centroïdes
 * Statut : testé sur CSES
**/
struct Centroid{
	vector<vector<int>> adj;
	vector<int> subtree, peres;
	vector<bool> r;
 
	Centroid(vector<vector<int>>& _adj):adj(_adj){
		int n = _adj.size();
		r = vector<bool>(n);
		peres = vector<int>(n,-1);
		subtree = vector<int>(n,1);
	}
 
	int init(int node, int pere){
		subtree[node] = 1;
		for (int voisin:adj[node]){
			if (voisin != pere and !r[voisin]){
				subtree[node] += init(voisin,node);
			}
		}
		return subtree[node];
	}
	
	int get_centroid(int node, int pere, int siz){
		for (int voisin:adj[node]){
			if (voisin != pere and !r[voisin] and subtree[voisin]*2 > siz){
				return get_centroid(voisin,node,siz);
			}
		}
		return node;
	}
 
	void get_tree(int node, int pere){
		int C = get_centroid(node,-1,init(node,-1));
		r[C] = true;
		peres[C] = pere;
		for (int i:adj[C]){
			if (!r[i]){
				get_tree(i,C);
			}
		}
	}
};
