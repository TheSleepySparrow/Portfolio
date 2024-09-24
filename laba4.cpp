/* Бакалавров с факультета математики, ухаживающих за бинарным деревом,
не устраивает существующий журнал, поскольку они хотят знать заранее,
кому в какой день приходить. Для этого им нужна программа,
позволяющая вводить соответствия дат и студентов,
а также составляющая список студентов в порядке возрастания дат дежурств.
Коллекция: бинарное дерево<дата, студент> с методом, возвращающим
список имен студентов в порядке возрастания ключа дат. */
#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

template <typename Key, typename Value>
class StudentTree
{
public:
    StudentTree(const Key &data, const Value &student)
    {
        key = data;
        value = student;
        right = left = nullptr;
    }
    StudentTree<Key, Value> *getRightChild() const
    {
        return right;
    }
    StudentTree<Key, Value> *getLeftChild() const
    {
        return left;
    }
    Key getKey() const
    {
        return key;
    }
    Value getValue() const
    {
        return value;
    }
    void changeLeftChild(StudentTree<Key, Value> *node)
    {
        left = node;
    }
    void changeRightChild(StudentTree<Key, Value> *node)
    {
        right = node;
    }
    void changeValue(const Value &anotherValue)
    {
        value = anotherValue;
    }
    void changeKey(const Key &anotherKey)
    {
        key = anotherKey;
    }

private:
    Key key;
    Value value;
    StudentTree<Key, Value> *right;
    StudentTree<Key, Value> *left;
};

template <typename Key, typename Value>
class BinaryTree
{
public:
    // Конструктор по умолчанию
    BinaryTree()
    {
        root = nullptr;
    }

    // конструктор копирования
    BinaryTree(const BinaryTree &other) : root(nullptr)
    {
        root = copyTree(other.getRoot());
    }
    // рекурсивная функция для копирования
    StudentTree<Key, Value> *copyTree(const StudentTree<Key, Value> *node) const
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        StudentTree<Key, Value> *newNode = new StudentTree<Key, Value>(node->getKey(), node->getValue());
        newNode->changeLeftChild(copyTree(node->getLeftChild()));
        newNode->changeRightChild(copyTree(node->getRightChild()));

