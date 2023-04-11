// АиСД - 2, 2023, задание 5
// Давиденко Алексей Ильич, БПИ214
// Clion + Jupyter Notebook
// Реализованы методы поиска, сгенерированы тексты, проведены замеры по времени, построены графики, написан отчёт

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>

// Вспомогательная функция для хеша с возможностью символа подстановки
bool cmpString(std::string a, int index, std::string b) {
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[index + i] && a[i] != '?') {
            return false;
        }
    }
    return true;
}

// Генерация строк в бинарном алфавите
void binStrings(std::string s, int index, std::vector<std::string> &results) {
    // Если мы дошли до конца строки, добавляем её в массив результатов и завершаем рекурсию.
    if (index == s.size()) {
        results.push_back(s);
        return;
    }
    // Если текущий символ не равен "?", переходим к следующему символу.
    if (s[index] != '?') {
        binStrings(s, index + 1, results);
        return;
    }
    // Если текущий символ равен "?", создаем две копии строки и заменяем "?" на 0 и 1 соответственно.
    std::string s1 = s;
    std::string s2 = s;
    s1[index] = '0';
    s2[index] = '1';
    // Рекурсивно вызываем функцию для обеих созданных строк.
    binStrings(s1, index + 1, results);
    binStrings(s2, index + 1, results);
}

// Генерация строк в днк алфавите
void dnkStrings(std::string s, int index, std::vector<std::string> &results) {
    // Если мы дошли до конца строки, добавляем её в массив результатов и завершаем рекурсию.
    if (index == s.size()) {
        results.push_back(s);
        return;
    }
    // Если текущий символ не равен "?", переходим к следующему символу.
    if (s[index] != '?') {
        dnkStrings(s, index + 1, results);
        return;
    }
    // Если текущий символ равен "?", создаем две копии строки и заменяем "?" на 0 и 1 соответственно.
    std::string s1 = s;
    std::string s2 = s;
    std::string s3 = s;
    std::string s4 = s;
    s1[index] = 'A';
    s2[index] = 'C';
    s3[index] = 'G';
    s4[index] = 'T';
    // Рекурсивно вызываем функцию для обеих созданных строк.
    dnkStrings(s1, index + 1, results);
    dnkStrings(s2, index + 1, results);
    dnkStrings(s3, index + 1, results);
    dnkStrings(s4, index + 1, results);
}

// Brute Force алгоритм с возможностью символа подстановки
void naiveAlgorithm(std::string a, std::string b) {
    auto n = a.length();
    auto m = b.length();
    std::vector<uint64_t> entries;
    // Вложенный цикл
    for (int i = 0; i <= m - n; ++i) {
        bool indicator = true;
        for (int j = 0; j < n; ++j) {
            if (a[j] != b[j + i] and a[j] != '?') {
                indicator = false;
                break;
            }
        }
        if (indicator) {
            entries.push_back(i);
        }
    }
    // Вывод всех вхождений для тестирования функции
    /*std::cout << entries.size() << std::endl;
    for (auto entry: entries) {
        std::cout << entry << std::endl;
    }*/
}

// Обычный алгоритм КМП
void kmpStandart(std::string a, std::string b) {
    int k = 0;
    a.insert(0, 1, '#');
    auto m = a.length() - 1;
    int br[a.length()];
    for (int i = 0; i < a.length(); ++i) {
        br[i] = 0;
    }
    // Префикс функция с возможностью символа подстановки
    for (uint64_t i = 2; i <= m; ++i) {
        while (k > 0 && a[k + 1] != a[i]) {
            k = br[k];
        }
        if (a[k + 1] == a[i] && a[k + 1] != '?' && a[i] != '?') {
            k++;
        }
        br[i] = k;
    }
    a.push_back('#');
    b.insert(0, 1, '#');
    auto n = b.length() - 1;
    uint64_t number = 0;
    std::vector<uint64_t> entries;
    // Поиск с помощью префикс функции и возможностью символа подстановки
    for (uint64_t i = 1; i <= n; ++i) {
        while (number && a[number + 1] != b[i] && a[number + 1] != '?') {
            number = br[number];
        }
        if (a[number + 1] == b[i] || a[number + 1] == '?') {
            number++;
        }
        if (number == m) {
            entries.push_back(i - m);
            number = br[number];
        }
    }
    // Вывод всех вхождений для тестирования функции
    /*std::cout << entries.size() << std::endl;
    for (auto entry: entries) {
        std::cout << entry << std::endl;
    }*/
}

