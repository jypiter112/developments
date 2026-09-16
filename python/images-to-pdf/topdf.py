import glob
import re
import sys
from PIL import Image
 
if len(sys.argv) > 1:
    img_name = sys.argv[1]
else:
    print("Usage: python program.py <img_name>")
    print("  <img_name> is the filename prefix to match, e.g. 'sivu' will match files like sivu1.jpg, sivu2.jpg, ...")
    sys.exit(1)
 
def numeric_key(filename):
    match = re.search(re.escape(img_name) + r'(\d+)', filename)
    return int(match.group(1)) if match else 0
 
# Matches any file starting with img_name, regardless of extension
files = sorted(glob.glob(f'{img_name}*'), key=numeric_key)
 
if not files:
    raise SystemExit(f"No files matching '{img_name}*' found.")
 
print("Combining in this order:", files)
 
images = [Image.open(f).convert('RGB') for f in files]
 
first, rest = images[0], images[1:]
first.save('combined_output.pdf', save_all=True, append_images=rest)
 
print(f"Saved combined_output.pdf with {len(images)} pages.")
 

