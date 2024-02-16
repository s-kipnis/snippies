with open("/tmp/my_log.txt", "a") as f:
    # Writing data to a file
    f.write("xxxxx \n")

def print_any(sd: Any, mark: str = "---"):
    TO_FILE = "/tmp/sd.txt"
    with open(TO_FILE, "a") as f:
        f.write(f"{mark} '{sd}'\n")

def print_f(value: str) :
    TO_FILE = "/tmp/sd.txt"
    with open(TO_FILE, "a") as f:
        f.write(f"{value}\n")

