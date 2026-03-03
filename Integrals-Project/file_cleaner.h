#pragma once

#include <fstream>
#include <iosfwd>
#include <iostream>

class file_cleaner {
public:
    static void replace_all(const std::string &filename , char oldChar, char newChar) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Błąd otwierania pliku: " << filename << std::endl;
            return;
        }

        // Odczytaj cały plik do stringa
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();

        // Zamień znaki
        for (char& c : content) {
            if (c == oldChar) {
                c = newChar;
            }
        }

        // Zapisz z powrotem do tego samego pliku
        std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            std::cerr << "Błąd otwierania pliku do zapisu: " << filename << std::endl;
            return;
        }

        outFile << content;
        outFile.close();
        std::cout << filename <<" - success"<< std::endl;

    }
};
