import os
import subprocess
import time
import webbrowser
from http.server import HTTPServer, SimpleHTTPRequestHandler

# Paths and configurations
BACKEND_BUILD_COMMAND = "g++ -std=c++11 main.cpp NurseList.cpp CSVParser.cpp NurseFunctions.cpp -o nurse_list_program"
BACKEND_EXECUTABLE = "./nurse_list_program"
FRONTEND_DIRECTORY = "path/to/EmployeeSchedulerCMPSC483W/frontend/UI"
FRONTEND_FILE = "DisplayUI.html"
FRONTEND_PORT = 8000

# Step 1: Build the back-end executable
def build_backend():
    print("Building back-end...")
    try:
        subprocess.run(BACKEND_BUILD_COMMAND, shell=True, check=True)
        print("Back-end build completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Error building back-end: {e}")
        exit(1)

# Step 2: Run the back-end program
def run_backend():
    print("Running back-end...")
    try:
        subprocess.run(BACKEND_EXECUTABLE, shell=True, check=True)
        print("Back-end execution completed.")
    except subprocess.CalledProcessError as e:
        print(f"Error running back-end: {e}")
        exit(1)

# Step 3: Serve the front-end
def serve_frontend():
    os.chdir(FRONTEND_DIRECTORY)
    server = HTTPServer(("localhost", FRONTEND_PORT), SimpleHTTPRequestHandler)
    print(f"Serving front-end at http://localhost:{FRONTEND_PORT}/{FRONTEND_FILE}")
    webbrowser.open(f"http://localhost:{FRONTEND_PORT}/{FRONTEND_FILE}")
    server.serve_forever()

# Main function to coordinate steps
if __name__ == "__main__":
    # Step 1: Build back-end
    build_backend()

    # Step 2: Run back-end
    run_backend()

    # Step 3: Serve front-end
    try:
        serve_frontend()
    except KeyboardInterrupt:
        print("\nServer stopped by user.")
