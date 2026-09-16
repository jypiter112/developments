import glob
import re
from PIL import Image

def numeric_key(filename):
    match = re.search(r'sivu(\d+)', filename)
    return int(match.group(1)) if match else 0

# Adjust the extension in the pattern if needed, e.g. 'sivu*.jpg'
files = sorted(glob.glob('sivu*'), key=numeric_key)

if not files:
    raise SystemExit("No files matching 'sivu*' found.")

print("Combining in this order:", files)

images = [Image.open(f).convert('RGB') for f in files]

first, rest = images[0], images[1:]
first.save('combined_output.pdf', save_all=True, append_images=rest)

print(f"Saved combined_output.pdf with {len(images)} pages.")
