from flask import Flask, request, jsonify
from pymongo import MongoClient
import datetime

app = Flask(__name__)

# MongoDB connection
client = MongoClient("localhost", 27017)
db = client["C3"]
collection_bmp180 = db["bmp180"]

print("MongoDB connected:", client, db, collection_bmp180)

# -------------------------------
# POST: Receive BMP180 data
# -------------------------------
@app.route("/api/bmp180", methods=["POST"])
def write_bmp180_data():
    try:
        data = request.get_json()
        print("Received data:", data)

        # Add timestamp
        data["timestamp"] = datetime.datetime.now().isoformat()

        result = collection_bmp180.insert_one(data)

        return jsonify({
            "message": "BMP180 data stored successfully",
            "id": str(result.inserted_id)
        }), 201

    except Exception as e:
        return jsonify({"error": str(e)}), 500


# -------------------------------
# GET: Fetch BMP180 data
# -------------------------------
@app.route("/api/bmp180", methods=["GET"])
def get_bmp180_data():
    try:
        data = list(collection_bmp180.find({}, {"_id": 0}))
        return jsonify(data), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
