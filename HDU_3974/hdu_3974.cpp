#include <cstdio>
#include <vector>
#include <cassert>
#include <cstring>

const int MAXN = 5E4+3;

class Graph {
public:
    void init_size(int node_count) {
        this->_node_count = node_count;
        _edges.resize(node_count+1);
    }
    void add_edge(int u, int v) {
        _edges[u].push_back(v);
    }
    void dfs(int now, int& index, std::vector<int>& tree_id, std::vector<int>& last_son_id) {
        tree_id[now] = ++index;
        for (int i = 0; i < (int)_edges[now].size(); ++i) {
            dfs(_edges[now][i], index, tree_id, last_son_id);
        }
        last_son_id[now] = index;
    }
private:
    int _node_count;
    std::vector<std::vector<int> > _edges;
};

class SegmentTree {
public:
    static const int ROOT_INDEX;

    void init(int max_length) {
        _values.resize(max_length<<2|1);
        _lazy.resize(max_length<<2|1);
    }

    void build(int left, int right, int root) {
        _values[root] = -1;
        _lazy[root] = 0;
        if (left == right) {
            return;
        }
        int mid = (left + right) >> 1;
        build(left, mid, root<<1);
        build(mid+1, right, root<<1|1);
    }

    void update(int left, int right, int root, int update_left, int update_right, int new_value) {
        if (update_left > right || update_right < left) {
            return;
        } else if (update_left <= left && right <= update_right) {
            _values[root] = new_value;
            _lazy[root] = 1;
            return;
        } else {
            int mid = (left + right) >> 1;
            push_down(root, left, right);
            update(left, mid, root<<1, update_left, update_right, new_value);
            update(mid+1, right, root<<1|1, update_left, update_right, new_value);
        }
    }

    int query(int left, int right, int root, int query_index) {
        if (left == right) {
            return _values[root];
        }
        push_down(root, left, right);
        int mid = (left + right) >> 1;
        if (query_index <= mid) {
            return query(left, mid, root<<1, query_index);
        } else {
            return query(mid+1, right, root<<1|1, query_index);
        }
    }

private:
    std::vector<int> _values;
    std::vector<int> _lazy;
    void push_down(int root, int left, int right) {  
        if (_lazy[root] != 0) { 
            int mid = (left + right) >> 1;
            _values[root<<1] = _values[root<<1|1] = _values[root];
            _lazy[root<<1] = _lazy[root<<1|1] = 1;
            _lazy[root] = 0;
        }
    }
};

const int SegmentTree::ROOT_INDEX = 1;

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }
    int T, n, m, u, v;
    scanf("%d", &T);

    SegmentTree * ptree = new SegmentTree();
    ptree->init(MAXN);
    static int indegree[MAXN];
    for (int ca = 1; ca <= T; ++ca) {
        scanf("%d", &n);
        Graph * pgraph = new Graph();
        pgraph->init_size(n);
        memset(indegree, 0, sizeof(int)*(n+1));
        for (int i = 1; i < n; ++i) {
            scanf("%d%d", &u, &v);
            pgraph->add_edge(v, u);
            ++indegree[u];
        }
        std::vector<int> tree_id;
        std::vector<int> last_son_id;
        tree_id.resize(n+1);
        last_son_id.resize(n+1);
        int root_index = -1;
        for (int i = 1; i <= n; ++i) {
            if (indegree[i] == 0) {
                root_index = i;
                break;
            }
        }

        assert(root_index != -1);
        int node_index = 0;
        pgraph->dfs(root_index, node_index, tree_id, last_son_id); 

        ptree->build(1, n, SegmentTree::ROOT_INDEX);
        
        scanf("%d", &m);
        printf("Case #%d:\n", ca);
        char op[2];
        while (m--) {
            scanf("%s", op);
            switch (op[0]) {
            case 'C':
                int query_index;
                scanf("%d", &query_index);
                printf("%d\n", ptree->query(1, n, SegmentTree::ROOT_INDEX, tree_id[query_index]));
                break;
            case 'T':
                int update_root, new_value;
                scanf("%d%d", &update_root, &new_value);
                ptree->update(1, n, SegmentTree::ROOT_INDEX, tree_id[update_root], last_son_id[update_root], new_value);
                break;
            }
        }
        delete pgraph;
    }
    
    delete ptree;

    if (argc == 2) {
        fclose(stdin);
    }

    return 0;
}
