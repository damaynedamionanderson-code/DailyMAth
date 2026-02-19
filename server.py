from flask import Flask, request, jsonify
from flask_cors import CORS
import csv
import os

app = Flask(__name__)

# UPDATED: This tells Flask to allow requests coming ONLY from your live domain
CORS(app, resources={r"/*": {"origins": ["https://daily-math.org", "https://www.daily-math.org"]}})

@app.route('/subscribe', methods=['POST'])
def subscribe():
    # Force=True ensures it reads the JSON even if the content-type header is slightly off
    data = request.get_json(force=True)
    email = data.get('email')
    
    if email:
        # LOGGING: This will show the email in your VS Code terminal as soon as it's sent
        print(f"New subscription request: {email}")
        
        # Save to your local file in 'append' mode
        with open("subscribers.txt", "a") as f:
            f.write(email + "\n")
        return jsonify({"success": True}), 200
        
    return jsonify({"success": False}), 400

@app.route('/get-questions', methods=['GET'])
def get_questions():
    questions = []
    # Make sure this path is accurate for your Linux environment
    csv_path = "/home/damaynedamionanderson/code/DailyMathApp/Backend/questions.csv"
    
    if not os.path.exists(csv_path):
        return jsonify({"error": "CSV file not found"}), 404

    with open(csv_path, mode='r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            questions.append(row)
    return jsonify(questions)

if __name__ == '__main__':
    # Running on 18080 to match your ngrok configuration
    app.run(port=18080, debug=True)