        return newNode;
    }

    // Деструктор
    ~BinaryTree()
    {
        destroyTree(root);
    }
    // Функция для удаления всех хранящичся значений
    void deleteAll()
    {
        destroyTree(root);
        root = nullptr;
    }
    // Функция для удаления всех хранящихся значений для узла дерева
    void destroyTree(StudentTree<Key, Value> *node)
    {
        if (node != nullptr)
        {
            destroyTree(node->getLeftChild());
            destroyTree(node->getRightChild());
            delete node;
        }
    }

    // функция возвращающая указатель на начало дерева
    const StudentTree<Key, Value> *getRoot() const
    {
        return root;
    }

    // Метод для сохранения содержимого коллекции в файл
    void saveToFile(const string &filename) const
    {
        std::ofstream file(filename);
        saveTreeToFile(root, file);
        file.close();
    }
    // Рекурсивная функция для сохранения дерева в файл
    void saveTreeToFile(StudentTree<Key, Value> *node, ofstream &file) const
    {
        if (node != nullptr)
        {
            saveTreeToFile(node->getLeftChild(), file);
            file << node->getKey() << "\n"
                 << node->getValue() << endl;
            saveTreeToFile(node->getRightChild(), file);
        }
    }

    // Метод для загрузки содержимого коллекции из файла
    void loadFromFile(const string &filename)
    {
        ifstream file;
        file.open(filename);
        Key key;
        Value value;
        string line1, line2;
        if (!file)
        {
            cout << "File is not opened" << endl;
        }
        else
        {
            destroyTree(root);
            while (!file.eof())
            {
                getline(file, line1);
                getline(file, line2);
                if (line1.empty() || line2.empty())
                {
                    break;
                }
                stringstream linestream1(line1), linestream2(line2);
                linestream1 >> key;
                linestream2 >> value;
                add(key, value);
            }
        }
        file.close();
    }

    // Метод для добавления пары <дата, студент> в дерево
    void add(const Key &key, const Value &value)
    {
        root = insert(key, value, root);
    }
    // Функция вставки данных с определенного узла
    StudentTree<Key, Value> *insert(const Key &key, const Value &value, StudentTree<Key, Value> *node)
    {
        if (node == nullptr)
        {
            StudentTree<Key, Value> *tmp = new StudentTree<Key, Value>(key, value);
            return tmp;
        }

        if (key < node->getKey())
        {
            node->changeLeftChild(insert(key, value, node->getLeftChild()));
        }
        if (key > node->getKey())
        {
            node->changeRightChild(insert(key, value, node->getRightChild()));
        }

        return node;
    }

    // Метод для удаления заданного значения из коллекции
    void remove(const Value &value)
    {
        root = removeNode(root, value);
    }
    // Рекурсивная функция для удаления узла с заданным значением из дерева
    StudentTree<Key, Value> *removeNode(StudentTree<Key, Value> *node, const Value &value)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (value < node->getValue())
        {
            node->changeLeftChild(removeNode(node->getLeftChild(), value));
        }
        else if (value > node->getValue())
        {
            node->changeRightChild(removeNode(node->getRightChild(), value));
        }
        else
        {
            // Найден узел для удаления
            if (node->getLeftChild() == nullptr && node->getRightChild() == nullptr)
            {
                delete node;
                return nullptr;
            }
            else if (node->getLeftChild() == nullptr)
            {
                StudentTree<Key, Value> *temp = node->getRightChild();
                delete node;
                return temp;
            }
            else if (node->getRightChild() == nullptr)
            {
                StudentTree<Key, Value> *temp = node->getLeftChild();
                delete node;
                return temp;
            }
            else
            {
                // Узел имеет двух потомков
                StudentTree<Key, Value> *successor = findMin(node->getRightChild());
                node->changeKey(successor->getKey());
                node->changeValue(successor->getValue());
                node->changeRightChild(removeNode(node->getRightChild(), successor->getValue()));
            }
        }

        return node;
    }
    // Рекурсивная функция для нахождения минимального узла в дереве
    StudentTree<Key, Value> *findMin(StudentTree<Key, Value> *node) const
    {
        while (node->getLeftChild() != nullptr)
        {
            node = node->getLeftChild();
        }
        return node;
    }

    // Метод, принимающий ключ и возвращающий при его наличии в коллекции истину, а при отсутствии – ложь
    bool containsKey(const Key &key) const
    {
        return findNode(root, key) != nullptr;
    }
    // Рекурсивная функция для поиска узла с заданным ключом
    StudentTree<Key, Value> *findNode(StudentTree<Key, Value> *node, const Key &key) const
    {
        if (node == nullptr || node->getKey() == key)
        {
            return node;
        }

        if (key < node->getKey())
        {
            return findNode(node->getLeftChild(), key);
        }
        else
        {
            return findNode(node->getRightChild(), key);
        }
    }

    // Метод для возвращения количества значений в коллекции
    size_t size() const
    {
        return countNodes(root);
    }
    // Рекурсивная функция для подсчета количества узлов в дереве
    size_t countNodes(const StudentTree<Key, Value> *node) const
    {
        if (node == nullptr)
        {
            return 0;
        }

        return 1 + countNodes(node->getLeftChild()) + countNodes(node->getRightChild());
    }

    // Оператор [], принимающий ключ и возвращающий его значение
    Value operator[](const Key &key) const
    {
        StudentTree<Key, Value> *node = findNode(root, key);
        if (node != nullptr)
        {
            return node->getValue();
        }
        else
        {
            cout << "Key not found" << endl;
        }
    }

    // Оператор ==
    bool operator==(const BinaryTree &other) const
    {
        return compareTrees(root, other.root);
    }
    // Рекурсивная функция для сравнения двух деревьев
    bool compareTrees(const StudentTree<Key, Value> *node1, const StudentTree<Key, Value> *node2) const
    {
        if (node1 == nullptr && node2 == nullptr)
        {
            return true;
        }
        if (node1 == nullptr || node2 == nullptr)
        {
            return false;
        }
        if (node1->getKey() != node2->getKey() || node1->getValue() != node2->getValue())
        {
            return false;
        }

        return compareTrees(node1->getLeftChild(), node2->getLeftChild()) &&
               compareTrees(node1->getRightChild(), node2->getRightChild());
    }

    // Оператор &&, возвращающий все общие значения T в виде новой коллекции
    BinaryTree<Key, Value> operator&&(const BinaryTree<Key, Value> &other) const
    {
        BinaryTree<Key, Value> result;
        findCommonValues(root, other.getRoot(), result);
        return result;
    }
    // Новая функция для нахождения общих значений между двумя деревьями
    void findCommonValues(const StudentTree<Key, Value> *node1, const StudentTree<Key, Value> *node2, BinaryTree<Key, Value> &result) const
    {
        if (node1 != nullptr && node2 != nullptr)
        {
            findCommonValues(node1->getLeftChild(), node2->getLeftChild(), result);
            if (this->containsKey(node2->getKey()) && this->operator[](node2->getKey()) == node2->getValue())
            {
                result.add(node2->getKey(), node2->getValue());
                // cout << node2->getKey() << " " << node2->getValue();
            }

            findCommonValues(node1->getRightChild(), node2->getRightChild(), result);
        }
    }

    // Метод для возвращения всех значений в порядке возрастания даты
    vector<Value> getSortedValues()
    {
        vector<Value> result;
        inOrderTraversal(root, result);
        return result;
    }
    void inOrderTraversal(StudentTree<Key, Value> *node, vector<Value> &result)
    {
        if (node != nullptr)
        {
            // Рекурсивно обходим левое поддерево
            inOrderTraversal(node->getLeftChild(), result);

            // Добавляем значение текущего узла
            result.push_back(node->getValue());

            // Рекурсивно обходим правое поддерево
            inOrderTraversal(node->getRightChild(), result);
        }
    }

