#include <iostream>
#include "C:\Users\User\Desktop\c++\practice\practice\Serial.cpp"
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
        if (Current->getData() == N->getData())
//нашли совпадение
            ;
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
public:
    void Add(T n)
    {
        Node<T>* N = new Node<T>(n);
        N->setData(n);
        Add_R(N, Tree<T>:: root);
    }

protected:

     Node<T>* Add_R(Node<T>* nodeToAdd, Node<T>* Current) override {

        if (nodeToAdd == NULL) return NULL;

        if (Tree<T>::root == NULL) {
            Tree<T>::root = nodeToAdd;
            return nodeToAdd;
        }

        if (Current->getData() == nodeToAdd->getData()) {
            throw AlreadyUsed();
        }

        (Current->getData() > nodeToAdd->getData()) ? goToTheLeft(nodeToAdd, Current) : goToTheRight(nodeToAdd, Current);

        balance(Current);

        return Current;
    }

    void goToTheLeft(Node<T> *nodeToAdd, Node<T> *Current) {
        if (Current->getLeft() != NULL) {
            Current->setLeft(Add_R(nodeToAdd, Current->getLeft()));
        } else {
            Current->setLeft(nodeToAdd);
        }
        Current->getLeft()->setParent(Current);
    }

    void goToTheRight(Node<T>* nodeToAdd, Node<T>* Current) {
        if (Current->getRight() != NULL)
            Current->setRight(Add_R(nodeToAdd, Current->getRight()));
        else {
            Current->setRight(nodeToAdd);
        }
        Current->getRight()->setParent(Current);
    }

    Node<T>* balance(Node<T>* nodeToCheck) {
        fixHeight(nodeToCheck);

        int h = heightDifference(nodeToCheck);

        if (h > 1) {
            if (heightDifference(nodeToCheck->getLeft()) < 0)
                nodeToCheck->setLeft(turnLeft(nodeToCheck->getLeft()));

            return turnRight(nodeToCheck);
        }
        else if (h < -1) {

            if (heightDifference(nodeToCheck->getRight()) > 0)
                nodeToCheck->setRight(turnRight(nodeToCheck->getRight()));

            return turnLeft(nodeToCheck);
        }

        return nodeToCheck;
    }

    void fixHeight(Node<T>* n) {
        int leftHeight = height(n->getLeft());
        int rightHeight = height(n->getRight());

        n->setHeight(max(leftHeight, rightHeight) + 1);
    }

    int heightDifference(Node<T>* node) { return height(node->getLeft()) - height(node->getRight()); }
    int height(Node<T>* node) { return (node)? node->getHeight() : 0; }
    void changeRoot(Node<T>* newRoot) { Tree<T>:: root = newRoot; }

    Node<T>* turnRight(Node<T>* nodeRotateAround) {
        Node<T>* newNode = nodeRotateAround->getLeft();
        nodeRotateAround->setLeft(newNode->getRight());
        newNode->setRight(nodeRotateAround);

        if (Tree<T>::root == nodeRotateAround) { changeRoot( newNode); }

        disconnectNodes(nodeRotateAround);
        connectNodes(newNode, nodeRotateAround);

        return newNode;
    }

    Node<T>* turnLeft(Node<T>* nodeRotateAround) {
        Node<T>* newNode = nodeRotateAround->getRight();
        nodeRotateAround->setRight(newNode->getLeft());
        newNode->setLeft(nodeRotateAround);

        newNode->setParent(nodeRotateAround->getParent());
        nodeRotateAround->setParent(newNode);

        if (Tree<T>::root == nodeRotateAround) { changeRoot( newNode);}

        disconnectNodes(nodeRotateAround);
        connectNodes(newNode, nodeRotateAround);

        return newNode;
    }

    void disconnectNodes(Node<T>* node) {
        Node<T>* parent = node->getParent();
        Node<T>* rightChild = node->getRight();
        Node<T>* leftChild = node->getLeft();

        if (parent != NULL) {
            if (node == parent->getLeft()) {
                parent->setLeft(NULL);
            }

            if (node == parent->getRight()) {
                parent->setRight(NULL);
            }
        }

        if (rightChild != NULL) {
            rightChild->setParent(NULL);
            node->setRight(NULL);
        }

        if (leftChild != NULL) {
            leftChild->setParent(NULL);
        }
    }

    void connectNodes(Node<T>* parent, Node<T>* child) {
        if (parent != NULL) {
            if (child->getData() > parent->data) {
                parent->setRight(child);
            }

            if (child->getData() < parent->data) {
                parent->setLeft(child);
            }

        }

        if (child != NULL) {
            child->setParent(parent);
        }
    }
};


template <class T>
class treeIterator : iterator<input_iterator_tag, T> {
private:
    Node<T>* ptr;
public:
    treeIterator () { ptr = NULL; }

    treeIterator(Node<T>* node) { ptr = node; }

    treeIterator& operator ++() {
    }

    treeIterator& operator --() {

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

    BalancedTree<double> T;
    T.Add(2.5);
    T.Add(2.6);
    T.Add(2.7);
    T.Add(2.2);
    cout << "Root: " << *T.getRoot() << endl;
    //cout << "Min: " << *T.Min(T.getRoot()) << endl;
    //cout << "Max: " << *T.Max(T.getRoot());
    return 0;
}
