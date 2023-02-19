// АиСД - 2, 2023, задание 5
// Давиденко Алексей Ильич, БПИ214
// Clion + Jupyter Notebook
// Написаны сортировки, проведены замеры по времени и количеству элементарных операций для 4 видов массивов в 2 разных
// диапазонах, результаты в гугл-таблицах, построение графиков в Jupyter Notebook с выводами

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>

long long number_of_operations = 0; // Глобальная переменная для подсчёта операций

int BinarySearch(const int *a, int l, int r, int k) { // Вспомогательная функция для бинарного поиска
    int mid;
    number_of_operations += 1;
    while (r - l > 1) {
        mid = l + (r - l) / 2;
        number_of_operations += 7;
        if (a[mid] < k) {
            l = mid;
        } else {
            r = mid;
        }
        number_of_operations += 1;
    }
    number_of_operations += 2;
    return r;
}

int findDigit(int a, int b) { // Вспомогательная функция для поиска цифры в числе
    int c = 1;
    number_of_operations += 2;
    for (int i = 0; i < b - 1; i++) {
        number_of_operations += 3;
        c *= 10;
    }
    a /= c;
    a %= 10;
    number_of_operations += 9;
    return a;
}

void makeHeap(int a, int *array, int n) { // Вспомогательная функция для построения  кучи
    int lc, rc, mc = a, cur;
    a = -1;
    number_of_operations += 6;
    while (mc != a) {
        number_of_operations += 28;
        a = mc;
        lc = 2 * a + 1;
        rc = 2 * a + 2;
        mc = a;
        if (lc < n && array[lc] > array[mc]) {
            number_of_operations += 2;
            mc = lc;
        }
        if (rc < n && array[rc] > array[mc]) {
            number_of_operations += 2;
            mc = rc;
        }
        cur = array[a];
        array[a] = array[mc];
        array[mc] = cur;
    }
    number_of_operations += 1;
}

void SelectionSort(int *a, int n) { // Сортировка выбором
    int tmp, key;
    number_of_operations += 2;
    for (int i = 0; i < n; i++) { // Проходимся по массиву
        number_of_operations += 2; // Сравнение и увеличивание
        key = i;
        number_of_operations += 1; // Присваивание
        for (int j = i + 1; j < n; j++) { // Находим минимум
            number_of_operations += 3; // Сравнение, увеличивание и берём значение i
            if (a[j] < a[key]) {
                key = j;
                number_of_operations += 2; // Сравнение и присваивание
            }
        }
        if (key != i) { // Ставим на место
            tmp = a[i];
            a[i] = a[key];
            a[key] = tmp;
            number_of_operations += 8; // Сравнение, присваивание, взятие элемента
        }
    }
}

void BubbleSort(int *a, int n) { // Сортировка пузырьком
    int tmp;
    number_of_operations += 1;
    for (int i = 0; i < n - 1; i++) { // Двойной цикл
        number_of_operations += 2; // Сравнение и увеличивание
        for (int j = 0; j < n - i - 1; j++) { // по массиву
            number_of_operations += 4; // Сравнениe, увеличивание, берём 2 значения
            if (a[j] > a[j + 1]) {
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                number_of_operations += 8; // Сравнение, присваивание, взятие элемента
            }
        }
    }
}

void BubbleSort1(int *a, int n) { // Сортировка пузырьком с первым условием Айверсона
    int i = 0, tmp;
    number_of_operations += 3;
    bool flag = true; // Флаг для того, чтобы понять если мы всё отсортировали
    number_of_operations += 1;
    while (flag) {
        number_of_operations += 1; // Сравнение
        flag = false;
        number_of_operations += 1; // Присваивание
        for (int j = 0; j < n - i - 1; j++) {
            number_of_operations += 5; // Сравнениe, увеличивание, берём 2 значения
            if (a[j] > a[j + 1]) {
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                flag = true; // Если была замена
                number_of_operations += 9; // Сравнение, присваивание, взятие элемента
            }
        }
        i++;
        number_of_operations += 1;
    }
}

void BubbleSort2(int *a, int n) { // Сортировка пузырьком с двумя условиями Айверсона
    int i = 0, tmp, prev = n - 1, prev1 = 0; // Будем запоминать на каком элементе закончили в прошлый раз
    number_of_operations += 8;
    bool flag = true;
    number_of_operations += 1;
    while (flag) {
        number_of_operations += 1;
        flag = false;
        number_of_operations += 1;
        for (int j = 0; j < prev; j++) { // Идём до этого элемента
            number_of_operations += 4;
            if (a[j] > a[j + 1]) {
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                flag = true;
                prev1 = j;
                number_of_operations += 10;
            }
        }
        i++;
        number_of_operations += 2;
        if (prev1 == 0) { // Если уже отсортировали
            number_of_operations += 2;
            break;
        }
        prev = prev1;
        number_of_operations += 1;
    }
}

