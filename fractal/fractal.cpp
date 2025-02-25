#include "SFML/Graphics.hpp"

using namespace sf; // подключаем пространство имен sf

struct StackValue // структура, описывающая точку центра и свойства окружности
{
    float x, y;
    float radius;
    int nestingLength;
};

struct StackItem // собственно сам элемент стека
{
    StackValue value; // точка и свойства
    StackItem* next;  // указатель на следующий элемент в списке
};

StackItem* stack = nullptr; // указатель на вершину стека, изначально стек пуст

bool IsEmpty() // функция проверяет пуст ли стеk
{
    return stack == nullptr;
}

void Push(const StackValue& value) // Добавление точки в стек (аналог функции AddFirst линейного списка)
{
    //StackItem* item = new StackItem{ point, stack }; // этой строчкой можно заменить следующие 2
    StackItem* item = new StackItem; // создаем новый элемент стека
    item->value = value; //инициализируем данные в элементе
    item->next = stack; //корректируем указатель на следующий элемент
    stack = item; // теперь вершина стека указывает на добавленный элемент
}

StackValue Pop() // извлекает элемент из стека и возвращает его значение
{
    StackValue result; // здесь будет результат
    StackItem* temp; // временная переменная
    if (stack == nullptr) // если стек пуст
    {
        result.x = 0;   // то операция извлечения невозможна
        result.y = 0;   // пока мы не умеем реагировать на такую ситуацию
        return result;  // поэтому пока возвращаем Point {0,0}
    }
    result = stack->value; //записываем возвращаемые данные
    temp = stack;   // запоминаем элемент на вершине стека
    stack = temp->next; // перемещаем вершину стека на следующий за извлекаемым элемент
    delete temp;    // освобождаем память, занятую элементом на вершине
    return result; // возвращаем результат
}
// Объект, который, собственно, является главным окном приложения

RenderWindow window(VideoMode(800, 600), "Circles");

// xc, yc - координаты центра
// radius - радиус главной окружности
// nestingLength - глубина вложенности рисования (1- одна окружность, 2 - 5 окружностей и т.д.)

void StackCircleCollection(float xc, float yc, float radius, int nestingLength)
{
    Push(StackValue{ xc, yc, radius, nestingLength }); // заносим в стек свойства базовой окружности
    while (!IsEmpty()) // цикл пока стек не пуст
    {
        auto point = Pop(); // извлекаем координаты и свойства нового квадрата из стека
        RectangleShape rectangle(Vector2f(point.radius*2, point.radius*2)); // создаем окружность
        rectangle.setOrigin(point.radius, point.radius); // устанавливаем точку привязки по центру окружности
        rectangle.setOutlineColor(Color::Black); // цвет границы черный
        rectangle.setFillColor(Color::Transparent); // заливка прозрачная
        rectangle.setOutlineThickness(1); // толщина 1
        rectangle.setPosition(point.x, point.y); // перемещаем окружность в точку xc, yc
        window.draw(rectangle); // отрисовываем квуадрат в буфере кадра
        if (point.nestingLength > 1) // если глубина вложенности превышает 1, то помещаем в стек данные о 4-х новых окружностях
        {                                                      // располагающихся сверху, снизу, слева и справа от текущей окружности
            Push(StackValue{ point.x - point.radius, point.y - point.radius, point.radius/2, point.nestingLength - 1 });
            Push(StackValue{ point.x - point.radius, point.y + point.radius, point.radius/2, point.nestingLength - 1 });
            Push(StackValue{ point.x + point.radius, point.y - point.radius, point.radius/2, point.nestingLength - 1 });
            Push(StackValue{ point.x + point.radius, point.y + point.radius, point.radius/2, point.nestingLength - 1 });
        }
    }
}

int main()
{
    // Главный цикл приложения: выполняется, пока открыто окно
    window.setFramerateLimit(1);
    while (window.isOpen())
    {
        // Обрабатываем очередь событий в цикле
        Event event;
        while (window.pollEvent(event))
        {
            // Пользователь нажал на «крестик» и хочет закрыть окно?
            if (event.type == Event::Closed)
                window.close(); // тогда закрываем его
        }
        // Установка цвета фона
        window.clear(Color::White);
        // рисуем наш узор рекурсивной функцией
        StackCircleCollection(400, 300, 150, 4);
        // Отрисовка окна
        window.display();
    }
}
