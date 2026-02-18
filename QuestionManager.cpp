#include "QuestionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

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
        if (line.empty()) continue; 
        std::stringstream ss(line);
        std::string idStr, diff, ques, ans;

        std::getline(ss, idStr, ',');
        std::getline(ss, diff, ',');
        std::getline(ss, ques, ',');
        std::getline(ss, ans, ',');

        MathQuestion mq;
        mq.id = (idStr.empty()) ? 0 : std::stoi(idStr);
        mq.difficulty = diff;
        mq.text = ques;   
        mq.answer = ans;

        allQuestions.push_back(mq);
    }
    return !allQuestions.empty();
}

// THIS IS THE PART YOU WERE MISSING:
MathQuestion QuestionManager::getTodayQuestion() {
    if (allQuestions.empty()) {
        MathQuestion emptyQ;
        emptyQ.id = 0;
        emptyQ.difficulty = "None";
        emptyQ.text = "No questions loaded";
        emptyQ.answer = "";
        return emptyQ;
    }
    // For now, it just returns the first question in the list
    return allQuestions[0];
}    