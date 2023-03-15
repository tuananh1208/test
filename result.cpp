#include "main.h"
ofstream outfile;
template <class T>
class DLinkedList
{
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        Node(const T &data = T{}, Node *next = nullptr, Node *prev = nullptr) : data(data), next(next), prev(prev) {}
    };

public:
    Node *head;
    Node *tail;
    int count;

    DLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~DLinkedList()
    {
        this->clear();
    }

    void add(const T &e)
    {
        add(size(), e);
    }

    void add(int index, const T &e)
    {
        if (index < 0 || index > size())
        {
            throw std::out_of_range("Index out of bounds");
        }

        Node *newNode = new Node(e, nullptr, nullptr);

        if (size() == 0)
        {
            head = tail = newNode;
        }
        else if (index == 0)
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else if (index == size())
        {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        else
        {
            Node *current = head;
            for (int i = 0; i < index; i++)
            {
                current = current->next;
            }

            newNode->prev = current->prev;
            newNode->next = current;
            current->prev->next = newNode;
            current->prev = newNode;
        }

        count++;
    }

    T removeAt(int index)
    {
        if (index < 0 || index >= size())
        {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == this->size() - 1)
        {
            if (index == 0)
            {
                T data = head->data;
                delete head;
                head = tail = nullptr;
                count--;
                return data;
            }
            T data = tail->data;
            Node *curr = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete curr;
            count--;
            return data;
        }
        Node *current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }

        T data = current->data;

        if (size() == 1)
        {
            head = tail = nullptr;
        }
        else if (current == head)
        {
            head = current->next;
            head->prev = nullptr;
        }
        else if (current == tail)
        {
            tail = current->prev;
            tail->next = nullptr;
        }
        else
        {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        delete current;
        count--;

        return data;
    }

    bool removeItem(const T &removeItem)
    {
        Node *current = head;
        while (current != nullptr)
        {
            if (current->data == removeItem)
            {
                if (size() == 1)
                {
                    head = tail = nullptr;
                }
                else if (current == head)
                {
                    head = current->next;
                    head->prev = nullptr;
                }
                else if (current == tail)
                {
                    tail = current->prev;
                    tail->next = nullptr;
                }
                else
                {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                delete current;
                count--;
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool empty() const
    {
        return (count == 0);
    }

    int size() const
    {
        return count;
    }
    void clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }
    T get(int index)
    {
        if (index < 0 || index >= size())
        {
            throw std::out_of_range("Index out of bounds");
        }
        Node *current = head;
        if (index == 0)
        {
            return head->data;
        }
        if (index == this->size() - 1)
        {
            return tail->data;
        }
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->data;
    }
    void set(int index, const T &e)
    {
        if (index < 0 || index >= size())
        {
            throw std::out_of_range("Index out of bounds");
        }
        Node *current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        current->data = e;
    }
    int indexOf(const T &item)
    {
        Node *current = head;
        int index = 0;
        while (current != nullptr)
        {
            if (current->data == item)
            {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }
    bool contains(const T &item)
    {
        return indexOf(item) != -1;
    }
};

// Khach khong co ID thi cho id = 0
void reg(int id, string name, int age, restaurant *&r, DLinkedList<pair<string, int>> *&st, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&stable_q, int &singleTable, table *&mergeTableHead, table *&recentAddedTable, int &customer_quantity)
{
    if (id == 0)
    {
        if (singleTable == MAXSIZE)
        {
            table *temp = r->recentTable;
            while (temp->ID != 1)
            {
                temp = temp->next;
            }
            int count = 1;
            while (true)
            {
                if (count > singleTable)
                {
                    if (q->size() < MAXSIZE)
                    {
                        q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, 0)));
                        stable_q->add(make_pair(name, age));
                        st->add(make_pair(name, age));
                    }
                    break;
                }
                else if (temp->age == 0)
                {
                    temp->age = age;
                    temp->name = name;
                    st->add(make_pair(name, age));
                    recentAddedTable = temp;
                    break;
                }
                else
                {
                    temp = temp->next;
                    count += 1;
                }
            }
        }
        else
        {
            table *temp = mergeTableHead;
            bool flag = false;
            while (temp != nullptr)
            {
                if (temp->ID == id)
                {
                    flag = true;
                    break;
                }
                temp = temp->next;
            }
            if (flag)
            {
                temp = r->recentTable;
            }
            else
            {
                table *curr = r->recentTable;
                int MIN = MAXSIZE + 1;
                int count = 1;
                while (true)
                {
                    if (count > singleTable)
                    {
                        break;
                    }
                    if (curr->ID < MIN)
                    {
                        MIN = curr->ID;
                        temp = curr;
                    }
                    count++;
                    curr = curr->next;
                }
            }
            int count = 1;
            while (true)
            {
                if (count > singleTable)
                {
                    if (q->size() < MAXSIZE)
                    {
                        q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, 0)));
                        stable_q->add(make_pair(name, age));
                        st->add(make_pair(name, age));
                    }
                    break;
                }
                else if (temp->age == 0)
                {
                    temp->age = age;
                    temp->name = name;
                    st->add(make_pair(name, age));
                    recentAddedTable = temp;
                    break;
                }
                else
                {
                    temp = temp->next;
                    count += 1;
                }
            }
        }
    }
    else
    {
        if (singleTable == MAXSIZE)
        {
            table *temp = r->recentTable->next;
            while (true)
            {
                if (temp->ID == id)
                {
                    break;
                }
                temp = temp->next;
            }
            int count = 1;
            while (true)
            {
                if (count > singleTable)
                {
                    if (q->size() < MAXSIZE)
                    {
                        q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, id)));
                        stable_q->add(make_pair(name, age));
                        st->add(make_pair(name, age));
                    }
                    break;
                }
                else if (temp->age == 0)
                {
                    temp->age = age;
                    temp->name = name;
                    st->add(make_pair(name, age));
                    recentAddedTable = temp;
                    break;
                }
                else
                {
                    temp = temp->next;
                    count++;
                }
            }
        }
        else
        {
            table *temp = mergeTableHead;
            bool flag = false;
            while (temp != nullptr)
            {
                if (temp->ID == id)
                {
                    flag = true;
                    break;
                }
                temp = temp->next;
            }
            if (flag)
            {
                temp = r->recentTable;
            }
            else
            {
                table *curr = r->recentTable;
                int count = 1;
                while (true)
                {
                    if (count > singleTable)
                    {
                        break;
                    }
                    if (curr->ID == id)
                    {
                        temp = curr;
                        break;
                    }
                    count++;
                    curr = curr->next;
                }
            }
            int count = 1;
            while (true)
            {
                if (count > singleTable)
                {
                    if (q->size() < MAXSIZE)
                    {
                        q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, id)));
                        stable_q->add(make_pair(name, age));
                        st->add(make_pair(name, age));
                    }
                    break;
                }
                else if (temp->age == 0)
                {
                    temp->age = age;
                    temp->name = name;
                    st->add(make_pair(name, age));
                    recentAddedTable = temp;
                    break;
                }
                else
                {
                    temp = temp->next;
                    count += 1;
                }
            }
        }
    }
}

