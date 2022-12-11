#include <iostream>
#include "C:\Users\User\Desktop\c++\practice\practice\Serial.cpp"
#include "Iterator.cpp"
using namespace std;
//узел
template<class T>
class Node {
protected:
//закрытые переменные Node N; N.data = 10 вызовет ошибку
    T data;

//не можем хранить Node, но имеем право хранить указатель
    Node *left;
    Node *right;
    Node *parent;

//переменная, необходимая для поддержания баланса дерева
    int height;

public:
//доступные извне переменные и функции

    //int balance;
    virtual void setData(T d) { data = d; }

    virtual T getData() { return data; }

    int getHeight() { return height; }

    virtual Node *getLeft() { return left; }

    virtual Node *getRight() { return right; }

    virtual Node *getParent() { return parent; }

    virtual void setLeft(Node *N) { left = N; }

    virtual void setRight(Node *N) { right = N; }

    virtual void setParent(Node *N) { parent = N; }

//Конструктор. Устанавливаем стартовые значения для указателей
    Node<T>(T n) {
        data = n;
        left = right = parent = NULL;
        height = 1;
    }

    Node<T>() {
        left = NULL;
        right = NULL;
        parent = NULL;
        data = 0;
        height = 1;
    }

    virtual void print() {
        cout << "\n" << data;
    }

    virtual void setHeight(int h) {
        height = h;
    }

    template<class M> friend ostream& operator<< (ostream&  stream, Node<M>& N);
};

template<class T>
ostream& operator<< (ostream& stream, Node<T>& N)
{
    stream << "\nNode data: " << N.data << ", height: " << N.height;
    return stream;
}

template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
//корень - его достаточно для хранения всего дерева
    Node<T>* root;
public:
//доступ к корневому элементу
    virtual Node<T>* getRoot() { return root; }
//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
    Tree<T>() { root = NULL; }
//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
    virtual Node<T>* Add_R(Node<T>* N)
    {
        return Add_R(N, root);
    }

    virtual Node<T>* Add_R(Node<T>* N, Node<T>* Current)
    {
        if (N == NULL) return NULL;
        if (root == NULL)
        {
            root = N;
            return N;
        }
        if (Current->getData() > N->getData())
        {
//идем влево
            if (Current->getLeft() != NULL)
                Current->setLeft(Add_R(N, Current->getLeft()));
            else
                Current->setLeft(N);
            Current->getLeft()->setParent(Current);
        }
        if (Current->getData() < N->getData())
        {
//идем вправо
            if (Current->getRight() != NULL)
                Current->setRight(Add_R(N, Current->getRight()));
            else
                Current->setRight(N);
            Current->getRight()->setParent(Current);
        }
        if (Current->getData() == N->getData());
//нашли совпадение

//для несбалансированного дерева поиска
        return Current;
    }
//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
    virtual void Add(int n)
    {
        Node<T>* N = new Node<T>;
        N->setData(n);
        Add_R(N);
    }
    virtual Node<T>* Min(Node<T>* Current)
    {
//минимум - это самый "левый" узел. Идём по дереву всегда влево
        if (root == NULL) return NULL;
        if(Current == NULL)
            Current = root;
        while (Current->getLeft() != NULL)
            Current = Current->getLeft();
        return Current;
    }
    virtual Node<T>* Max(Node<T>* Current = NULL)
    {
//максимум - это самый "правый" узел. Идём по дереву всегда вправо
        if (root == NULL) return NULL;
        if (Current == NULL)
            Current = root;
        while (Current->getRight() != NULL)
            Current = Current->getRight();
        return Current;
    }
//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
    virtual Node<T>* Find(int data, Node<T>* Current)
    {
//база рекурсии
        if (Current == NULL) return NULL;
        if (Current->getData() == data) return Current;
//рекурсивный вызов
        if (Current->getData() > data) return Find(data,Current->getLeft());
        if (Current->getData() < data) return Find(data,Current->getRight());
    }
//три обхода дерева
    virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
    {
        if (N != NULL)
            f(N);
        if (N != NULL && N->getLeft() != NULL)
            PreOrder(N->getLeft(), f);
        if (N != NULL && N->getRight() != NULL)
            PreOrder(N->getRight(), f);
    }
