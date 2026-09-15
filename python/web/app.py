from flask import Flask, send_from_directory
import os

# Folder that will be served
HTTP_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "http")

app = Flask(__name__)


@app.route("/", defaults={"path": "index.html"})
@app.route("/<path:path>")
def serve_file(path):
    """Serve any file from the ./http folder. Defaults to index.html at root."""
    return send_from_directory(HTTP_DIR, path)


if __name__ == "__main__":
    # host=0.0.0.0 makes it reachable from other devices on your network
    app.run(host="0.0.0.0", port=5000, debug=True)