void InsertionSort(int *a, int n) { // Сортировка вставками
    int key, i;
    number_of_operations += 2;
    for (int j = 1; j < n; j++) {
        number_of_operations += 3;
        key = a[j];
        i = j - 1;
        number_of_operations += 4;
        while (i >= 0 && a[i] > key) { // Ищем место и двигаем массив
            a[i + 1] = a[i];
            i--;
            number_of_operations += 4;
        }
        a[i + 1] = key; // Вставляем элемент
        number_of_operations += 2;
    }
}

void BinaryInsertionSort(int *a, int n) { // Сортировка бинарными вставками
    int j, k, tmp;
    number_of_operations += 3;
    for (int i = 1; i < n; i++) {
        j = i - 1;
        k = BinarySearch(a, -1, j + 1, a[i]); // С помощью бинарного поиска ищем место
        number_of_operations += 9;
        for (int m = j; m >= k; m--) { // Перетаскиваем элемент на нужное место
            number_of_operations += 8;
            tmp = a[m];
            a[m] = a[m + 1];
            a[m + 1] = tmp;
        }
    }
}

void CountingSort(int *a, int n) { // Сортировка подсчётом устойчивая
    int c[4101] = {}, b[n]; // Массив из 4101 элемента
    number_of_operations += 4104;
    for (int i = 0; i < n; ++i) {
        c[a[i]]++;
        number_of_operations += 6;
    }
    for (int i = 1; i < 4101; ++i) {
        c[i] = c[i - 1] + c[i];
        number_of_operations += 6;
    }
    for (int i = n - 1; i >= 0; --i) {
        c[a[i]]--;
        b[c[a[i]]] = a[i];
        number_of_operations += 11;
    }
    for (int i = 0; i < n; i++) {
        a[i] = b[i];
        number_of_operations += 5;
    }
}

void RadixSort(int *a, int n) { // Цифровая сортировка
    int c[10], b[n];
    number_of_operations += 3;
    for (int k = 1; k <= 4; k++) { // Всего 4 разряда, так как числа <= 4100
        number_of_operations += 2;
        for (int &i: c) {
            number_of_operations += 1;
            i = 0;
        }
        for (int i = 0; i < n; i++) {
            number_of_operations += 5;
            c[findDigit(a[i], k)]++;
        }
        for (int i = 1; i < 10; i++) {
            number_of_operations += 4;
            c[i] += c[i - 1];
        }
        for (int i = n - 1; i >= 0; i--) {
            number_of_operations += 15;
            c[findDigit(a[i], k)]--;
            b[c[findDigit(a[i], k)]] = a[i];
        }
        for (int i = n - 1; i >= 0; i--) {
            number_of_operations += 5;
            a[i] = b[i];
        }
    }
}

void MergeSort(int *a, int n) { // Сортировка слиянием
    if (n == 1) { // Массив из 1 элемента отсортирован
        return;
    }
    number_of_operations += 3;
    MergeSort(a, n / 2); // Рекурсивно для левой половины
    number_of_operations += 4;
    MergeSort(&a[n / 2], n - n / 2); // Рекурсивно для правой половины
    number_of_operations += 7;
    int left = 0, right = 0, b[n]; // Вспомогательный массив b
    number_of_operations += 5;
    for (int i = 0; i < n; i++) { // Сортировка + слияние
        number_of_operations += 2;
        if (a[left] < a[n / 2 + right]) {
            b[i] = a[left];
            left++;
        } else {
            b[i] = a[n / 2 + right];
            right++;
        }
        number_of_operations += 10;
        if (left == n / 2) {
            while (right < n - n / 2) {
                number_of_operations += 15;
                i++;
                b[i] = a[n / 2 + right];
                right++;
            }
            break;
        } else if (right == n - n / 2) {
            while (left < n / 2) {
                number_of_operations += 15;
                i++;
                b[i] = a[left];
                left++;
            }
            break;
        }
        number_of_operations += 4;
    }
    for (int i = 0; i < n; i++) { // Переносим в исходный массив
        a[i] = b[i];
        number_of_operations += 7;
    }
}

