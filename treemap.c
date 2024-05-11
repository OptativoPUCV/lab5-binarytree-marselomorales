#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap *tree = (TreeMap *)malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    TreeNode *node = createTreeNode(key, value);
    if (tree->root == NULL)
    {
        tree->root = node;
        tree->current = node;
        return;
    }

    TreeNode *aux = tree->root;
    while (1)
        {
            Pair *aux_pair = aux->pair;
            void *key_aux = aux_pair->key;
            if(is_equal(tree, key_aux, key)) return;
            if(aux->left == NULL || aux->right == NULL) break;

            if(tree->lower_than(key, key_aux)) aux = aux->left;
            else aux = aux->right;
        }

    if(aux->left == NULL) aux->left = node;
    else aux->right = node;

    node->parent = aux;
    tree->current = node;
}

TreeNode * minimum(TreeNode * x)
{
    TreeNode *aux = x;
    while(aux->left != NULL) aux = aux->left;
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    TreeNode *eliminar = node;
    TreeNode *padre = node->parent;
    TreeNode *hijo = NULL;

    //Caso Nodo sin hijos
    if(eliminar->left == NULL && eliminar->right == NULL)
    {
        if(padre->left == eliminar) padre->left = NULL;
        else padre->right = NULL;
    }

    //Caso Nodo con un hijo
    else if(eliminar->left == NULL || eliminar->right == NULL)
    {
        if(eliminar->left != NULL) hijo = eliminar->left;
        else hijo = eliminar->right;

        if(padre->left == eliminar) padre->left = hijo;
        else padre->right = hijo;

        hijo->parent = padre;
    }

    else
    {
        TreeNode *min = minimum(eliminar->right);
        removeNode(tree, min);
        eliminar->pair->key = min->pair->key;
    }
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    TreeNode *aux = tree->root;
    while (aux != NULL)
        {
            Pair *aux_pair = aux->pair;
            void *key_aux = aux_pair->key;
            if(is_equal(tree, key_aux, key))
            {
                tree->current = aux;
                return aux_pair;
            }
            if(tree->lower_than(key, key_aux)) aux = aux->left;
            else aux = aux->right;
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    TreeNode *nodo = minimum(tree->root);
    tree->current = nodo;
    return nodo->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode *aux = tree->current;
    if(aux->right != NULL)
    {
        TreeNode *minimo = minimum(aux->right);
        tree->current = minimo;
        return minimo->pair;
    }
    else
    {
        while(aux->parent != NULL && aux->parent->right == aux) aux = aux->parent;

        if (aux->parent != NULL)
        {
            tree->current = aux->parent;
            return aux->parent->pair;
        }
    }
    return NULL;
}
