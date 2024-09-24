// Коммивояжер продает конструкторы для умных детей. У элементов конструктора есть выпуклые соединители
// и вогнутые гнезда, в которые вставляются соединители. Элементы могут быть произвольной формы,
// которую можно описать в виде матрицы. Все углы у элемента прямые, высота всех элементов одинакова,
// позиции соединителей и гнезд совпадают. Необходимо научиться хранить схемы из таких элементов.

// Задание 1 - Элемент: расположение соединителей и гнезд, проверка возможности занять часть или все соединители элементом.
// Задание 2 - Умный элемент: элемент с дополнительным функционалом, который можно задействовать
//              в схеме (автономная пищалка, датчик, мотор, … на выбор).
// Задание 3 - Схема: трехмерная схема, проверяющая возможность расположения элементов по слоям.

#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

enum LegoKind
{
    lNone,
    lBeeper,
    lMotor
};

template <typename Type>
class Container
{
public:
    void pushBack(Type element)
    {
        this->queue.push_back(element);
    }
    Type getElement(int k) const
    {
        return this->queue[k];
    }
    int getSize() const
    {
        return this->queue.size();
    }
    bool isEmpty() const
    {
        return this->queue.empty();
    }
    void clearAll() const
    {
        queue.clear();
    }

private:
    vector<Type> queue;
};

// Описание класса
class Lego
{
public:
    // Конструктор по умолчанию
    Lego();
    // Инициализирующий конструктор
    Lego(int legoLength, int legoWidth, int *positionOfConnectors);
    // Копирующий конструктор
    Lego(const Lego &legoPiece);
    // Деструктор
    virtual ~Lego();
    // Несколько функций для get
    int getLegoLength() const;
    int getLegoWidth() const;
    const int *getLegoConnectors() const;
    // Несколько функций для изменения значений полей
    void setNewLegoConnector(int newNumber, int length, int width);
    // Функция для ввода нового объекта с клавиатуры
    virtual void inputLego();
    // Функция вывода схемы
    void outputLego() const;
    // Функция поворота объекта (поморот матрицы соединителей) на 90 градусов
    void rotate90LegoPiece();
    virtual LegoKind getKind() const
    {
        return lNone;
    }

protected:
    // Ширина и длина объекта
    int legoWidth,
        legoLength;
    // Положение соединителей
    int *positionOfConnectors;
};

Lego::Lego()
{
    legoWidth = 1;
    legoLength = 1;
    positionOfConnectors = new int[1];
    *positionOfConnectors = 1;
}
// Функция для корректировки значений матрицы соединителей
int correctPositionOfConnectors(int &n)
{
    if (n < 0)
    {
        n = 0;
    }
    if (n > 10)
    {
        n = 10;
    }
    return n;
}
Lego::Lego(int length, int width, int *connectors)
{
    legoLength = length;
    legoWidth = width;
    positionOfConnectors = new int[legoLength * legoWidth];
    for (int i = 0; i < legoLength * legoWidth; ++i)
    {
        *(positionOfConnectors + i) = correctPositionOfConnectors(*(connectors + i));
    }
}

Lego::Lego(const Lego &legoPiece)
{
    legoWidth = legoPiece.legoWidth;
    legoLength = legoPiece.legoLength;
    int *t = legoPiece.positionOfConnectors;
    positionOfConnectors = new int[legoPiece.legoLength * legoPiece.legoWidth];
    for (int i = 0; i < legoPiece.legoLength * legoPiece.legoWidth; i++)
    {
        *(positionOfConnectors + i) = *(t + i);
    }
}
Lego::~Lego()
{
    delete positionOfConnectors;
}
int Lego::getLegoLength() const
{
    return legoLength;
}
int Lego::getLegoWidth() const
{
    return legoWidth;
}
const int *Lego::getLegoConnectors() const
{
    return positionOfConnectors;
}
void Lego::setNewLegoConnector(int n, int len, int width)
{
    if (len + width <= legoLength + legoWidth && len >= 0 && width >= 0)
    {
        *(positionOfConnectors + (len * legoWidth) + width) = n;
    }
    else
    {
        cout << "This connector position does not exist\n";
    }
}
// Функция вывода схемы детали
void Lego::outputLego() const
{
    cout << "The Lego Piece looks like this:\n";
    for (int i = 0; i < legoLength; i++)
    {
        for (int j = 0; j < legoWidth; j++)
        {
            cout << *(positionOfConnectors + i * (legoWidth) + j) << " ";
        }
        cout << "\n";
    }
}
// Функция ввода произвольной детали
void Lego::inputLego()
{
    int n, m, k;
    cout << "Input the size of Lego piece: \n"
         << "Length: ";
    cin >> n;
    while (n < 0 || n > 10)
    {
        cout << "Length is rather too small or too big. Correct yourself!\n"
             << "Length: ";
        cin >> n;
    }
    cout << "Width: ";
    cin >> m;
    while (m < 0 || m > 10)
    {
        cout << "Width is rather too small or too big. Correct yourself!\n"
             << "Width: ";
        cin >> m;
    }
    cout << "Now input n x m matrix:\n";
    legoLength = n;
    legoWidth = m;
    positionOfConnectors = new int[n * m];
    for (int i = 0; i < n * m; i++)
    {
        cin >> k;
        *(positionOfConnectors + i) = correctPositionOfConnectors(k);
    }
    // cout << "You successfully created one Lego piece!\n";
}