void reg_from_queue(int id, string name, int age, restaurant *&r, DLinkedList<pair<string, int>> *&st, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&stable_q, int &singleTable, table *&mergeTableHead, table *&recentAddedTable, int &customer_quantity)
{
    if (id == 0)
    {
        table *temp = r->recentTable;
        while (temp->ID != 1)
        {
            temp = temp->next;
        }
        int count = 1;
        while (true)
        {
            if (count > singleTable)
            {
                if (q->size() < MAXSIZE)
                {
                    q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, 0)));
                    stable_q->add(make_pair(name, age));
                }
                break;
            }
            else if (temp->age == 0)
            {
                temp->age = age;
                temp->name = name;
                recentAddedTable = temp;
                break;
            }
            else
            {
                temp = temp->next;
                count += 1;
            }
        }
    }
    else
    {

        table *temp = r->recentTable->next;
        while (true)
        {
            if (temp->ID == id)
            {
                break;
            }
            temp = temp->next;
        }
        int count = 1;
        while (true)
        {
            if (count > singleTable)
            {
                if (q->size() < MAXSIZE)
                {
                    q->add(make_pair(make_pair(customer_quantity++, name), make_pair(age, id)));
                    stable_q->add(make_pair(name, age));
                }
                break;
            }
            else if (temp->age == 0)
            {
                temp->age = age;
                temp->name = name;
                recentAddedTable = temp;
                break;
            }
            else
            {
                temp = temp->next;
                count++;
            }
        }
    }
}