// Алгоритм КМП с уточнёнными гранями
void kmpOptimized(std::string a, std::string b) {
    int k = 0;
    a.insert(0, 1, '#');
    auto m = a.length() - 1;
    int br[a.length()], brs[a.length()];
    for (int i = 0; i < a.length(); ++i) {
        br[i] = 0;
        brs[i] = 0;
    }
    // Префикс функция
    for (uint64_t i = 2; i <= m; ++i) {
        while (k > 0 && a[k + 1] != a[i]) {
            k = br[k];
        }
        if (a[k + 1] == a[i] && a[k + 1] != '?' && a[i] != '?') {
            k++;
        }
        br[i] = k;
    }
    // Уточнённые грани
    for (uint64_t i = 2; i <= m; ++i) {
        if (a[br[i] + 1] != a[i + 1]) {
            brs[i] = br[i];
        } else {
            brs[i] = br[br[i] + 1];
        }
    }
    a.push_back('#');
    b.insert(0, 1, '#');
    auto n = b.length() - 1;
    uint64_t number = 0;
    std::vector<uint64_t> entries;
    // Поиск с помощью префикс функции и возможностью символа подстановки
    for (uint64_t i = 1; i <= n; ++i) {
        while (number && a[number + 1] != b[i] && a[number + 1] != '?') {
            number = brs[number];
        }
        if (a[number + 1] == b[i] || a[number + 1] == '?') {
            number++;
        }
        if (number == m) {
            entries.push_back(i - m);
            number = brs[number];
        }
    }
    // Вывод всех вхождений для тестирования функции
    /*std::cout << entries.size() << std::endl;
    for (auto entry: entries) {
        std::cout << entry << std::endl;
    }*/
}

// Алгоритм Рабина-Карпа
void rkAlgorithm(const std::string& a, std::string b, int abc_size) {
    std::vector<int> entries;
    std::vector<int> hashes;
    std::vector<std::string> strings;
    // Запускаем функцию генерации строк
    if (abc_size == 2) {
        binStrings(a, 0, strings);
    } else {
        dnkStrings(a, 0, strings);
    }
    // Просчитываем хеши всех функций
    for (auto &string: strings) {
        int hash_a = string[0] % 9973;
        for (int i = 1; i < a.length(); ++i) {
            hash_a *= 4;
            hash_a += string[i];
            hash_a %= 9973;
        }
        hashes.push_back(hash_a);
    }
    // Хеш текста
    int hash_b = b[0] % 9973, hash_first = 1;
    for (int i = 1; i < a.length(); ++i) {
        hash_b *= 4;
        hash_b += b[i];
        hash_b %= 9973;
        hash_first *= 4;
        hash_first %= 9973;
    }
    // Сверяем хеши используя доп. функцию
    for (int i = 0; i <= b.length() - a.length(); ++i) {
        for (int hash_a: hashes) {
            if (hash_a != hash_b) {
                continue;
            }
            if (cmpString(a, i, b)) {
                entries.push_back(i);
            }
        }
        if (i == b.length() - a.length()) break;
        hash_b -= (b[i] * hash_first) % 9973;
        hash_b += 9973;
        hash_b *= 4;
        hash_b += b[i + a.length()];
        hash_b %= 9973;
    }
    // Вывод всех вхождений для тестирования функции
    /*std::cout << entries.size() << std::endl;
    for (auto entry: entries) {
        std::cout << entry << std::endl;
    }*/
}

