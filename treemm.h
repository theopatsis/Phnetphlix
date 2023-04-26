//
//  treemm.h
//  Project 4
//
//  Created by Theodore Patsis on 8/3/23.
//

#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        :it_values(){
            i=0;
        }
        
        Iterator(std::vector<ValueType*> values){
            it_values=values;
            i=0;
        }

        ValueType& get_value() const
        {
            if(i<it_values.size()){
                return *it_values[i];
            }
            throw 1; //throws exception if i is not within appropriate values
        }

        bool is_valid() const
        {
            if(it_values.empty() || i>=it_values.size()){
                return false;
            }
            return true;
        }

        void advance()
        {
            if(i!=it_values.size())
                i++;
        }
    private:
        std::vector<ValueType*> it_values;
        int i;
    };

    TreeMultimap()
    {
        root=nullptr;
    }

    ~TreeMultimap()
    {
        freeTree(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if(root==nullptr){
            root=createNode(key, value);
            return;
        }
        Node* cur=root;
        for(;;){
            if(key==cur->key){ //same as the BST iterative insertion algorithm but if we find a duplicate we just add the value to the values vector
                cur->values.push_back(value);
                return;
            }
            else if(key<cur->key){
                if(cur->left != nullptr){
                    cur=cur->left;
                }
                else{
                    cur->left=createNode(key, value);
                    return;
                }
            }
            else if (key > cur->key){
                if(cur->right != nullptr){
                    cur=cur->right;
                } else{
                    cur->right=createNode(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* cur=root;
        while(cur!=nullptr){
            if (key==cur->key){
                std::vector <ValueType*> temp(cur->values.size()); //converts the vector of type ValueType to ValueType* so that we can call the Iterator constructor with the appropriate arguments
                for (int k=0; k<cur->values.size(); k++){ 
                    temp[k]=&(cur->values[k]);
                }
                return Iterator(temp);
            }
            else if (key < cur->key){
                cur=cur->left;
            }
            else{
                cur=cur->right;
            }
        }
        return Iterator(); //if not found, return an invalid Iterator object
    }
    
    private:
    
    struct Node{
      KeyType key;
      std::vector<ValueType> values;
      Node* left;
      Node* right;
    };
    
    Node* root;
    
    void freeTree(Node* cur){ //recursive helper function to destruct the tree when we're done
        if(cur==nullptr){
            return;
        }
        freeTree(cur->left);
        freeTree(cur->right);
        delete cur;
    }
    
    Node* createNode(KeyType key, ValueType value){
        Node* newNode = new Node();
        newNode->key = key;
        newNode->values.push_back(value);
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
};

#endif // TREEMULTIMAP_INCLUDED