void regm(string name, int age, int num, restaurant *&r, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&st, int &singleTable, table *&mergeTableHead, table *&mergeTableTail, table *&recentAddedTable, int &customer_quantity)
{
    if (singleTable < MAXSIZE)
    {
        return;
    }
    if (num > singleTable)
    {
        return;
    }
    table *temp = r->recentTable;
    while (temp->ID != 1)
    {
        temp = temp->next;
    }
    DLinkedList<int> *dp = new DLinkedList<int>();
    dp->add(0);
    for (int i = 1; i <= MAXSIZE + num - 1; i++)
    {
        if (temp->age == 0)
        {
            dp->add(1 + dp->tail->data);
        }
        else
        {
            dp->add(0);
        }
        temp = temp->next;
    }
    while (temp->ID != 1)
    {
        temp = temp->next;
    }
    bool valid = false;
    int MAX = -1;
    DLinkedList<int>::Node *curr = dp->head->next;
    for (int i = 1; i <= MAXSIZE + num - 1; i++)
    {
        if (curr->data >= num)
        {
            valid = true;
            if (MAX < i)
            {
                MAX = i;
            }
        }
        temp = temp->next;
        curr = curr->next;
    }
    table *check = temp;
    while (valid && check->ID != MAX - num + 1)
    {
        check = check->next;
    }
    if (valid)
    {
        singleTable = MAXSIZE - num;
        table *temp = check;
        while (mergeTableHead == nullptr || mergeTableTail == nullptr)
        {
            if (temp == check)
            {
                mergeTableHead = temp;
            }
            if (temp->ID == (check->ID + num - 1) % MAXSIZE || temp->ID == (check->ID + num - 1))
            {
                mergeTableTail = temp;
            }
            temp = temp->next;
        }
        mergeTableHead->age = age;
        mergeTableHead->name = name;
        recentAddedTable = mergeTableHead;
        st->add(make_pair(mergeTableHead->name, mergeTableHead->age));
        table *pre = nullptr;
        table *past = nullptr;
        while (pre == nullptr || past == nullptr)
        {
            if (temp->next == mergeTableHead)
            {
                pre = temp;
            }
            if (temp == mergeTableTail->next)
            {
                past = temp;
            }
            temp = temp->next;
        }
        pre->next = past;
        r->recentTable = past;
        mergeTableTail->next = nullptr;
    }
    delete dp;
}

void cle(int id, restaurant *&r, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&st, DLinkedList<pair<string, int>> *&stable_q, int &singleTable, table *&mergeTableHead, table *&mergeTableTail, table *&recentAddedTable, int &customer_quantity)
{
    table *temp = r->recentTable;
    int count = 1;
    while (count <= singleTable)
    {
        if (temp->ID == id)
        {
            break;
        }
        temp = temp->next;
        count++;
    }
    if (count <= singleTable)
    {
        st->removeItem(make_pair(temp->name, temp->age));
        if (temp->age != 0)
        {
            temp->age = 0;
            temp->name = "";
            recentAddedTable = temp;
        }
        if (q->size() > 0)
        {
            temp->age = q->head->data.second.first;
            temp->name = q->head->data.first.second;
            recentAddedTable = temp;
            stable_q->removeItem(make_pair(temp->name, temp->age));
            q->removeAt(0);
        }
    }
    else
    {
        if (id != mergeTableHead->ID)
        {
            return;
        }
        if (singleTable == 0)
        {
            mergeTableTail->next = mergeTableHead;
        }
        else
        {
            table *temp = r->recentTable;
            while (temp->next != r->recentTable)
            {
                temp = temp->next;
            }
            temp->next = mergeTableHead;
            mergeTableTail->next = r->recentTable;
        }
        st->removeItem(make_pair(mergeTableHead->name, mergeTableHead->age));
        mergeTableHead->age = 0;
        mergeTableHead->name = "";
        recentAddedTable = mergeTableHead;
        singleTable = MAXSIZE;
        int available = 0;
        temp = r->recentTable->next;
        while (temp != r->recentTable)
        {
            if (temp->age == 0)
            {
                available++;
            }
            temp = temp->next;
        }
        if (temp->age == 0)
        {
            available++;
        }
        mergeTableHead = mergeTableTail = nullptr;
        while (available--)
        {
            if (q->empty())
            {
                break;
            }
            else
            {
                DLinkedList<pair<pair<int, string>, pair<int, int>>>::Node *curr = q->head;
                reg_from_queue(curr->data.second.second, curr->data.first.second, curr->data.second.first, r, st, q, stable_q, singleTable, mergeTableHead, recentAddedTable, customer_quantity);
                stable_q->removeItem(make_pair(curr->data.first.second, curr->data.second.first));
                q->removeAt(0);
            }
        }
    }
}