int main() {

    setlocale(LC_ALL, "Russian");

    // Массив для открытия файлов с нужными строками
    /*std::string texts[4] = {"binary_104.txt", "binary_105.txt", "dnk_104.txt", "dnk_105.txt"};
    std::string totals[4] = {"binary_104.csv", "binary_105.csv", "dnk_104.csv", "dnk_105.csv"};
    std::string totals_1[4] = {"binary_104_1.csv", "binary_105_1.csv", "dnk_104_1.csv", "dnk_105_1.csv"};
    std::string totals_2[4] = {"binary_104_2.csv", "binary_105_2.csv", "dnk_104_2.csv", "dnk_105_2.csv"};
    std::string totals_3[4] = {"binary_104_3.csv", "binary_105_3.csv", "dnk_104_3.csv", "dnk_105_3.csv"};
    std::string totals_4[4] = {"binary_104_4.csv", "binary_105_4.csv", "dnk_104_4.csv", "dnk_105_4.csv"};*/

    // Установка seed для корректного рандома
    /*auto ltime = time(nullptr);
    unsigned int stime = (unsigned int) ltime / 2;
    srand(stime);*/

    // Записываем строку в файл
    /*std::ofstream out(texts[3]);
    char dnk[4] = {'A', 'T', 'G', 'C'};
    for (int i = 0; i < 100000; i++) {
        out << dnk[rand() % 4];
    }*/

    // Замер времени без подстановок для всех алфавитов
    /*for (int text = 0; text < 4; ++text) {
        std::ifstream in(texts[text]);
        std::string s;
        // Считываем текст
        in >> s;
        in.close();
        // Создаём массив подстрок из рандомных мест текста
        std::string samples[30];
        std::ofstream out(totals[text], std::ios::app);
        for (int i = 0; i < 30; ++i) {
            samples[i] = s.substr(rand() % (s.length() - (i + 1) * 100), (i + 1) * 100);
        }
        long long sum = 0;
        // Цикл по каждой подстроке, просчитываем 100 раз для точного времени
        for (auto &sample: samples) {
            for (int j = 0; j < 100; j++) {
                auto start = std::chrono::steady_clock::now();
                rkAlgorithm(sample, s);
                auto end = std::chrono::steady_clock::now();
                sum += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }
            out << sum / 100 << ',';
        }
        out << '\n';
        out.close();
    }*/

    // Замер времени с X подстановками для всех алфавитов
    /*for (int text = 0; text < 4; ++text) {
        std::ifstream in(texts[text]);
        std::string s;
        in >> s;
        in.close();
        std::string samples[30];
        std::ofstream out(totals_4[text], std::ios::app);
        for (int i = 0; i < 30; ++i) {
            samples[i] = s.substr(rand() % (s.length() - (i + 1) * 100), (i + 1) * 100);
            // Вставляем символы подстановки (в данном случае 4) в рандомные места, но следим
            // Чтобы все 4 символа были в строке
            int pos = rand() % ((i + 1) * 100);
            samples[i].replace(pos, 1, "?");
            int pos_2 = pos;
            while (pos_2 == pos) {
                pos_2 = rand() % ((i + 1) * 100);
            }
            samples[i].replace(pos_2, 1, "?");
            int pos_3 = rand() % ((i + 1) * 100);
            while (pos_3 == pos || pos_3 == pos_2) {
                pos_3 = rand() % ((i + 1) * 100);
            }
            samples[i].replace(pos_3, 1, "?");
            int pos_4 = rand() % ((i + 1) * 100);
            while (pos_4 == pos || pos_4 == pos_2 || pos_4 == pos_3) {
                pos_4 = rand() % ((i + 1) * 100);
            }
            samples[i].replace(pos_4, 1, "?");
        }
        long long sum = 0;
        for (auto &sample: samples) {
            for (int j = 0; j < 100; j++) {
                int size;
                if (text <= 1) {
                    size = 2;
                } else {
                    size = 4;
                }
                auto start = std::chrono::steady_clock::now();
                rkAlgorithm(sample, s, size);
                auto end = std::chrono::steady_clock::now();
                sum += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }
            out << sum / 100 << ',';
        }
        out << '\n';
        out.close();
    }*/
    return 0;
}