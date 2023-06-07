with open("/tmp/my_log.txt", "a") as f:
    # Writing data to a file
    f.write("xxxxx \n")

TO_FILE = "/tmp/sd.txt"

def print_any(sd: Any, mark: str = "---") -> ServiceData:
    with open(TO_FILE, "a") as f:
        f.write(f"{mark} '{sd}'\n")
    return sd

def print_f(value: str) :
    with open(TO_FILE, "a") as f:
        f.write(f"{value}\n")

