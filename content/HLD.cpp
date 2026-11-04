/*ca compile avec le struct LazyTree (faut include le fichier LazyTree)
VALS_IN_EDGES permet de faire les op sur aretes si true sinon c'est sur les noeuds
par defaut j'ai pris add et get comme fonctions
pas testé mais verifié statiquement par IA*/
template<int SZ, bool VALS_IN_EDGES> struct HLD{
    int N;
    vector<int> adj[SZ];
    int par[SZ], root[SZ], depth[SZ], sz[SZ], t;
    int tin[SZ];
    void push(int x, int y){
        adj[x].emplace_back(y);
        adj[y].emplace_back(x);
    }
    void dfs_siz(int x){
        sz[x] = 1;
        for (int& y:adj[x]){
            par[y] = x;
            depth[y] = depth[x]+1;
            adj[y].erase(find(begin(adj[y]),end(adj[y]),x));
            dfs_siz(y);
            sz[x] += sz[y];
            if (sz[y] > sz[adj[x][0]]) swap(y,adj[x][0]);
        }
    }
    void dfs_hld(int x){
        tin[x] = t++;
        for (int y:adj[x]){
            root[y] = (y == adj[x][0] ? root[x] : y);
            dfs_hld(y);
        }
    }
    void init(int _N, int R = 0){
        N = _N;
        par[R] = depth[R] = t = 0;
        dfs_siz(R);
        root[R] = R;
        dfs_hld(R);
    }
    int lca(int x, int y){
        for (; root[x] != root[y]; y = par[root[y]])
            if (depth[root[x]] > depth[root[y]]) swap(x,y);
        return depth[x] < depth[y] ? x : y;
    }
    int dist(int x, int y){
        return depth[x]+depth[y]-2*depth[lca(x,y)];
    }
    vector<int> v = vector<int>(SZ);
    LazyTree tree = LazyTree(v);
    template <class BinaryOp>
    void processPath(int x, int y, BinaryOp op){
        for (; root[x] != root[y]; y = par[root[y]]){
            if (depth[root[x]] > depth[root[y]]) swap(x,y);
            op(tin[root[y]],tin[y]);
        }
        if (depth[x] > depth[y]) swap(x,y);
        op(tin[x]+VALS_IN_EDGES,tin[y]);
    }
    void modifyPath(int x, int y, int v){
        processPath(x,y,[this, &v](int l, int r){
            tree.add(l,r+1,v);
        });
    }
    int queryPath(int x, int y){
        int res = 0;
        processPath(x,y,[this,&res](int l, int r){
            res += tree.get(l,r+1);
        });
        return res;
    }
    void modifySubtree(int x, int v){
        tree.add(tin[x]+VALS_IN_EDGES, tin[x]+sz[x],v);
    }
};