void QuickSort(int *a, int n) { // Быстрая сортировка
    int i = 0, j = n - 1, tmp; // Опорным берём первый элемент
    number_of_operations += 6;
    do {
        while (a[i] < a[0]) {
            i++;
            number_of_operations += 6;
        }
        while (a[j] > a[0]) {
            j--;
            number_of_operations += 6;
        }
        number_of_operations += 3;
        if (i <= j) {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
            number_of_operations += 12;
        }
        number_of_operations += 3;
    } while (i <= j);
    if (j > 0) {
        QuickSort(a, j + 1); // Рекурсивно для левой части
        number_of_operations += 4;
    }
    if (i < n) { // Рекурсивно для правой части
        QuickSort(&a[i], n - i);
        number_of_operations += 5;
    }
    number_of_operations += 2;
}

void HeapSort(int *a, int n) { // Пирамидальная сортировка
    int res;
    number_of_operations += 1;
    for (int i = n / 2; i >= 0; i--) { // Делаем из массива кучу
        makeHeap(i, a, n);
        number_of_operations += 6;
    }
    for (int i = n - 1; i >= 0; i--) { // Сортируем
        number_of_operations += 13;
        res = a[0];
        a[0] = a[i];
        a[i] = res;
        makeHeap(0, a, i);
    }
}

void ShellSort1(int *a, int n) { // Сортировка Шелла с последовательностью Шелла
    int tmp, j;
    number_of_operations += 2;
    for (int d = n / 2; d != 0; d /= 2) { // Выбираем расстояние деля на 2
        number_of_operations += 4;
        for (int i = d; i < n; i++) { // Сама сортировка Шелла
            number_of_operations += 8;
            tmp = a[i];
            for (j = i; j >= d; j -= d) {
                number_of_operations += 7;
                if (tmp < a[j - d]) {
                    a[j] = a[j - d];
                    number_of_operations += 5;
                } else {
                    number_of_operations += 1;
                    break;
                }
            }
            a[j] = tmp;
        }
    }
}

void ShellSort2(int *a, int n) { // Сортировка Шелла с последовательностью Циура
    int d[9] = {1750, 701, 301, 132, 57, 23, 10, 4, 1}; // Сама последовательность
    int tmp, j;
    number_of_operations += 12;
    for (int k = 0; k <= 8; k++) { // Проходим по последовательности
        number_of_operations += 5;
        if (d[k] >= n) { // Не превышаем размер массива
            number_of_operations += 1;
            continue;
        }
        for (int i = d[k]; i < n; i++) { // Сама сортировка Шелла
            tmp = a[i];
            for (j = i; j >= d[k]; j -= d[k]) {
                number_of_operations += 9;
                if (tmp < a[j - d[k]]) {
                    number_of_operations += 8;
                    a[j] = a[j - d[k]];
                } else {
                    number_of_operations += 1;
                    break;
                }
            }
            a[j] = tmp;
            number_of_operations += 9;
        }
    }
}

bool SortCheck(const int *a, int n) { // проверяем сортировку проходом по массиву
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) {
            return false; // если была ошибка функция закончится
        }
    }
    return true; // ошибки не было - true
}

