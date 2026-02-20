#include "QuestionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool QuestionManager::loadQuestions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "❌ SYSTEM ERROR: Cannot find " << filename << " in this folder!" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header row

    allQuestions.clear();
    while (std::getline(file, line)) {
        if (line.empty() || line == "\r") continue;

        std::stringstream ss(line);
        std::string col1, col2, col3, col4;

        // Extract 4 columns (assuming standard CSV format)
        std::getline(ss, col1, ',');
        std::getline(ss, col2, ',');
        std::getline(ss, col3, ',');
        std::getline(ss, col4, ',');

        // --- THE FIX: Let's find which column is the actual ID ---
        // Based on your terminal, 'col1' is the Question (LaTeX). 
        // We will assume 'col3' is the ID.
        std::string idStr = col3; 
        std::string difficulty = col2;
        std::string questionText = col1;
        std::string answer = col4;

        // Clean ID of spaces/quotes
        idStr.erase(std::remove_if(idStr.begin(), idStr.end(), [](char c) {
            return !std::isdigit(c);
        }), idStr.end());

        try {
            if (!idStr.empty()) {
                MathQuestion mq;
                mq.id = std::stoi(idStr);
                mq.difficulty = difficulty;
                mq.text = questionText;
                mq.answer = answer;
                allQuestions.push_back(mq);
            }
        } catch (...) { continue; }
    }

    if (allQuestions.empty()) {
        std::cerr << "❌ 0 questions loaded. Your CSV format is still not matching the code." << std::endl;
    } else {
        std::cout << "🚀 SUCCESS! Loaded " << allQuestions.size() << " questions." << std::endl;
        std::cout << "Today's Feature: " << allQuestions[0].text << std::endl;
    }
    return !allQuestions.empty();
}

MathQuestion QuestionManager::getTodayQuestion() {
    if (allQuestions.empty()) return {0, "None", "Empty", ""};
    return allQuestions[0];
}