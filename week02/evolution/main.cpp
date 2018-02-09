#include <iostream>
#include <string>
#include <map>

using namespace std;

struct node {
    string name;
    size_t age;
    node *parent;
};

string find_ancestor(node *curr_node, size_t age_limit)
{
    if (!curr_node->parent || curr_node->parent->age > age_limit)
        return curr_node->name;
    return find_ancestor(curr_node->parent, age_limit);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    size_t t;
    cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n, q;
        cin >> n >> q;
        map<string, node*> nodes;
        string name;
        size_t age;
        for (size_t i = 0; i < n; ++i)
        {
            cin >> name >> age;
            node *tmp_node;
            tmp_node = new node();
            tmp_node->name = name;
            tmp_node->age = age;
            tmp_node->parent = nullptr;
            nodes[name] = tmp_node;
        }
        string child, parent;
        for (size_t i = 0; i < n - 1; ++i)
        {
            cin >> child >> parent;
            nodes[child]->parent = nodes[parent];
        }
        string *res;
        res = new string[q];
        for (size_t i = 0; i < q; ++i)
        {
            cin >> name >> age;
            res[i] = find_ancestor(nodes[name], age);
        }
        cout << res[0];
        for (size_t i = 1; i < q; ++i)
            cout << " " << res[i];
        cout << endl;
    }
    return 0;
}