//InOrder-обход даст отсортированную последовательность
    virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
    {
        if (N != NULL && N->getLeft() != NULL)
            InOrder(N->getLeft(), f);
        if (N != NULL)
            f(N);
        if (N != NULL && N->getRight() != NULL)
            InOrder(N->getRight(), f);
    }
    virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
    {
        if (N != NULL && N->getLeft() != NULL)
            PostOrder(N->getLeft(), f);
        if (N != NULL && N->getRight() != NULL)
            PostOrder(N->getRight(), f);
        if (N != NULL)
            f(N);
    }
};

bool operator < (Serial s1, Serial s2) {
    return (s1.rating == s2.rating) ? s1.name > s2.name : s1.rating < s2.rating;
}

class AlreadyUsed : public exception {
public:
    const char * what() const noexcept override {
        cout << "This element is already used\n";
    }
};

template <class T>
class BalancedTree : public Tree<T> {
    const bool LEFT = true;
    const bool RIGHT = false;

    void disconnectNode(Node<T>* node) {
        Node<T>* parentNode = node->getParent();
        Node<T>* rightChild = node->getRight();
        Node<T>* leftChild = node->getLeft();

        if(parentNode != NULL) {
            if (node == parentNode->getLeft())  parentNode->setLeft(NULL);
            if (node == parentNode->getRight()) parentNode->setRight(NULL);

            node->setParent(NULL);
        }

        if(rightChild != NULL) {
            rightChild->setParent(NULL);
            node->setRight(NULL);
        }

        if(leftChild != NULL) {
            leftChild->setParent(NULL);
            node->setLeft(NULL);
        }
    }

    void connectNodes(Node<T>* node, Node<T>* nodeToConnect,bool side) {
        if(node != NULL) {
            if (side == LEFT)  node->setLeft(nodeToConnect);
            if (side == RIGHT) node->setRight(nodeToConnect);
        }

        if(nodeToConnect != NULL) nodeToConnect->setParent(node);
    }


    void movement(Node<T>* nodeIB, Node<T>* childOfInbalance, bool sideOfInbalance, Node<T>* brotherOfInbalance) {
        Node<T>* childLeft = childOfInbalance->getLeft();
        Node<T>* childRight = childOfInbalance->getRight();

        disconnectNode(childOfInbalance);
        disconnectNode(nodeIB);

        connectNodes(nodeIB,brotherOfInbalance , !sideOfInbalance);
        if (sideOfInbalance == LEFT) {
            connectNodes(nodeIB, childRight, sideOfInbalance);
            connectNodes(childOfInbalance,childLeft , sideOfInbalance);
        }
        else {
            connectNodes(nodeIB, childLeft, sideOfInbalance);
            connectNodes(childOfInbalance,childRight , sideOfInbalance);
        }

        connectNodes(childOfInbalance, nodeIB, !sideOfInbalance);
    }


    void balanceDBZero(Node<T>* nodeIB, bool sideOfInbalance, bool whereNodeIB) {
        Node<T>* parent = nodeIB->getParent();
        Node<T>* childL = nodeIB->getLeft();
        Node<T>* childR = nodeIB->getRight();

        if(sideOfInbalance == LEFT) {
            auto c = childR;
            childR = childL;
            childL = c;
        }

        movement(nodeIB, childR, sideOfInbalance, childL);

        connectNodes(parent,childR, whereNodeIB);

        childR->setHeight(childR->getHeight() + 1);
        nodeIB->setHeight(nodeIB->getHeight() - 1);

    }

    void balanceDBMinusOne(Node<T>* nodeIB, bool sideOfInbalanсe, bool whereNodeIB) {
        Node<T>* parent = nodeIB->getParent();
        Node<T>* childL = nodeIB->getLeft();
        Node<T>* childR = nodeIB->getRight();

        if(sideOfInbalanсe == LEFT) {
            auto c = childR;
            childR = childL;
            childL = c;
        }

        movement(nodeIB, childR, sideOfInbalanсe, childL);

        connectNodes(parent, childR, whereNodeIB);

        nodeIB->setHeight(nodeIB->getHeight() - 2);
    }