// Функция поворота схемы на 90 градусов
void Lego::rotate90LegoPiece()
{
    int variable;
    // Новый массив
    int new_array[legoLength * legoWidth];
    // Транспонирование
    for (int i = 0; i < legoLength; i++)
    {
        for (int j = 0; j < legoWidth; j++)
        {
            *(new_array + j * legoLength + i) = *(positionOfConnectors + i * legoWidth + j);
        }
    }
    // Поменять местами столбцы
    for (int i = 0; i < legoWidth; i++)
    {
        for (int j = 0; j < legoLength / 2; j++)
        {
            variable = *(new_array + i * legoWidth + j);
            *(new_array + i * legoWidth + j) = *(new_array + i * legoWidth + legoLength - j - 1);
            *(new_array + i * legoWidth + legoLength - j - 1) = variable;
        }
    }
    // Переписать старые данные на новые
    int t = legoLength;
    legoLength = legoWidth;
    legoWidth = t;
    for (int i = 0; i < legoLength * legoWidth; i++)
    {
        *(positionOfConnectors + i) = *(new_array + i);
    }
}
// Функция сравнения положения соединителей двух объектов начиная с какого-то положения

// Новый класс
class MotorLego : public Lego
{
public:
    MotorLego() : Lego(), status(false){};
    MotorLego(int length, int width, int positionOfConnectors[], bool motorStatus = false)
        : Lego(length, width, positionOfConnectors), status(motorStatus){};
    MotorLego(const MotorLego &legoPiece) : Lego(legoPiece)
    {
        status = legoPiece.status;
    };
    virtual ~MotorLego(){
        // delete positionOfConnectors;
    };
    bool getStatus() const
    {
        return status;
    };
    void turnOff()
    {
        status = false;
    };
    void turnOn()
    {
        status = true;
    };
    void inputLego() override
    {
        Lego::inputLego();
        bool k;
        cout << "Input the status of the piece: \n";
        cin >> k;
        status = k;
    };
    LegoKind getKind() const override
    {
        return lMotor;
    }

private:
    bool status;
};

class BeepingLego : public Lego
{
public:
    BeepingLego() : Lego(){};
    BeepingLego(int legoLength, int legoWidth, int *positionOfConnectors)
        : Lego(legoLength, legoWidth, positionOfConnectors){};
    BeepingLego(const BeepingLego &legoPiece) : Lego(legoPiece){};
    void beep() const
    {
        cout << "The Beeping Piece makes a 'beep' sound\n";
    };
    LegoKind getKind() const override
    {
        return lBeeper;
    }
};

// Функция поэлементной проверки на совпадение
bool compareTwoLegoPieces(Lego const &firstPiece, Lego const &secondPiece, int h, int w)
{
    const int width1 = firstPiece.getLegoWidth();
    const int *pointer1 = firstPiece.getLegoConnectors();

    const int len2 = secondPiece.getLegoLength();
    const int width2 = secondPiece.getLegoWidth();
    const int *pointer2 = secondPiece.getLegoConnectors();

    for (int i = 0; i < len2; i++)
    {
        for (int j = 0; j < width2; j++)
        {
            if (*(pointer1 + (h + i) * width1 + w + j) != *(pointer2 + i * width2 + j))
                return false;
        }
    }
    return true;
}

