#include "EmailSystem.h"
#include "QuestionManager.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    QuestionManager qm;
    EmailSystem emailer;

    // 1. Load the math database
    if (!qm.loadQuestions("questions.csv")) {
        std::cerr << "Error: questions.csv not found!" << std::endl;
        return 1;
    }

    // 2. Get today's featured problem
    MathQuestion today = qm.getTodayQuestion();
    
    // 3. Open the subscriber list
    std::ifstream subscriberFile("subscribers.txt");
    
    // Check if the file exists
    if (!subscriberFile.is_open()) {
        std::cerr << "Error: subscribers.txt not found. Running Python server first?" << std::endl;
        return 1;
    }

    // Check if the file is empty (your snippet)
    if (subscriberFile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "No subscribers found. Waiting for new sign-ups!" << std::endl;
        return 0;
    }

    std::string currentEmail;
    int count = 0;

    std::cout << "--- Starting Daily Blast ---" << std::endl;

    // 4. Loop through every email in the list
    while (std::getline(subscriberFile, currentEmail)) {
        // Simple check to ensure the line isn't empty or just whitespace
        if (currentEmail.length() > 5 && currentEmail.find('@') != std::string::npos) {
            if (emailer.sendUpdateNotification(currentEmail, today.difficulty, today.id)) {
                std::cout << "[SENT] " << currentEmail << std::endl;
                count++;
            } else {
                std::cerr << "[FAILED] " << currentEmail << std::endl;
            }
        }
    }

    std::cout << "--- Done! Sent to " << count << " users. ---" << std::endl;

    subscriberFile.close();
    return 0;
}