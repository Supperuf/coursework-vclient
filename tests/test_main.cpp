#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>

// Подключаем твои заголовочные файлы
#include "../src/Sha1Auth.h"
#include "../src/VectorLoader.h"
#include "../src/NetworkService.h"

void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
    out.close();
}

int main() {
    std::cout << "=== ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ ===" << std::endl;
    int passed = 0;

    // --- ТЕСТ 1: Генерация SHA-1 (Позитивный) ---
    {
        std::string salt = "A1B2C3D4E5F60718"; 
        std::string pass = "password";
        std::string result = Sha1Auth::computeSHA1(salt, pass);
        
        std::string expected = "487714C694CA6C431010B10B64179232DCF31FB0"; 
        assert(result == expected);
        std::cout << "[PASS] Т01: Корректный хеш SHA-1" << std::endl;
        passed++;
    }

    // --- ТЕСТ 2: Проверка регистра хеша (Позитивный) ---
    {
        std::string salt = "A1B2C3D4E5F60718";
        std::string pass = "password";
        std::string result = Sha1Auth::computeSHA1(salt, pass);
        
        bool is_upper = true;
        for (char c : result) {
            if (std::isalpha(c) && !std::isupper(c)) {
                is_upper = false;
                break;
            }
        }
        assert(is_upper == true);
        std::cout << "[PASS] Т02: Хеш состоит только из заглавных букв" << std::endl;
        passed++;
    }

    // --- ТЕСТ 3: Чтение векторов (Позитивный) ---
    {
        createTestFile("test_vectors.txt", "2\n3 1.1 2.2 3.3\n2 4.4 5.5\n");
        VectorLoader loader;
        auto vecs = loader.load("test_vectors.txt");
        
        assert(vecs.size() == 2);
        assert(vecs[0].size() == 3);
        assert(std::abs(vecs[0][0] - 1.1) < 0.0001); 
        
        std::remove("test_vectors.txt");
        std::cout << "[PASS] Т03: Корректное чтение векторов" << std::endl;
        passed++;
    }

    // --- ТЕСТ 4: Отсутствие файла (Негативный) ---
    {
        VectorLoader loader;
        bool exception_thrown = false;
        try {
            loader.load("no_such_file.txt");
        } catch (...) {
            exception_thrown = true;
        }
        assert(exception_thrown == true);
        std::cout << "[PASS] Т04: Обработка отсутствия файла" << std::endl;
        passed++;
    }

    // --- ТЕСТ 5: Пустой файл (Негативный) ---
    {
        createTestFile("empty_test.txt", ""); // Создаем пустой файл
        VectorLoader loader;
        bool exception_thrown = false;
        try {
            loader.load("empty_test.txt");
        } catch (...) {
            exception_thrown = true;
        }
        std::remove("empty_test.txt");
        assert(exception_thrown == true);
        std::cout << "[PASS] Т05: Обработка пустого файла" << std::endl;
        passed++;
    }

    // --- ТЕСТ 6: Сетевая ошибка (Негативный) ---
    {
        NetworkService net;
        bool exception_thrown = false;
        try {
            net.openConnection("127.0.0.1", 9999); // Закрытый порт
        } catch (...) {
            exception_thrown = true;
        }
        assert(exception_thrown == true);
        std::cout << "[PASS] Т06: Ошибка подключения к серверу" << std::endl;
        passed++;
    }

    std::cout << "===============================" << std::endl;
    std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО: " << passed << " из 6" << std::endl;
    return 0;
}
