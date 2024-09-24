/* Студенты кафедры робототехники решили создать систему для передачи сообщений
между роботами, ползающими по зданию.Робот может связаться с несколькими роботами,
находящимися рядом, граф связей не хранится централизованно, а существует только
в электронных мозгах роботов.Известно, что роботы могут быть сделаны на очень
разнообразном, в том числе и устаревшем, железе, которое имеет ограничение
по скорости передачи сообщений(каждый робот знает сколько сообщений он может
передать в единицу времени).Роботы хранят числовые параметры в виде хеш - массива.
Анализ показал, что передаваемые сообщения делятся на те, что модифицируют
структуру графа(добавление и удаление ребер, информация об исчезновении узлов),
модифицируют информацию в узлах или простые сообщения, не меняющие состояние графа.
Напишите программу, моделирующую поведение такой сети. */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <cassert>
using namespace std;

enum RobotKind
{
    newRobot,
    oldRobot
};
template <typename T>
struct MessageInfo
{
    string message;
    time_t timestamp;
    T sender;

    MessageInfo(const string &msg, time_t time, T robot) : message(msg), timestamp(time), sender(robot) {}
};
class Robot
{
public:
    Robot()
    {
        unordered_map<string, int> numericParameters;
        vector<Robot *> neighbors;
    };
    Robot(const Robot &robot)
    {
        numericParameters = robot.numericParameters;
        for (Robot *n : robot.neighbors)
        {
            addNeighbor(n);
        }
    };
    virtual RobotKind getType() const = 0;
    virtual const int getTransmissionSpeed() const = 0;
    const unordered_map<string, int> &getNumericParameters() const
    {
        return numericParameters;
    };
    void setNumericParameter(const string &key, int value)
    {
        numericParameters[key] = value;
    }
    int getNumericParameter(const string &key) const
    {
        auto it = numericParameters.find(key);
        if (it != numericParameters.end())
        {
            return it->second;
        }
        else
        {
            return -1;
        }
    }
    const vector<Robot *> &getNeighbors() const
    {
        return neighbors;
    };

    void addNeighbor(Robot *neighbor)
    {
        neighbors.push_back(neighbor);
        neighbor->neighbors.push_back(&*this);
    };

    void removeNeighbors(Robot *neighbor)
    {
        removeNeighbor(neighbor);
        neighbor->removeNeighbor(&*this);
    };
    void deleteAllNeighbors()
    {
        neighbors.clear();
        // добавить соседей
    };
    virtual bool doesHaveHeighbor(Robot *neighbor)
    {
        if (find(neighbors.begin(), neighbors.end(), neighbor) != neighbors.end())
            return true;
        else
            return false;
    };
    vector<Robot *> visitGraph()
    {
        const std::vector<Robot *> listOfVertex = neighbors;
        std::vector<bool> visited(listOfVertex.size(), false);
        DFS(this, listOfVertex, visited);
        return listOfVertex;
    }

protected:
    unordered_map<string, int> numericParameters;
    vector<Robot *> neighbors;

    void removeNeighbor(Robot *neighbor)
    {
        auto it = find(neighbors.begin(), neighbors.end(), neighbor);
        if (it != neighbors.end())
        {
            neighbors.erase(it);
        }
    };

    void DFS(const Robot *robot, const vector<Robot *> &listOfVertex, vector<bool> &visited)
    {
        auto it = find(listOfVertex.begin(), listOfVertex.end(), robot);
        if (it != listOfVertex.end())
        {
            size_t index = distance(listOfVertex.begin(), it);
            visited[index] = true;
            for (Robot *neighbor : robot->getNeighbors())
            {
                it = find(listOfVertex.begin(), listOfVertex.end(), neighbor);
                if (it != listOfVertex.end())
                {
                    size_t neighborIndex = distance(listOfVertex.begin(), it);
                    if (!visited[neighborIndex])
                    {
                        DFS(neighbor, listOfVertex, visited);
                    }
                }
            }
        }
    };
};

class NewRobot : public Robot
{
private:
    static const int transmissionSpeed = 20;
    vector<MessageInfo<NewRobot *>> receivedMessages;

public:
    NewRobot() : Robot()
    {
        vector<pair<string, time_t>> receivedMessages;
    };
    NewRobot(const NewRobot &robot) : Robot(robot)
    {
        receivedMessages = robot.getReceivedMessages();
    };
    ~NewRobot(){};

    const int getTransmissionSpeed() const override
    {
        return transmissionSpeed;
    };
    RobotKind getType() const override
    {
        return newRobot;
    };

    const vector<MessageInfo<NewRobot *>> &getReceivedMessages() const
    {
        return receivedMessages;
    };

    void receiveMessage(NewRobot *sender, const string &str)
    {
        receivedMessages.push_back(MessageInfo<NewRobot *>(str, time(nullptr), sender));
    };

