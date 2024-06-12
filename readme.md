# Язык программирования "Rap language" и компилятор для него

Сергей Кулагин, 1 курс ФРКТ МФТИ

## Сборка и запуск

### Для сборки:

1. Клонируйте репозиторий: 
    ```console
    $ git clone https://github.com/metaironia/MyProgrammingLanguage
    ```

2. Перейдите в папку с исходниками:

    ```console
    $ cd MyProgrammingLanguage/src
    ```

3. Соберите компилятор:
    - Для сборки frontend-части:
        ```console
        $ cd compiler_frontend
        $ make -f makefile_frontend 
        ```
    - Для сборки middle-части:
        ```console
        $ cd compiler_middleend
        $ make -f makefile_middleend
        ```
    - Для сборки backend-части:
        ```console
        $ cd compiler_backend
        $ make -f makefile_backend
        ```

### Для запуска:

* Для запуска frontend-части:
    ```console
    $ compiler_frontend <input_file> <output_file>
    ```
* Для запуска middle-части:
    ```console
    $ compiler_middleend <input_file> <output_file>
    ```
* Для запуска backend-части:
    ```console
    $ compiler_backend <input_file> <output_file>
    ```

## Правила и синтаксис языка "Rap language"

Rap language - Тьюринг-полный язык программирования. 

<!--Возможность, которую дает этот язык программирования - составлять текст для русских рэп , при этом-->

Синтаксис языка похож на синтаксис языка C за некоторыми ключевыми особенностями:

1. Все переменные имеют один тип - double;

2. Имена переменных и функций обязательно должны быть на английском языке;

3. Все ключевые слова являются либо словами на русском языке, либо знаками препинания;

4. Русские слова, не являющиеся ключевыми, разрешено использовать в любом месте и порядке (они считаются комментариями);

5. Каждая функция обязана возвращать значение;

6. Как и в языке C, в программе должна присутствовать функция `main`.

Подробнее про каждый из этих пунктов речь пойдет далее.

### 1. Тип переменных

Rap language допускает только один тип переменных - double. Он отвечает за хранение вещественного числа в переменной. 

### 2. Имена переменных

В качестве имен переменных может использоваться любое имя переменной, которое допустимо в языке C. Любое имя переменной, которое недопустимо в C, будет так же недопустимым именем переменной и в Rap language. Те же правила и для имен функций.

### 3 и 4. Ключевые слова

Большинство ключевых слов в Rap language - это русские слова, которые чаще всего используются русскими рэп-исполнителями в своих песнях. 

Для простоты понимания, приведем аналогию с ключевыми словами языка C. Так, несколько ключевых слов в Rap language могут обозначать одно и то же ключевое слово в языке C. Важно отметить, что если слово в Rap language является ключевым, то являются также ключевыми и все слова, в которые входит это слово.

Например, для ключевого слово "видел" (назовем его минимальным) можно придумать ряд ключевых слов, основанных на нем: "увидел", "видела", "развидела" и т.д.

Полное описание минимальных ключевых слов приведено в таблице через запятую, причем каждое ключевое слово заключено в кавычки для ясности; приведены также и аналогичные ключевые слова в языке C:

| Минимальное ключевое слово в Rap language | Аналогичное ключевое слово в C | Ключевое слово в стандарте AST |
|:----------------------------------------: | :----------------------------: | :------------------: |
| "сюда", "сипаю" | `double` | TYPE_INT |
| "теперь", "это", "сейчас", "равно", "будет", "равен" | `=` | ASSIGN |
| "нужен", "если", "хотя", "хоть", "когда" | `if` | IF |
| "пока" | `while` | WHILE |
| "звоню", "звонил", "пуллап", "зовет", "зову", "звонить" | ближайший аналог - имя функции как вызов | FUNC_CALL |
| "отдай", "отдавай", "домой", "дать", "вернуть", "верни" | `return` | RETURN |
| "видел", "вижу", "смотрю", "смотрел", "читать", "читаю" | `scanf` за некоторыми исключениями | READ |
| "говорил", "говорит", "базарит", "базарил", "сказал","пишем", "пишет", "базаришь" | `printf` за некоторыми исключениями | PRINT |
| "где", "гэнг", "генг", "братва", "кореш", "взял" | `(` при объявлении аргументов объявляемой функции | FUNC_ARG |
| "открыть", "открыл" | `(` в математических выражениях | OPEN_PARENTHESIS |
| "закрыть", "закрыл" | `)` в математических выражениях | CLOSE_PARENTHESIS |
| "думал", "думает", "будто" | `==` | EQ |
| "не" | `!=` | NOT_EQ |
| "больше", "богаче" | `>` | GREATER |
| "меньше" | `<` | LESS |
| "множу", "множит", "множить", "многократно", "множать" | `*` | MUL |
| "делю", "делим", "разделим", "разделяем" | `/` | DIV |
| "суммирую", "суммировать", "плюс", "прибавляю", "прибавлять", "прибыль" | `+` | ADD |
| "отнимаю", "минус", "убыток", "отнимаешь" | `-` | SUB |
| "степен" | `pow` за некоторыми исключениями | POW |
| "корен" | `sqrt` | SQRT |
| "либо", "или" | `\|\|` | OR |
| "и" | `&&` | AND |
| ",", "кстати", "эй" | `{` | OPEN_BRACE |
| ";", "йо", "йоу", "гоу", "хей", "хэй" | `;` и `}` | LINE_END |

В таблице также даны ключевые слова из стандарта AST (Abstract Syntax Tree). Это сделано для простоты дальнейшего описания использования некоторых ключевых слов Rap language. О стандарте AST и его цели речь пойдет позже.

### 5. Возвращаемое значение функций 

