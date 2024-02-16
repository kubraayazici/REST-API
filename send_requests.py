import requests
import concurrent.futures
import sys
import time

# Check if the correct number of command-line arguments are provided
if len(sys.argv) != 3:
    print("Usage: python script_name.py <port> <num_requests>")
    sys.exit(1)

# Extract command-line arguments
port = sys.argv[1]
num_requests = int(sys.argv[2])

# Define the URL of the server
url = f'http://localhost:{port}'

# Define the function to send a single request
def send_request(url):
    try:
        response = requests.get(url)
        print(f"Response from server: {response.text}")
    except Exception as e:
        print(f"Failed to send request: {e}")

# Record the start time
start_time = time.time()

# Send multiple requests asynchronously
with concurrent.futures.ThreadPoolExecutor() as executor:
    # Create a list of URLs to send requests to
    urls = [url] * num_requests

    # Submit requests to the executor
    executor.map(send_request, urls)

# Calculate the total time taken
end_time = time.time()
total_time = end_time - start_time
print(f"All requests sent in {total_time * 1000:.2f} milliseconds.")
