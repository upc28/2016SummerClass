#ifndef MINHEAP_H
#define MINHEAP_H

#include <QVector>
#include <iostream>
#include <QDebug>
using namespace std;

struct Node{
    QChar value;
    qint32 weight;
    qint32 loc;
    Node* left;Node* right;
    Node(){
        left = NULL;right=NULL;
    }
    Node(QChar a,qint32 b){
        value = a;weight =b;
        left = NULL;right=NULL;
    }
    Node(qint32 b){
        weight =b;
        left = NULL;right=NULL;
    }
};

class MinHeap
{
public:
    MinHeap(int length);
   // ~MinHeap();
    void insert(Node* node);
    Node* reMini();
    bool isEmpty();
    void display();
    qint32 length();
    Node* at(qint32 i);
private:
    Node** heap;
    qint32 num;
    int maxLength;
    void siftDown(int start,int end);
    void createHeap();
    void siftUp(int start);
};

#endif // MINHEAP_H