int main() {

    // Тест сортировок
    setlocale(LC_ALL, "Russian");

    // Основной массив
    int a[4100];

    // Массив указателей на функции
    void (*sorting_functions[13])(int *, int) = {SelectionSort, BubbleSort, BubbleSort1, BubbleSort2,
                                                 InsertionSort, BinaryInsertionSort, CountingSort, RadixSort,
                                                 MergeSort, QuickSort, HeapSort, ShellSort1, ShellSort2};

    // Массив для открытия файлов с нужными массивами
    std::string arrays[4] = {
            "C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\random_5_array.txt",
            "C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\random_4000_array.txt",
            "C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\almost_sorted_array.txt",
            "C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\reverse_sorted_array.txt"};

    // Проверка корректности сортировки
    /*if (SortCheck(a, 4100)) {
        std::cout << "Сортировка успешна" << std::endl;
    } else {
        std::cout << "Косяк" << std::endl;
    }*/

    // Установка seed для корректного рандома
    /*auto ltime = time(nullptr);
    unsigned int stime = (unsigned int) ltime / 2;
    srand(stime);*/

    //Заполнение отсортированного массива
    /*for (int i = 0; i < 4100; i++) {
        a[i] = i;
    }

    //Заполнение обратно отсортированного массива
    for (int i = 0; i < 4100; i++) {
        a[i] = 4100 - i;
    }

    //Заполнение радомно отсортированного массива (0 - 5)
    for (int i = 0; i < 4100; i++) {
        a[i] = rand() % 6;
    }

    //Заполнение радомно отсортированного массива (0 - 4100)
    for (int i = 0; i < 4100; i++) {
        a[i] = rand() % 4101;
    }

    //Заполнение почти отсортированного массива
    for (int i = 0; i < 82; i++) {
        int l = i * 50, r = l + 50, tmp, first, second;
        for (int j = 0; j < 5; j++) {
            first = l + rand() % (r - l);
            second = l + rand() % (r - l);
            tmp = a[first];
            a[first] = a[second];
            a[second] = tmp;
        }
    }*/

    // Записываем массив в нужный файл
    /*std::ofstream out;
    out.open("C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\almost_sorted_array.txt");
    for (int i = 0; i < 4100; i++) {
        out << a[i] << std::endl;
    }*/

    // Замер времени для массивов 50 - 300 сразу для всех сортировок и видов массивов
    /*std::ofstream out;

    // Открываем csv
    out.open ("C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\first_time.csv");

     // Цикл по размерам массивов
    for (int array_size = 50; array_size <= 300; array_size += 50) {

        // Динамически создаём массив, который будем сортировать
        int *b = new int[array_size];

        // Цикл по сортировкам
        for (int sorting_number = 0; sorting_number < 13; sorting_number++) {

            // Цикл по видам массивов
            for (int array_type = 0; array_type < 4; array_type++) {

                // Считываем нужный массив
                std::ifstream in(arrays[array_type]);
                for (int i = 0; i < array_size; i++) {
                    in >> a[i];
                }

                // Переменная для замера времени
                long long sum = 0;

                // Запускам сортироку на одном и том же массиве 100 раз
                for (int i = 0; i < 100; i++) {
                    for (int j = 0; j < array_size; j++) {
                        b[j] = a[j];
                    }

                    // Измеряем время
                    auto start = std::chrono::steady_clock::now();
                    sorting_functions[sorting_number](b, array_size);
                    auto end = std::chrono::steady_clock::now();
                    sum += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                }

                // Выводим среднее
                out << sum / 100 << ',';
            }
        }

        //Очищаем динамическую память
        delete[]b;
        out << '\n';
    }
    */

    // Замер времени для массивов 100 - 4100 сразу для всех сортировок и видов массивов
    /*std::ofstream out;
    out.open ("C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\second_time.csv");
    for (int array_size = 100; array_size <= 4100; array_size += 100) {
        int *b = new int[array_size];
        for (int sorting_number = 0; sorting_number < 13; sorting_number++) {
            for (int array_type = 0; array_type < 4; array_type++) {
                std::ifstream in(arrays[array_type]);
                for (int i = 0; i < array_size; i++) {
                    in >> a[i];
                }
                long long sum = 0;
                for (int i = 0; i < 100; i++) {
                    for (int j = 0; j < array_size; j++) {
                        b[j] = a[j];
                    }
                    auto start = std::chrono::steady_clock::now();
                    sorting_functions[sorting_number](b, array_size);
                    auto end = std::chrono::steady_clock::now();
                    sum += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                }
                out << sum / 100 << ',';
            }
        }
        delete[]b;
        out << '\n';
    }*/

    // Замер операций для массивов 50 - 300 сразу для всех сортировок и видов массивов
    /*std::ofstream out;
    out.open("C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\first_operations.csv");
    for (int array_size = 50; array_size <= 300; array_size += 50) {
        int *b = new int[array_size];
        for (int sorting_number = 0; sorting_number < 13; sorting_number++) {
            for (int array_type = 0; array_type < 4; array_type++) {
                std::ifstream in(arrays[array_type]);
                for (int i = 0; i < array_size; i++) {
                    in >> a[i];
                }
                number_of_operations = 0;
                sorting_functions[sorting_number](a, array_size);
                out << number_of_operations << ',';
            }
        }
        delete[]b;
        out << '\n';
    }*/

    // Замер операций для массивов 100 - 4100 сразу для всех сортировок и видов массивов
    /*std::ofstream out;
    out.open("C:\\Users\\pupki\\Desktop\\2022 - 2023 уч. год\\Построение и анализ алгоритмов\\КДЗ\\КДЗ_1\\second_operations.csv");
    for (int array_size = 100; array_size <= 4100; array_size += 100) {
        int *b = new int[array_size];
        for (int sorting_number = 0; sorting_number < 13; sorting_number++) {
            for (int array_type = 0; array_type < 4; array_type++) {
                std::ifstream in(arrays[array_type]);
                for (int i = 0; i < array_size; i++) {
                    in >> a[i];
                }
                number_of_operations = 0;
                sorting_functions[sorting_number](a, array_size);
                out << number_of_operations << ',';
            }
        }
        delete[]b;
        out << '\n';
    }*/

    return 0;
}
