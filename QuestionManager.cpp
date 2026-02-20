#include "QuestionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool QuestionManager::loadQuestions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "SYSTEM ERROR: Cannot open file at " << filename << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header

    allQuestions.clear();
    while (std::getline(file, line)) {
        // Skip truly empty lines or carriage returns (\r)
        if (line.empty() || line == "\r") continue; 

        std::stringstream ss(line);
        std::string idStr, diff, ques, ans;

        std::getline(ss, idStr, ',');
        std::getline(ss, diff, ',');
        std::getline(ss, ques, ',');
        std::getline(ss, ans, ',');

        // Remove potential hidden spaces or \r from idStr
        idStr.erase(std::remove(idStr.begin(), idStr.end(), ' '), idStr.end());
        idStr.erase(std::remove(idStr.begin(), idStr.end(), '\r'), idStr.end());

        try {
            if (!idStr.empty()) {
                MathQuestion mq;
                mq.id = std::stoi(idStr);
                mq.difficulty = diff;
                mq.text = ques;   
                mq.answer = ans;
                allQuestions.push_back(mq);
            }
        } catch (const std::exception& e) {
            std::cerr << "⚠️ Skipping row with invalid ID: [" << idStr << "]" << std::endl;
            continue;
        }
    }
    
    std::cout << "✅ Loaded " << allQuestions.size() << " questions successfully." << std::endl;
    return !allQuestions.empty();
} // <--- THIS BRACE WAS MISSING

MathQuestion QuestionManager::getTodayQuestion() {
    if (allQuestions.empty()) {
        MathQuestion emptyQ;
        emptyQ.id = 0;
        emptyQ.difficulty = "None";
        emptyQ.text = "No questions loaded";
        emptyQ.answer = "";
        return emptyQ;
    }
    // Returns the first question in the list
    return allQuestions[0];
}