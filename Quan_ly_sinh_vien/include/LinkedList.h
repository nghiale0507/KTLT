#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <string>
using std::string;

template <class T>
struct Node {
    T data;
    Node<T>* next;
    Node(const T& value) {
        data = value;
        next = 0;
    }
};

template <class T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() { head = 0; }

    ~LinkedList() { clear(); }

    Node<T>* getHead() const { return head; }

    bool isEmpty() const { return head == 0; }

    int count() const {
        int n = 0;
        Node<T>* cur = head;
        while (cur != 0) {
            n++;
            cur = cur->next;
        }
        return n;
    }

    void addBack(const T& value) {
        Node<T>* node = new Node<T>(value);
        if (head == 0) {
            head = node;
            return;
        }
        Node<T>* cur = head;
        while (cur->next != 0) cur = cur->next;
        cur->next = node;
    }

    T* findById(const string& id) const {
        Node<T>* cur = head;
        while (cur != 0) {
            if (cur->data.id == id) return &(cur->data);
            cur = cur->next;
        }
        return 0;
    }

    bool removeById(const string& id) {
        if (head == 0) return false;
        if (head->data.id == id) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        Node<T>* prev = head;
        Node<T>* cur = head->next;
        while (cur != 0) {
            if (cur->data.id == id) {
                prev->next = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    void clear() {
        Node<T>* cur = head;
        while (cur != 0) {
            Node<T>* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
        head = 0;
    }

    void bubbleSort(int (*compare)(const T&, const T&)) {
        if (head == 0 || head->next == 0) return;
        bool swapped;
        do {
            swapped = false;
            Node<T>* cur = head;
            while (cur->next != 0) {
                if (compare(cur->data, cur->next->data) > 0) {
                    T tmp = cur->data;
                    cur->data = cur->next->data;
                    cur->next->data = tmp;
                    swapped = true;
                }
                cur = cur->next;
            }
        } while (swapped);
    }
};

#endif
