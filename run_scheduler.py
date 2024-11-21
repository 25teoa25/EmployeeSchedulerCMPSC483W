import os
import subprocess
import webbrowser
from http.server import HTTPServer, SimpleHTTPRequestHandler

# Configuration
PROJECT_DIRECTORY = "/Users/alexteo/EmployeeSchedulerCMPSC483W"  # Root project directory, !!!CHANGE THIS!!!
BACKEND_DIRECTORY = os.path.join(PROJECT_DIRECTORY, "DataStructure/LinkedListDS")  # Back-end directory
BACKEND_BUILD_COMMAND = "g++ -std=c++11 main.cpp NurseList.cpp CSVParser.cpp NurseFunctions.cpp -o nurse_list_program"
BACKEND_EXECUTABLE = "./nurse_list_program"
FRONTEND_FILE = "UI/DisplayUI.html"  # Adjusted to reflect the UI subdirectory
PORT = 8000

# Step 1: Build the back end
def build_backend():
    print("Building back-end...")
    try:
        subprocess.run(BACKEND_BUILD_COMMAND, shell=True, cwd=BACKEND_DIRECTORY, check=True)
        print("Back-end built successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Error building back-end: {e}")
        exit(1)

# Step 2: Run the back end
def run_backend():
    print("Running back-end...")
    try:
        subprocess.run(BACKEND_EXECUTABLE, shell=True, cwd=BACKEND_DIRECTORY, check=True)
        print("Back-end execution completed. JSON output generated.")
    except subprocess.CalledProcessError as e:
        print(f"Error running back-end: {e}")
        exit(1)

# Step 3: Serve the project
def serve_project():
    os.chdir(PROJECT_DIRECTORY)
    server = HTTPServer(("localhost", PORT), SimpleHTTPRequestHandler)
    print(f"Serving project at http://localhost:{PORT}/{FRONTEND_FILE}")
    webbrowser.open(f"http://localhost:{PORT}/{FRONTEND_FILE}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nServer stopped.")

# Main script
if __name__ == "__main__":
    # Step 1: Build the back end
    build_backend()

    # Step 2: Run the back end
    run_backend()

    # Step 3: Serve the entire project
    serve_project()