void ps(int num, DLinkedList<pair<string, int>> *&st)
{
    if (st->count == 0)
    {
        outfile << "Empty" << endl;
        return;
    }
    DLinkedList<pair<string, int>>::Node *temp = st->tail;
    int i;
    if (num <= st->size())
    {
        i = num;
    }
    else
    {
        i = st->size();
    }
    while (i--)
    {
        outfile << temp->data.first << endl;
        temp = temp->prev;
    }
}

void pq(int num, DLinkedList<pair<string, int>> *&stable_q)
{
    if (stable_q->count == 0)
    {
        outfile << "Empty" << endl;
        return;
    }
    DLinkedList<pair<string, int>>::Node *temp = stable_q->head;
    int i;
    if (num <= stable_q->size())
    {
        i = num;
    }
    else
    {
        i = stable_q->size();
    }
    while (i--)
    {
        outfile << temp->data.first << endl;
        temp = temp->next;
    }
}

void sq(int num, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q)
{
    if (q->count == 0)
    {
        outfile << "Empty" << endl;
        return;
    }
    DLinkedList<pair<pair<int, string>, pair<int, int>>>::Node *curr = q->head;
    if (num > q->size())
    {
        num = q->size();
    }
    int current = 0;
    while (current <= num - 1)
    {
        DLinkedList<pair<pair<int, string>, pair<int, int>>>::Node *largest = curr;
        DLinkedList<pair<pair<int, string>, pair<int, int>>>::Node *walker = curr->next;
        while (walker != nullptr)
        {
            if (walker->data.second.first > largest->data.second.first || (walker->data.second.first == largest->data.second.first && walker->data.first.first < largest->data.first.first))
            {
                largest = walker;
            }
            walker = walker->next;
        }
        pair<pair<int, string>, pair<int, int>> temp = largest->data;
        largest->data = curr->data;
        curr->data = temp;
        curr = curr->next;
        current += 1;
    }
    curr = q->head;
    while (curr != nullptr)
    {
        outfile << curr->data.first.second << endl;
        curr = curr->next;
    }
}
void pt(restaurant *&r, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&st, int &singleTable, table *&mergeTableHead, table *&mergeTableTail, table *&recentAddedTable)
{
    if (recentAddedTable == nullptr)
    {
        return;
    }
    if (singleTable == MAXSIZE)
    {
        table *temp = recentAddedTable;
        while (temp->next != recentAddedTable)
        {
            if (temp->age != 0)
            {
                outfile << temp->ID << "-" << temp->name << endl;
            }
            temp = temp->next;
        }
        if (temp->age != 0)
        {
            outfile << temp->ID << "-" << temp->name << endl;
        }
    }
    else
    {
        table *temp = recentAddedTable;
        if (singleTable == 0)
        {
            outfile << mergeTableHead->ID << "-" << mergeTableHead->name << endl;
            return;
        }
        if (temp == mergeTableHead)
        {
            outfile << temp->ID << "-" << temp->name << endl;
            temp = r->recentTable;
            while (temp->next != r->recentTable)
            {
                if (temp->age != 0)
                {
                    outfile << temp->ID << "-" << temp->name << endl;
                }
                temp = temp->next;
            }
            if (temp->age != 0)
            {
                outfile << temp->ID << "-" << temp->name << endl;
            }
        }
        else
        {
            while (temp->next != recentAddedTable)
            {
                if (temp->next == r->recentTable)
                {
                    if (temp->age != 0)
                    {
                        outfile << temp->ID << "-" << temp->name << endl;
                    }
                    outfile << mergeTableHead->ID << "-" << mergeTableHead->name << endl;
                }
                else
                {
                    if (temp->age != 0)
                    {
                        outfile << temp->ID << "-" << temp->name << endl;
                    }
                }
                temp = temp->next;
            }
            if (temp->next == r->recentTable)
            {
                if (temp->age != 0)
                {
                    outfile << temp->ID << "-" << temp->name << endl;
                }
                outfile << mergeTableHead->ID << "-" << mergeTableHead->name << endl;
            }
            else
            {
                if (temp->age != 0)
                {
                    outfile << temp->ID << "-" << temp->name << endl;
                }
            }
        }
    }
}

