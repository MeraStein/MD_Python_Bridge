print("Success! Python is running through the C Bridge.")

import os
import sys

print(f"--- Python Bridge Logic Test ---")
print(f"Current Directory: {os.getcwd()}")

try:
    import requests
    print("SUCCESS: 'requests' library found and loaded.")
except ImportError:
    print("ERROR: 'requests' library not found.")

try:
    print("\nAttempting to import a non-existent library...")
    import magicdata_special_lib 
except ImportError as e:
    print(f"CAUGHT EXPECTED ERROR: {e}")
    print("The Bridge identified the missing module correctly.")

print("\n--- Test Finished Successfully ---")