private:
    StudentTree<Key, Value> *root;
};

int main()
{
    BinaryTree<int, string> root1, root2, root3, root4;
    assert(root1.getRoot() == nullptr);
    cout << "Test #1 passed: default constractor (1)\n";
    assert(root1.size() == 0);
    cout << "Test #2 passed: default constractor (2)\n";
    assert((root1 == root2) == true);
    cout << "Test #3 passed: default constractor (3)\n";

    root1.add(20200130, "Милованова_Анастасия");
    assert(root1.size() == 1);
    assert(root1[20200130] == "Милованова_Анастасия");
    cout << "Test #4 passed: add element (1)\n";

    root1.add(20200130, "Сидоров_Иван");
    assert(root1.size() == 1);
    assert(root1[20200130] == "Милованова_Анастасия");
    cout << "Test #4 passed: add element (2)\n";

    root2.loadFromFile("input.txt");
    assert(root2.size() == 2);
    assert(root2[20210303] == "Попов_Арсений");
    assert(root2[20210304] == "Иванов_Петр");
    cout << "Test #5 passed: read file\n";
    root2.saveToFile("output.txt");
    root3.loadFromFile("output.txt");
    assert((root2 == root3) == true);
    root2.add(20210306, "Милованова_Анастасия");
    assert((root2 == root3) == false);
    cout << "Test #6 passed: read and write file\n";

    assert(root2.containsKey(20210305) == false);
    assert(root2.containsKey(20210303) == true);
    cout << "Test #7 passed: method containKeys()\n";

    root2.remove("Попов_Арсений");
    assert(root2.containsKey(20210303) == false);
    assert(root2.containsKey(20210304) == true);
    assert(root2.containsKey(20210306) == true);
    assert(root2.size() == 2);
    cout << "Test #8 passed: method remove() (1)\n";

    root2.remove("Попов_Арсений");
    assert(root2.containsKey(20210303) == false);
    assert(root2.containsKey(20210304) == true);
    assert(root2.containsKey(20210306) == true);
    assert(root2.size() == 2);
    cout << "Test #8 passed: method remove() (2)\n";

    root3.deleteAll();
    assert((root3 == root4) == true);
    cout << "Test #9 passed: method deleteAll()\n";

    BinaryTree<int, string> root5(root2);
    assert((root5 == root2) == true);
    cout << "Test #10 passed: copying constractor (1)\n";
    root5.add(20210401, "Милованова_Анастасия");
    assert((root5 == root2) == false);
    cout << "Test #11 passed: copying constractor (2)\n";

    BinaryTree<int, string> commonValues1 = root5 && root2;
    assert(commonValues1.size() == 2);
    assert(commonValues1.containsKey(20210304) == true);
    assert(commonValues1.containsKey(20210306) == true);
    // commonValues1.saveToFile("output.txt");
    cout << "Test #12 passed: oparator && (1)\n";

    BinaryTree<int, string> commonValues2 = root5 && root4;
    assert((commonValues2 == root4) == true);
    cout << "Test #13 passed: oparator && (2)\n";

    BinaryTree<int, string> root6(root5);
    BinaryTree<int, string> commonValues3 = root6 && root5;
    // commonValues3.saveToFile("output.txt");
    assert((commonValues3 == root6) == true);
    cout << "Test #14 passed: oparator && (3)\n";

    root5.saveToFile("output.txt");
    vector<string> order = root5.getSortedValues();
    /* for (int i = 0; i < order.size(); i++)
    {
        cout << order[i] << "\n";
    } */
    assert(order.size() == 3);
    assert(order[0] == "Иванов_Петр");
    assert(order[1] == "Милованова_Анастасия");
    assert(order[2] == "Милованова_Анастасия");
    cout << "Test #15 passed: get queue\n";

    return 0;
}