// Функция проверки можно ли соединить для объекта вместе (один поворот)
bool checkLegoPieces(Lego const &firstLego, Lego const &secondLego, Lego &legoForHeight)
{
    int length1 = firstLego.getLegoLength();
    int length2 = secondLego.getLegoLength();
    int width1 = firstLego.getLegoWidth();
    int width2 = secondLego.getLegoWidth();
    const int *pointer1 = firstLego.getLegoConnectors();
    const int *pointer2 = secondLego.getLegoConnectors();
    const int *pointer3 = legoForHeight.getLegoConnectors();

    if (length1 < length2 || width1 < width2)
    {
        return false;
    }
    for (int h = 0; h < length1 - length2 + 1; h++)
    {
        for (int w = 0; w < width1 - width2 + 1; w++)
        {
            if (compareTwoLegoPieces(firstLego, secondLego, h, w) == true)
            {
                int k = *(pointer3 + h * width1 + w);
                for (int i = 0; i < length2; i++)
                {
                    for (int j = 0; j < width2; j++)
                    {
                        if (*(pointer3 + (h + i) * width1 + w + j) != k)
                        {
                            return false;
                        }
                    }
                }
                for (int i = h; i < h + length2; i++)
                {
                    for (int j = w; j < w + width2; j++)
                    {
                        legoForHeight.setNewLegoConnector(k + 1, i, j);
                    }
                }
                return true;
            }
        }
    }
    return false;
}
// Функция проверки можно ли соединить для объекта вместе (любой поворот)
bool canBeJoined(Lego &firstLego, Lego const &secondLego, Lego &legoForHeight)
{
    for (int i = 0; i < 4; i++)
    {
        if (checkLegoPieces(firstLego, secondLego, legoForHeight) == true)
        {
            return true;
        }
        legoForHeight.rotate90LegoPiece();
        firstLego.rotate90LegoPiece();
    }
    return false;
}

Lego joinPieces(Container<Lego *> &legoPieces)
{
    Container<Container<Lego *>> allPossibleCombinations; // для сохранения инфо о всех комбинациях
    Container<Lego *> allLayers;                          // для сохранения инфо о наложения частей друг на друга
    const int N = legoPieces.getSize();

    for (int i = 0; i < N; i++)
    {
        Container<Lego *> piecesCombination;      // комбинация в данном цикле
        Lego *pointer = legoPieces.getElement(i); // элемент относительно которого будет проверка

        // создание детали которая бы показывала наложения (пока все по нулям)
        int arr[pointer->getLegoLength() * pointer->getLegoWidth()];
        for (int i = 0; i < pointer->getLegoLength() * pointer->getLegoWidth(); i++)
        {
            arr[i] = 0;
        }
        Lego *layerLego = new Lego(pointer->getLegoLength(), pointer->getLegoWidth(), arr);

        // цикл проверки
        for (int j = 0; j < N; j++)
        {
            if (i != j && canBeJoined(*pointer, *(legoPieces.getElement(j)), *layerLego) == true)
            {
                piecesCombination.pushBack(legoPieces.getElement(j));
            }
        }
        piecesCombination.pushBack(pointer);

        // сохранить в общем случае если есть какие-то пересечения
        if (piecesCombination.getSize() > 1)
        {
            allPossibleCombinations.pushBack(piecesCombination);
            allLayers.pushBack(layerLego);
        }
    }

    // если ничего не нашлось
    if (allPossibleCombinations.isEmpty())
    {
        int arr[1] = {-1};
        Lego lego(1, 1, arr);
        return lego;
    }
    // если что-то есть, ищем максимальную последовательность элементов
    int maxSizeIndex = 0;
    int maxSize = allPossibleCombinations.getElement(0).getSize();
    for (int i = 0; i < allPossibleCombinations.getSize(); i++)
    {
        int currentSize = allPossibleCombinations.getElement(i).getSize();
        if (currentSize > maxSize)
        {
            maxSizeIndex = i;
            maxSize = currentSize;
        }
    }
    // создание элемента для вывода общей схемы
    Container<Lego *> maxCombination = allPossibleCombinations.getElement(maxSizeIndex);
    Lego neededPiece(*(maxCombination.getElement(maxCombination.getSize() - 1)));
    int width = neededPiece.getLegoWidth();
    const int *pointer = neededPiece.getLegoConnectors();
    const int *maxLayerPointer = allLayers.getElement(maxSizeIndex)->getLegoConnectors();
    /* for (int i = 0; i < maxCombination.getSize(); i++)
    {
        maxCombination.getElement(i)->outputLego();
    } */

    for (int i = 0; i < neededPiece.getLegoLength(); i++)
    {
        for (int j = 0; j < width; j++)
        {
            neededPiece.setNewLegoConnector(*(pointer + i * width + j) + *(maxLayerPointer + i * width + j), i, j);
        }
    }
    return neededPiece;
}