    bool doesHaveMessage(string &str)
    {
        for (const MessageInfo<NewRobot *> &messageInfo : receivedMessages)
        {
            if (messageInfo.message == str)
            {
                return true;
            }
        }
        return false;
    };

    void deleteAllMessages()
    {
        receivedMessages.clear();
    }
    // ПЕреписать с возвратом булевой перменной
    void transmitMessage(const string &message, const Robot *robot)
    {
        vector<Robot *> robots = this->visitGraph();
        auto it = find(robots.begin(), robots.end(), robot);
        if (it != robots.end() && robot->getType() == newRobot)
        {
            ((NewRobot *)robot)->receiveMessage(&*this, message);
        }
    };
};

class OldRobot : public Robot
{
private:
    static const int transmissionSpeed = 10;

public:
    OldRobot() : Robot(){};
    OldRobot(const OldRobot &robot) : Robot(robot){};
    ~OldRobot(){};
    const int getTransmissionSpeed() const override
    {
        return transmissionSpeed;
    };
    RobotKind getType() const override
    {
        return oldRobot;
    };
};

int main()
{
    NewRobot newRobot1;
    assert(newRobot1.getTransmissionSpeed() == 20);
    assert((newRobot1.getNeighbors()).empty() == true);
    assert((newRobot1.getNumericParameters()).empty() == true);
    assert((newRobot1.getReceivedMessages()).empty() == true);
    cout << "Test #1 passed\n";
    OldRobot oldRobot1;
    assert(oldRobot1.getTransmissionSpeed() == 10);
    assert((oldRobot1.getNeighbors()).empty() == true);
    assert((oldRobot1.getNumericParameters()).empty() == true);
    cout << "Test #2 passed\n";

    newRobot1.setNumericParameter("Длина", 30);
    newRobot1.setNumericParameter("Ширина", 40);
    assert(newRobot1.getNumericParameter("Длина") == 30);
    assert(newRobot1.getNumericParameter("Ширина") == 40);
    assert((newRobot1.getNumericParameters()).size() == 2);
    assert(newRobot1.getNumericParameter("Высота") == -1);
    cout << "Test #3 passed\n";

    oldRobot1.setNumericParameter("Длина", 15);
    oldRobot1.addNeighbor(&newRobot1);
    assert(oldRobot1.doesHaveHeighbor(&newRobot1) == true);
    assert(newRobot1.doesHaveHeighbor(&oldRobot1) == true);
    cout << "Test #4 passed\n";

    OldRobot oldRobot2(oldRobot1);
    assert(oldRobot2.getTransmissionSpeed() == oldRobot1.getTransmissionSpeed());
    assert((oldRobot2.getNeighbors()).size() == (oldRobot1.getNeighbors()).size());
    assert(oldRobot2.getNumericParameters().size() == oldRobot1.getNumericParameters().size());
    assert(newRobot1.doesHaveHeighbor(&oldRobot2) == true);
    cout << "Test #5 passed\n";

    NewRobot newRobot2(newRobot1);
    assert(newRobot2.doesHaveHeighbor(&oldRobot2) == true);
    assert(newRobot2.doesHaveHeighbor(&oldRobot1) == true);
    assert(newRobot2.getNumericParameter("Длина") == 30);
    assert(newRobot2.getNumericParameter("Ширина") == 40);
    assert((newRobot2.getNumericParameters()).size() == 2);
    assert(newRobot2.getTransmissionSpeed() == 20);
    cout << "Test #6 passed\n";

    newRobot1.removeNeighbors(&oldRobot2);
    assert(newRobot1.doesHaveHeighbor(&oldRobot2) == false);
    assert(oldRobot2.doesHaveHeighbor(&newRobot1) == false);
    cout << "Test #7 passed\n";

    assert(oldRobot1.getType() == oldRobot);
    assert(newRobot1.getType() == newRobot);
    cout << "Test #8 passed\n";

    newRobot1.addNeighbor(&oldRobot2);
    newRobot1.addNeighbor(&newRobot2);
    vector<Robot *> allRobots = newRobot1.visitGraph();
    assert(allRobots.size() == 3);
    assert(allRobots[2] == &newRobot2);
    assert(allRobots[1] == &oldRobot2);
    assert(allRobots[0] == &oldRobot1);
    cout << "Test #9 passed\n";

    string message = "Hello";
    newRobot1.transmitMessage(message, &newRobot2);
    assert(newRobot2.getReceivedMessages().size() == 1);
    assert(newRobot2.getReceivedMessages()[0].message == "Hello");
    assert(newRobot2.getReceivedMessages()[0].sender == &newRobot1);
    cout << "Test #10 passed\n";

    newRobot2.deleteAllMessages();
    assert(newRobot2.getReceivedMessages().size() == 0);
    cout << "Test #11 passed\n";

    /* newRobot1.transmitMessage(message, &oldRobot1);
    assert(oldRobot1.getReceivedMessages().size() == 0);
    cout << "Test #12 passed\n"; */
    return 0;
}