int convertNumber(string s)
{
    for (char c : s)
    {
        if ('0' > c || c > '9')
        {
            return -1;
        }
    }
    return stoi(s);
}

void Command(DLinkedList<string> *&command, restaurant *&r, DLinkedList<pair<string, int>> *&st, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&stable_q, int &singleTable, table *&mergeTableHead, table *&mergeTableTail, table *&recentAddedTable, int &custumer_quantity)
{
    DLinkedList<string>::Node *temp = command->head;
    if (temp->data == "REG")
    {
        if (command->size() <= 2 || command->size() >= 5)
        {
            return;
        }
        else if (command->size() == 3)
        {
            string s1 = temp->next->data;
            string s2 = temp->next->next->data;
            if (convertNumber(s2) == -1)
            {
                return;
            }
            if (stoi(s2) > 115 || stoi(s2) < 16)
            {
                return;
            }
            reg(0, s1, stoi(s2), r, st, q, stable_q, singleTable, mergeTableHead, recentAddedTable, custumer_quantity);
        }
        else
        {
            string s1 = temp->next->data;
            string s2 = temp->next->next->data;
            string s3 = temp->next->next->next->data;
            if (convertNumber(s1) == -1 || convertNumber(s3) == -1)
            {
                return;
            }
            if (stoi(s3) > 115 || stoi(s3) < 16)
            {
                return;
            }
            if (stoi(s1) < 1 || stoi(s1) > MAXSIZE)
            {
                return;
            }
            reg(stoi(s1), s2, stoi(s3), r, st, q, stable_q, singleTable, mergeTableHead, recentAddedTable, custumer_quantity);
        }
    }
    else if (temp->data == "REGM")
    {
        if (command->size() != 4)
        {
            return;
        }
        else
        {
            string s1 = temp->next->data;
            string s2 = temp->next->next->data;
            string s3 = temp->next->next->next->data;
            if (convertNumber(s2) == -1 || convertNumber(s3) == -1)
            {
                return;
            }
            if (stoi(s2) > 115 || stoi(s2) < 16)
            {
                return;
            }
            if (stoi(s3) < 1 || stoi(s3) > MAXSIZE)
            {
                return;
            }
            regm(s1, stoi(s2), stoi(s3), r, q, st, singleTable, mergeTableHead, mergeTableTail, recentAddedTable, custumer_quantity);
        }
    }
    else if (temp->data == "PT")
    {
        if (command->size() != 1)
        {
            return;
        }
        pt(r, q, st, singleTable, mergeTableHead, mergeTableTail, recentAddedTable);
    }
    else if (temp->data == "CLE")
    {
        if (command->size() != 2)
        {
            return;
        }
        else
        {
            string s1 = temp->next->data;
            if (stoi(s1) == -1)
            {
                return;
            }
            if (stoi(s1) > MAXSIZE || stoi(s1) < 1)
            {
                return;
            }
            cle(stoi(s1), r, q, st, stable_q, singleTable, mergeTableHead, mergeTableTail, recentAddedTable, custumer_quantity);
        }
    }
    else if (temp->data == "PS")
    {
        if (command->size() > 2 || command->size() < 1)
        {
            return;
        }
        else if (command->size() == 2)
        {
            string s1 = temp->next->data;
            if (stoi(s1) == -1)
            {
                return;
            }
            if (stoi(s1) > 2 * MAXSIZE || stoi(s1) < 1)
            {
                return;
            }
            ps(stoi(s1), st);
        }
        else
        {
            ps(st->size(), st);
        }
    }
    else if (temp->data == "PQ")
    {
        if (command->size() > 2 || command->size() < 1)
        {
            return;
        }
        else if (command->size() == 2)
        {
            string s1 = temp->next->data;
            if (stoi(s1) == -1)
            {
                return;
            }
            if (stoi(s1) > MAXSIZE || stoi(s1) < 1)
            {
                return;
            }
            pq(stoi(s1), stable_q);
        }
        else
        {
            pq(q->size(), stable_q);
        }
    }
    else if (temp->data == "SQ")
    {
        if (command->size() != 2)
        {
            return;
        }
        string s1 = temp->next->data;
        if (convertNumber(s1) == -1)
        {
            return;
        }
        sq(stoi(s1), q);
    }
    else
    {
        return;
    }
}