int main()
{
    Lego *legoPiece1 = new Lego;
    assert((*legoPiece1).getLegoLength() == 1);
    cout << "Test #1 passed: default constructor (1)\n";
    assert((*legoPiece1).getLegoWidth() == 1);
    cout << "Test #2 passed: default constructor (2)\n";
    assert(*((*legoPiece1).getLegoConnectors()) == 1);
    cout << "Test #3 passed: default constructor (3)\n";

    int arr[4] = {1, 0, 0, 0};
    Lego *legoPiece2 = new Lego(2, 2, arr);
    assert((*legoPiece2).getLegoWidth() == 2);
    cout << "Test #4 passed: initialising constructor (1)\n";
    assert((*legoPiece2).getLegoWidth() == 2);
    cout << "Test #5 passed: initialising constructor (2)\n";
    assert(*((*legoPiece2).getLegoConnectors()) == 1);
    assert(*((*legoPiece2).getLegoConnectors() + 1) == 0);
    assert(*((*legoPiece2).getLegoConnectors() + 2) == 0);
    assert(*((*legoPiece2).getLegoConnectors() + 3) == 0);
    cout << "Test #6 passed: initialising constructor (3)\n";

    Lego *legoPiece3 = new Lego(*legoPiece2);
    assert((*legoPiece3).getLegoWidth() == 2);
    cout << "Test #7 passed: copying constructor (1)\n";
    assert((*legoPiece3).getLegoWidth() == 2);
    cout << "Test #8 passed: copying constructor (2)\n";
    assert(*((*legoPiece3).getLegoConnectors()) == 1);
    assert(*((*legoPiece3).getLegoConnectors() + 1) == 0);
    assert(*((*legoPiece3).getLegoConnectors() + 2) == 0);
    assert(*((*legoPiece3).getLegoConnectors() + 3) == 0);
    cout << "Test #9 passed: copying constructor (3)\n";

    (*legoPiece3).setNewLegoConnector(1, 0, 1);
    assert(*((*legoPiece3).getLegoConnectors() + 1) == 1);
    cout << "Test #10 passed: function setNewLegoConnector()\n";

    MotorLego motorPiece;
    assert(motorPiece.getLegoLength() == 1);
    cout << "Test #11 passed: motor piece default constructor (1)\n";
    assert(motorPiece.getLegoWidth() == 1);
    cout << "Test #12 passed: motor piece default constructor (2)\n";
    assert(*(motorPiece.getLegoConnectors()) == 1);
    cout << "Test #13 passed: motor piece default constructor (3)\n";
    assert(motorPiece.getStatus() == false);
    cout << "Test #13 passed: motor piece default constructor (3)\n";

    MotorLego motorPiece2(2, 2, arr);
    assert(motorPiece2.getLegoWidth() == 2);
    cout << "Test #14 passed: motor piece initialising constructor (1)\n";
    assert(motorPiece2.getLegoWidth() == 2);
    cout << "Test #15 passed: motor piece initialising constructor (2)\n";
    assert(*(motorPiece2.getLegoConnectors()) == 1);
    assert(*(motorPiece2.getLegoConnectors() + 1) == 0);
    assert(*(motorPiece2.getLegoConnectors() + 2) == 0);
    assert(*(motorPiece2.getLegoConnectors() + 3) == 0);
    cout << "Test #16 passed: motor piece initialising constructor (3)\n";
    assert(motorPiece2.getStatus() == false);
    cout << "Test #17 passed: motor piece initialising constructor (4)\n";

    MotorLego motorPiece3(motorPiece2);
    assert(motorPiece3.getLegoWidth() == 2);
    cout << "Test #18 passed: motor piece copying constructor (1)\n";
    assert(motorPiece3.getLegoWidth() == 2);
    cout << "Test #19 passed: motor piece copying constructor (2)\n";
    assert(*(motorPiece3.getLegoConnectors()) == 1);
    assert(*(motorPiece3.getLegoConnectors() + 1) == 0);
    assert(*(motorPiece3.getLegoConnectors() + 2) == 0);
    assert(*(motorPiece3.getLegoConnectors() + 3) == 0);
    cout << "Test #20 passed: motor piece copying constructor (3)\n";
    assert(motorPiece3.getStatus() == false);
    cout << "Test #21 passed: motor piece copying constructor (4)\n";
    motorPiece3.turnOn();
    assert(motorPiece3.getStatus() == true);
    cout << "Test #22 passed: function turnOn for motor piece\n";

    BeepingLego beepPiece;
    beepPiece.beep();

    BeepingLego *beepPiece2 = new BeepingLego(beepPiece);
    assert((*legoPiece1).getKind() == lNone);
    cout << "Test #23 passed: check identifying method (1)\n";
    assert((*legoPiece1).getKind() == (*legoPiece2).getKind());
    cout << "Test #24 passed: check identifying method (2)\n";
    assert(motorPiece.getKind() == lMotor);
    cout << "Test #25 passed: check identifying method (3)\n";
    assert(beepPiece.getKind() == lBeeper);
    cout << "Test #26 passed: check identifying method (4)\n";
    MotorLego *newLego = new MotorLego;
    assert(((Lego *)newLego)->getKind() == lMotor);
    cout << "Test #27 passed: check identifying method (5)\n";
    BeepingLego *newLego2 = new BeepingLego;
    assert(((Lego *)newLego2)->getKind() == lBeeper);
    cout << "Test #28 passed: check identifying method (6)\n";

    ((Lego *)newLego)->inputLego();
    motorPiece2.inputLego();
    // motorPiece2.outputLego();
    assert(motorPiece2.getKind() == lMotor);
    cout << "Test #29 passed: check inputLego for motor piece\n";

    Lego *pointerLego = new Lego;
    Container<Lego *> container;
    container.pushBack(legoPiece1);
    assert(container.getElement(0)->getKind() == lNone);
    cout << "Test #30 passed: container get element and push back(1)\n";
    container.pushBack(pointerLego);
    assert(container.getElement(1) == pointerLego);
    cout << "Test #31 passed: container get element and push back(2)\n";
    container.pushBack(beepPiece2);
    assert(container.getElement(2)->getKind() == lBeeper);
    cout << "Test #32 passed: container get element and push back(3)\n";
    assert(container.getSize() == 3);
    cout << "Test #33 passed: container size\n";

    Container<int> container2;
    assert(container2.isEmpty() == true);
    cout << "Test #34 passed: container is empty\n";

    Container<Lego *> container1;
    int arr1[9] = {1, 0, 1, 0, 0, 1, 0, 1, 0};
    int arr2[4] = {0, 0, 1, 1};
    int arr3[3] = {0, 1, 0};
    int arr4[2] = {1, 1};
    int arr5[2] = {1, 0};
    Lego *legoPiece12 = new Lego(3, 3, arr1);
    Lego *legoPiece13 = new Lego(2, 2, arr2), *legoPiece14 = new Lego(3, 1, arr3);
    Lego *legoPiece15 = new Lego(2, 1, arr4);
    Lego *legoPiece16 = new Lego(2, 1, arr5);
    /* (*legoPiece12).inputLego();
    (*legoPiece13).inputLego();
    (*legoPiece14).inputLego();
    (*legoPiece15).inputLego(); */
    container1.pushBack(legoPiece12);
    container1.pushBack(legoPiece13);
    container1.pushBack(legoPiece14);
    container1.pushBack(legoPiece15);
    Lego joinedPieces = joinPieces(container1);
    // joinedPieces.outputLego();
    assert(*(joinedPieces.getLegoConnectors()) == 1);
    assert(*(joinedPieces.getLegoConnectors() + 1) == 2);
    assert(*(joinedPieces.getLegoConnectors() + 2) == 1);
    assert(*(joinedPieces.getLegoConnectors() + 3) == 3);
    assert(*(joinedPieces.getLegoConnectors() + 4) == 1);
    assert(*(joinedPieces.getLegoConnectors() + 5) == 0);
    assert(*(joinedPieces.getLegoConnectors() + 6) == 3);
    assert(*(joinedPieces.getLegoConnectors() + 7) == 1);
    assert(*(joinedPieces.getLegoConnectors() + 8) == 1);
    cout << "Test #35 passed: joining multiple pieces\n";
    container1.clearAll();
    assert(container1.isEmpty() == true);
    cout << "Test #36 passed: clear container\n";

    Container<Lego *> container3;
    container3.pushBack(legoPiece15);
    container3.pushBack(legoPiece16);
    Lego joined2 = joinPieces(container3);
    assert(*joined2.getLegoConnectors() == 0);
    assert(joined2.getLegoLength() == 1);
    assert(joined2.getLegoWidth() == 1);
    cout << "Test #37 passed: joining pieces that cannot connect\n";

    return 0;
}