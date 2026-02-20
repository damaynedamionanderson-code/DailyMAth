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
        if (line.empty() || line == "\r") continue; 

        std::stringstream ss(line);
        std::string idStr, diff, ques, ans;

        // Use a more careful extraction
        if (!std::getline(ss, idStr, ',')) continue;
        if (!std::getline(ss, diff, ',')) continue;
        if (!std::getline(ss, ques, ',')) continue;
        if (!std::getline(ss, ans, ',')) continue;

        // Clean idStr of any non-numeric characters (like quotes or spaces)
        idStr.erase(std::remove_if(idStr.begin(), idStr.end(), [](char c) {
            return !std::isdigit(c);
        }), idStr.end());

        try {
            if (!idStr.empty()) {
                MathQuestion mq;
                mq.id = std::stoi(idStr);
                mq.difficulty = diff;
                mq.text = ques;   
                mq.answer = ans;
                allQuestions.push_back(mq);
            } else {
                // This helps us debug if the first column isn't the ID
                std::cerr << "⚠️ Row skipped: First column was not a number (Found: " << diff << " instead)" << std::endl;
            }
        } catch (const std::exception& e) {
            continue;
        }
    }
    
    if (allQuestions.empty()) {
        std::cerr << "❌ Error: 0 questions loaded. Check if your CSV is comma-separated." << std::endl;
    } else {
        std::cout << "✅ Successfully loaded " << allQuestions.size() << " questions." << std::endl;
    }
    return !allQuestions.empty();
}

MathQuestion QuestionManager::getTodayQuestion() {
    if (allQuestions.empty()) {
        MathQuestion emptyQ = {0, "None", "No questions loaded", ""};
        return emptyQ;
    }
    return allQuestions[0];
}