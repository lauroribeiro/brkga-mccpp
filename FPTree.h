#ifndef FPTREE_H
#define FPTREE_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

typedef pair<int, int> VertexColor; // Definindo o tipo VertexColor como par de inteiros

class FPNode {
public:
    VertexColor item;
    int count;
    FPNode* parent;
    map<VertexColor, FPNode*> children; // Usando std::map ao invés de unordered_map
    FPNode* link;

    FPNode(VertexColor item, FPNode* parent) : item(item), count(1), parent(parent), link(nullptr) {}
};

class FPTree {
public:
    FPNode* root;
    map<VertexColor, vector<FPNode*> > headerTable; // Usando std::map ao invés de unordered_map

    FPTree() {
        VertexColor rootItem = make_pair(-1, -1);
        root = new FPNode(rootItem, nullptr); // Nó raiz com item (-1, -1)
    }

    void addTransaction(const vector<VertexColor>& transaction) {
        FPNode* current = root;
        for (const VertexColor& item : transaction) {
            if (current->children.find(item) == current->children.end()) {
                current->children[item] = new FPNode(item, current);
                headerTable[item].push_back(current->children[item]);
            } else {
                current->children[item]->count++;
            }
            current = current->children[item];
        }
    }

    ~FPTree() {
        deleteTree(root);
    }

private:
    void deleteTree(FPNode* node) {
        if (node) {
            for (auto& child : node->children) {
                deleteTree(child.second);
            }
            delete node;
        }
    }
};

void printTree(FPNode* node, int indent = 0) {
    if (node->item.first != -1) { // Não imprimir o nó raiz
        cout << string(indent, ' ') << "(" << node->item.first << ", " << node->item.second << ", " << node->count << ")" << endl;
    }
    for (auto& child : node->children) {
        printTree(child.second, indent + 2);
    }
}

void findFrequentPatterns(FPTree& tree, int minSupport, vector<VertexColor> prefix, vector<vector<VertexColor> >& patterns) {
    for (auto& entry : tree.headerTable) {
        VertexColor item = entry.first;
        int support = 0;
        for (FPNode* node : entry.second) {
            support += node->count;
        }

        if (support >= minSupport) {
            vector<VertexColor> newPrefix = prefix;
            newPrefix.push_back(item);
            patterns.push_back(newPrefix);

            vector<vector<VertexColor> > conditionalPatterns;
            for (FPNode* node : entry.second) {
                int count = node->count;
                vector<VertexColor> path;
                FPNode* parent = node->parent;
                while (parent->item.first != -1) {
                    path.push_back(parent->item);
                    parent = parent->parent;
                }
                for (int i = 0; i < count; ++i) {
                    conditionalPatterns.push_back(path);
                }
            }

            FPTree conditionalTree;
            for (auto& pattern : conditionalPatterns) {
                reverse(pattern.begin(), pattern.end());
                conditionalTree.addTransaction(pattern);
            }

            findFrequentPatterns(conditionalTree, minSupport, newPrefix, patterns);
        }
    }
}

void findMaximalPatterns(FPTree& tree, int minSupport, vector<VertexColor> prefix, vector<vector<VertexColor>>& maximalPatterns) {
    bool isMaximal = true;

    for (auto& entry : tree.headerTable) {
        VertexColor item = entry.first;
        int support = 0;
        for (FPNode* node : entry.second) {
            support += node->count;
        }

        if (support >= minSupport) {
            isMaximal = false;

            vector<VertexColor> newPrefix = prefix;
            newPrefix.push_back(item);

            vector<vector<VertexColor>> conditionalPatterns;
            for (FPNode* node : entry.second) {
                int count = node->count;
                vector<VertexColor> path;
                FPNode* parent = node->parent;
                while (parent->item.first != -1) {
                    path.push_back(parent->item);
                    parent = parent->parent;
                }
                for (int i = 0; i < count; ++i) {
                    conditionalPatterns.push_back(path);
                }
            }

            FPTree conditionalTree;
            for (auto& pattern : conditionalPatterns) {
                reverse(pattern.begin(), pattern.end());
                conditionalTree.addTransaction(pattern);
            }

            findMaximalPatterns(conditionalTree, minSupport, newPrefix, maximalPatterns);
        }
    }

    if (isMaximal && !prefix.empty()) {
        maximalPatterns.push_back(prefix);
    }
}




#endif // FPTREE_H
