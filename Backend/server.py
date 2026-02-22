from flask import Flask, request, jsonify
from flask_cors import CORS
import csv
import os

app = Flask(__name__)
CORS(app)

@app.route('/subscribe', methods=['POST'])
def subscribe():
    data = request.get_json(force=True)
    email = data.get('email')
    if email:
        with open("subscribers.txt", "a") as f:
            f.write(email + "\n")
        return jsonify({"success": True}), 200
    return jsonify({"success": False}), 400

@app.route('/get-questions', methods=['GET'])
def get_questions():
    questions = []
    # Make sure this matches your file name exactly
    csv_path = "/home/damaynedamionanderson/code/DailyMathApp/Backend/questions.csv"
    
    if not os.path.exists(csv_path):
        return jsonify({"error": "CSV file not found"}), 404

    with open(csv_path, mode='r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            questions.append(row)
    return jsonify(questions)

if __name__ == '__main__':
    app.run(port=18080, debug=True)