Каждая функция должна обладать возвращаемым значением типа `double`.

### 6. Функция `main`

Программа, написанная на Rap language, обязана иметь функцию `main` подобно языку C, так как выполнение программы начинается с вызова этой функции. 

### Про особенности использования некоторых ключевых слов Rap language

Далее ключевые слова Rap language будем описывать с помощью их названий в стандарте AST.
    
POW работает, как знак возведения в степень:

    variable2 = variable1 в степени 3

Здесь variable2 и variable1 - имена переменных; степени - POW. Значением variable2 будет являться значение переменной variable1, которое возведено в 3-ю степень.

READ считывает число в консоли (по сути результат READ является числом). Присваивание переменной прочитанного значения необходимо делать по аналогии с присваиванием переменной числа (через ASSIGN):

    variable сейчас читаю 

Здесь variable - это имя переменной, куда требуется записать прочитанное значение; сейчас - ASSIGN; читаю - READ.

Распечатать в консоль можно только значение переменной, число распечатать нельзя. Чтобы распечатать значение переменной в консоль, требуется указать PRINT, после чего написать имя переменной, значение которой требуется вывести:

    говорил variable

Здесь variable - это имя переменной, значение которой требуется вывести; говорил - PRINT.

При перечислении аргументов в объявлениях функций запятые, как в языке C, использовать не нужно. Чтобы задать принимаемые аргументы в функцию, после имени функции следует указать FUNC_ARG, после чего перечислить аргументы через пробел. После перечисления всех аргументов, тело функции следует начать с помощью OPEN_BRACE. Пример:

    function arg1 arg2 arg3 эй

Здесь function - имя объявляемой функции; arg1, arg2 и arg3 - аргументы функции; эй - OPEN_BRACE.

Для вызова функции стоит указать FUNC_CALL и после него написать имя вызываемой функции:

    пуллап function

Здесь пуллап - FUNC_CALL; function - имя вызываемой функции.

## Примеры программ на Rap language

Репозиторий содержит две демонстрационных программы на Rap language. Одна из них - это программа, вычисляющая факториал вводимого числа ([fact.txt](src/examples_of_programs/fact.txt)). Другая - программа, решающая квадратное уравнение ([quadratic.txt](src/examples_of_programs/quadratic.txt)).

### Программа, вычисляющая факториал

Для наглядности устройства Rap language продемонстрируем код [fact.txt](src/examples_of_programs/fact.txt) и сравним его с аналогичным кодом на языке C.

Код на языке Rap language:
```
factorial с гэнгом n эй
если n будто 1 эй
отдавай 1 без сомнений
йоу гоу
сюда x равно n минус 1 йо
верни пуллап в factorial
x множит n миллениал йоу йоу

ща на main блоке,
мы делаем банкноты
сюда x теперь читаю йоу 
сюда мой opp будет звонить factorial x гоу
флоу плохой сказал дурак opp йоу
ладно верни тогда 0;
йоу
```
Почти аналогичный код на языке C:
```c
#include <stdio.h>

double factorial (double n) {

    if (n == 1) {

        return 1;
    }
    
    double x = n - 1;

    return factorial (x) * n;
}

int main (void) {

    double x;
    scanf ("%lf", &x);

    double opp = factorial (x);
    printf ("%lf", opp);

    return 0;
}
```

Единственное отличие состоит в том, что в Rap language функция `main` может вернуть значение типа double, а в языке C - нет.

### Программа, вычисляющая корни квадратного уравнения

Сначала в консоль вводятся коэффициенты квадратного уравнения Ax^2 + Bx + C = 0 в порядке A, B и C через ENTER. Затем программа может вывести следующие значения в консоль в зависимости от корней квадратного уравнения:

1. Число 666, если корней бесконечно много;

2. Число 1337, если корней нет;

3. Значение корня уравнения, если корень один;

4. Значения двух корней уравнения, если корней два.

## Компилятор языка Rap language

Компилятор языка Rap language состоит из трех частей: frontend, middle-end и backend.

### Frontend

Frontend часть переводит код с языка Rap language в AST, после чего сохраняет AST в отдельный файл. Пример AST дерева (AST программы [fact.txt](src/examples_of_programs/fact.txt)):

![AST программы fact.txt](/readme_images/fact_ast.png)

Выше описанные правила и синтаксис языка Rap language - это на самом деле правила работы frontend части компилятора.

Стандарт AST будет описан чуть позже.

### Middle-end

Middle-end часть читает файл и строит AST из файла, после чего происходит свертка констант; измененное AST сохраняется в файл. Пример работы middle-end части:

<image src="readme_images/middle_end_work_example.jpg" alt="Пример работы middle-end части" width=70%>


### Backend

Backend часть также читает AST из файла и имеет две модели преобразования AST:

1. Преобразование AST в код [разработанного мной ассемблера](src/MyAsm);

2. Преобразование в NASM x86-64 для последующего исполнения в Win64. 

Например, в такой код на языке разработанного мной ассемблера преобразуется AST демонстрационной программы [fact.txt](src/examples_of_programs/fact.txt) (на картинке отображена лишь начальная часть кода): 

<image src="readme_images/code_my_asm_example.jpg" alt="Пример кода на моем ассемблере" width=50%>

В этом репозитории содержится только первая модель backend части (преобразующая AST в код для моего ассемблера).

Вторая модель содержится в другом репозитории: https://github.com/metaironia/CompilerBackend; подробное описание про эту модель находится в файле readme.md в репозитории по этой ссылке. 

<!-- ## Описание стандарта AST

Стандарт AST придуман вместе с [worthlane](https://github.com/worthlane) и [d3clane](https://github.com/d3clane). -->
