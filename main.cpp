#include <iostream>
#include <fstream>
#include <set>
using namespace std;
ifstream fin("abce.in");
ofstream fout("abce.out");

struct Nod{
    int key;
    Nod *left, *right; // *father;
    int height;
    Nod(int key = 0)
    {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
        //this->father = nullptr;
        this->height = 1;
    }
};

int getHeight(Nod* nod)
{
    if(nod == nullptr) return 0;
    return nod->height;
}

void updateHeight(Nod* nod)
{
    nod->height = 1 + max(getHeight(nod->left), getHeight(nod->right));
}

int getBalance(Nod* nod)
{
    if(nod == nullptr) return 0;
    return getHeight(nod->left) - getHeight(nod->right);
}

Nod* leftRotate(Nod* nod)
{
    Nod* x = nod->right;
    Nod* T2 = x->left;

    x->left = nod;
    nod->right = T2;
    updateHeight(nod);
    updateHeight(x);

    return x;
}

Nod* rightRotate(Nod* nod)
{
    Nod* x = nod->left;
    Nod* T2 = x->right;

    x->right = nod;
    nod->left = T2;
    updateHeight(nod);
    updateHeight(x);

    return x;
}

Nod* balanceNode(Nod* nod)
{
    if(nod == nullptr) return nullptr;
    int balance = getBalance(nod);
    int leftBalance = getBalance(nod->left);
    int rightBalance = getBalance(nod->right);

    if(balance > 1 && (leftBalance == 0 || leftBalance == 1))
        return rightRotate(nod);
    else if(balance < -1 && (rightBalance == 0 || rightBalance == -1))
        return leftRotate(nod);
    else if(balance > 1 && leftBalance == -1)
    {
        nod->left = leftRotate(nod->left);
        return rightRotate(nod);
    }
    else if(balance < -1 && rightBalance == 1)
    {
        nod->right = rightRotate(nod->right);
        return leftRotate(nod);
    }

    return nod;
}

Nod* goRight(Nod* root)
{
    if(root->right != nullptr)
        return goRight(root->right);
    return root;
}

Nod* goLeft(Nod* root)
{
    if(root->left != nullptr)
        return goLeft(root->left);
    return root;
}

Nod* predecesor(Nod* root)
{
    if(root->left != nullptr) return goRight(root->left);
    return root;
}

Nod* succesor(Nod* root)
{
    if(root->right != nullptr) return goLeft(root->right);
    return root;
}

Nod* father(Nod* root, int key)
{
    if(root == nullptr) return nullptr;
    if(root->left != nullptr && key == root->left->key) return root;
    if(root->right != nullptr && key == root->right->key) return root;
    if(key < root->key) return father(root->left, key);
    if(key > root->key) return father(root->right, key);

    return nullptr;
}

Nod* fixBalance(Nod* root)
{
    if(root == nullptr) return nullptr;
    root->left = fixBalance(root->left);
    root->right = fixBalance(root->right);

    updateHeight(root);

    int balance = getBalance(root);
    if(balance < -1 || balance > 1) return balanceNode(root);

    return root;

}

Nod* push(Nod* root, int key)
{
    if(root == nullptr) return new Nod(key);
    if(key < root->key) root->left = push(root->left, key);
    else if(key > root->key) root->right = push(root->right, key);
    else return root;

    updateHeight(root);

    int balance = getBalance(root);
    if(balance < -1 || balance > 1) return balanceNode(root);

    return root;
}