void ReadFile(string filename, restaurant *&r, DLinkedList<pair<string, int>> *&st, DLinkedList<pair<pair<int, string>, pair<int, int>>> *q, DLinkedList<pair<string, int>> *&stable_q, int &singleTable, table *&mergeTableHead, table *&mergeTableTail, table *&recentAddedTable, int &customer_quantity)
{
    ifstream myfile(filename);
    // myfile.open(filename);
    if (!myfile.is_open())
    {
        outfile << "Error: cannot open file " << filename << endl;
        return;
    }
    string line;
    while (getline(myfile, line))
    {
        DLinkedList<int> *space_pos = new DLinkedList<int>();
        DLinkedList<string> *command = new DLinkedList<string>();
        size_t pos = -1;
        while ((pos = line.find(' ', pos + 1)) != string::npos)
        {
            space_pos->add(pos);
        }
        DLinkedList<int>::Node *temp = space_pos->head;
        while (temp != nullptr)
        {
            temp = temp->next;
        }
        temp = space_pos->head;
        int prev_pos = 0;
        while (temp != nullptr)
        {
            command->add(line.substr(prev_pos, temp->data - prev_pos));
            prev_pos = temp->data + 1;
            temp = temp->next;
        }
        command->add(line.substr(prev_pos, line.size() - prev_pos));
        Command(command, r, st, q, stable_q, singleTable, mergeTableHead, mergeTableTail, recentAddedTable, customer_quantity);
        delete command;
        delete space_pos;
    }
    myfile.close();
}
void simulate(string filename, restaurant *r, int testcase)
{
    outfile.open("got" + to_string(testcase) + ".txt");
    DLinkedList<pair<string, int>> *st = new DLinkedList<pair<string, int>>();
    DLinkedList<pair<string, int>> *stable_q = new DLinkedList<pair<string, int>>();
    DLinkedList<pair<pair<int, string>, pair<int, int>>> *q = new DLinkedList<pair<pair<int, string>, pair<int, int>>>();
    int singleTable = MAXSIZE;
    int customer_quantity = 0;
    table *mergeTableHead = nullptr;
    table *mergeTableTail = nullptr;
    table *recentAddedTable = nullptr;
    ReadFile(filename, r, st, q, stable_q, singleTable, mergeTableHead, mergeTableTail, recentAddedTable, customer_quantity);
    table *temp = r->recentTable->next;
    r->recentTable->next = nullptr;
    while (temp != r->recentTable)
    {
        table *curr = temp;
        temp = temp->next;
        delete curr;
    }
    temp = mergeTableHead;
    while (temp != nullptr)
    {
        table *curr = temp;
        temp = temp->next;
        delete curr;
    }
    delete st;
    delete q;
    delete stable_q;
    outfile.close();
}