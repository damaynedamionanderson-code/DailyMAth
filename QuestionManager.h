#ifndef QUESTION_MANAGER_H
#define QUESTION_MANAGER_H

#include <string>
#include <vector>

// Structure to hold individual question data
struct MathQuestion {
    int id;
    std::string difficulty;
    std::string text;      // The actual question (e.g., "5 + 5")
    std::string answer;    // The correct answer
};

class QuestionManager {
public:
    // Default constructor
    QuestionManager() = default;

    // Loads questions from a CSV file. Returns true if successful.
    bool loadQuestions(const std::string& filename);

    // Returns the current day's question. 
    // Returns a dummy question if none are loaded.
    MathQuestion getTodayQuestion();

private:
    // Stores all questions loaded from the CSV
    std::vector<MathQuestion> allQuestions;
};

#endif // QUESTION_MANAGER_H