    void balanceDBPlusOne(Node<T>* nodeIB, bool sideOfInbalance, bool whereNodeIB) {
        Node<T>* leftChild = nodeIB->getLeft();
        Node<T>* rightChild = nodeIB->getRight();


        if(sideOfInbalance == LEFT) {
            auto temp = rightChild;
            rightChild = leftChild;
            leftChild = temp;
        }

        balanceDBZero(rightChild, !sideOfInbalance,sideOfInbalance);
        balanceDBMinusOne(nodeIB,sideOfInbalance,whereNodeIB);
    }

    int delta(Node<T>* node) {
        if(node == NULL) return 0;

        int leftChild = node->getLeft()   == NULL ? 0 : node->getLeft()->getHeight();
        int rightChild = node->getRight() == NULL ? 0 : node->getRight()->getHeight();
        return leftChild - rightChild;
    }

    void nodeBalance(Node<T>* node) {
        int d = delta(node);

        if(d < 2 && d > -2) return;

        bool whereNode;

        if(node->getParent() != NULL)
            whereNode = (node == node->getParent()->getLeft()) ? LEFT : RIGHT;
        else
            whereNode = true;

        if (d == 2) {
            int deltaChild = delta(node->getLeft());

            if(deltaChild == 0)  balanceDBZero(node,LEFT,whereNode);
            if(deltaChild == -1) balanceDBPlusOne(node,LEFT,whereNode);
            if (deltaChild == 1) balanceDBMinusOne(node,LEFT,whereNode);
        }
        if (d == -2) {
            int deltaChild = delta(node->getRight());
            if(deltaChild == 0)  balanceDBZero(node,RIGHT,whereNode);
            if(deltaChild == 1)  balanceDBPlusOne(node,RIGHT,whereNode);
            if(deltaChild == -1) balanceDBMinusOne(node,RIGHT,whereNode);
        }

    }

    void calcHeight(Node<T>* node) {
        if(node == NULL) return;
        int lCh = node->getLeft()  == NULL ? 0 : node->getLeft()->getHeight();
        int rCh = node->getRight() == NULL ? 0 : node->getRight()->getHeight();

        node->setHeight(1 + max(lCh, rCh));
    }
    Node<T>* Add_R(Node<T>* N, Node<T>* Current) override {
        if (N == NULL) return NULL;

        if (Tree<T>::root == NULL) {
            Tree<T>::root = N;
            return N;
        }

        if (N->getData() == Current->getData()) {
            throw AlreadyUsed();
        }

        if (Current->getData() > N->getData()) {
            //идем влево
            if (Current->getLeft() != NULL) Add_R(N, Current->getLeft());
            else {
                Current->setLeft(N);
                Current->getLeft()->setParent(Current);
            }
        }

        if (Current->getData() < N->getData()) {
            //идем вправо
            if (Current->getRight() != NULL) Add_R(N, Current->getRight());
            else {
                Current->setRight(N);
                Current->getRight()->setParent(Current);
            }
        }

        nodeBalance(Current);
        calcHeight(Current);

        if (this->getRoot()->getParent() != NULL) Tree<T>::root = this->getRoot()->getParent();

        return Current;
    }

    Node<T>* Add_R(Node<T>* N) override {
        return Add_R(N,this->getRoot());
    }

public:

    virtual void Add(T data) override {
        Node<T>* N = new Node<T>;
        N->setData(data);
        Add_R(N);
    }
    virtual Node<T>* Find(T data) {
        return Tree<T>::Find(data, Tree<T>::root);
    }
};


int main()
{
//    Tree<double> T;
//    int arr[15];
//    int i = 0;
//    for (i = 0; i < 15; i++) arr[i] = (int)(100 * cos(15 * double(i+1)));
//    for (i = 0; i < 15; i++)
//        T.Add(arr[i]);
//
//    Node<double>* M = T.Min();
//    cout << "\nMin = " << M->getData()
//        << "\tFind " << arr[3] << ": " << T.Find(arr[3], T.getRoot());
    void (*f_ptr)(Node<double>*);
    f_ptr = print;
//    cout << "\n-----\nInorder:";
//    T.InOrder(T.getRoot(), f_ptr);
//    char c; cin >> c;

    BalancedTree<int> T = BalancedTree<int> ();

    for (int i = 0; i < 6; i++) {
        T.Add(i);
    }

    cout << T.getRoot()->getData() << endl;
    cout << T.Min(T.getRoot())->getData() << endl;
    cout << T.Find(3)->getData() << endl;
    return 0;
}