Nod* pop(Nod* root, int key)
{
    if(root == nullptr) return nullptr;
    if(key < root->key) root->left = pop(root->left, key);
    else if(key > root->key) root->right = pop(root->right, key);
    else
    {
        Nod* aux;
        if(root->left == nullptr && root->right == nullptr) return nullptr;
        if(root->left == nullptr && root->right != nullptr)
        {
            aux = root->right;
            delete root;
            return aux;
        }
        if(root->left != nullptr && root->right == nullptr)
        {
            aux = root->left;
            delete root;
            return aux;
        }

        Nod* pred = predecesor(root);
        if(pred == root->left)
        {
            pred->right = root->right;
            delete root;
            updateHeight(pred);
            int balance = getBalance(pred);
            if(balance < -1 || balance > 1) return balanceNode(pred);
            return pred;
        }
        Nod* fat = father(root, pred->key);
        fat->right = pred->left;
        pred->left = root->left;
        pred->right = root->right;
        delete root;

        return fixBalance(pred);
    }

    updateHeight(root);
    int balance = getBalance(root);
    if(balance < -1 || balance > 1) return balanceNode(root);

    return root;

}

bool findNod(Nod* root, int key)
{
    if(root == nullptr) return false;
    if(key < root->key) return findNod(root->left, key);
    if(key > root->key) return findNod(root->right, key);
    return true;
}

int* cautaMareMic(Nod* root, int key)
{

    if(root == nullptr) return nullptr;

    int *mareMic;
    if(key < root->key)
    {
        mareMic = cautaMareMic(root->left, key);
        if(root->key <= key && mareMic == nullptr) return &(root->key);
        if(root->key <= key && root->key > *mareMic) return &(root->key);
        return mareMic;
    }

    if(key > root->key)
    {
        mareMic = cautaMareMic(root->right, key);
        if(root->key <= key && mareMic == nullptr) return &(root->key);
        if(root->key <= key && root->key > *mareMic) return &(root->key);
        return mareMic;
    }
    return &(root->key);
}

int* cautaMicMare(Nod* root, int key)
{

    if(root == nullptr) return nullptr;

    int *micMare;
    if(key < root->key)
    {
        micMare = cautaMicMare(root->left, key);
        if(root->key >= key && micMare == nullptr) return &(root->key);
        if(root->key >= key && root->key < *micMare) return &(root->key);
        return micMare;
    }

    if(key > root->key)
    {
        micMare = cautaMicMare(root->right, key);
        if(root->key >= key && micMare == nullptr) return &(root->key);
        if(root->key >= key && root->key < *micMare) return &(root->key);
        return micMare;
    }
    return &(root->key);
}
set<int> setul;
void inOrderXY(Nod* root, int x, int y)
{

    if(root == nullptr) return;
    if(x < root->key) inOrderXY(root->left, x, y);
    if(x > root->key) inOrderXY(root->right, x, y);

    if(root->key >= x && root->key <= y) setul.insert(root->key);

    if(y < root->key) inOrderXY(root->left, x, y);
    if(y > root->key) inOrderXY(root->right, x, y);
}

void callInOrderXY(Nod* root, int x, int y)
{
    setul.clear();
    inOrderXY(root, x, y);
    for(auto it = setul.begin(); it != setul.end(); it++)
        fout<<*it<<" ";
    fout<<"\n";
}

void preorder(Nod* root)
{
    if(root != nullptr)
    {
        cout<<root->key<<" ";
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(Nod* root)
{
    if(root != nullptr)
    {
        inorder(root->left);
        cout<<root->key<<" ";
        inorder(root->right);
    }
}

int main()
{
    Nod *root = nullptr;
    int n, comanda, x, y;
    fin>>n;
    for(int i = 1; i <= n; i++)
    {
        fin>>comanda;
        switch(comanda)
        {
            case 1:
            {
                fin>>x;
                root = push(root, x);
                break;
            }
            case 2:
            {
                fin>>x;
                root = pop(root, x);
                break;
            }
            case 3:
            {
                fin>>x;
                fout<<findNod(root, x)<<"\n";
                break;
            }
            case 4:
            {
                fin>>x;
                int *nr = cautaMareMic(root, x);
                fout<<*nr<<"\n";
                break;
            }
            case 5:
            {
                fin>>x;
                int *nr = cautaMicMare(root, x);
                fout<<*nr<<"\n";
                break;
            }
            case 6:
            {
                fin>>x>>y;
                callInOrderXY(root, x, y);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}
