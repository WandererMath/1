import threading
import time

# A heavy CPU-bound function
def heavy_compute(n):
    total = 0
    for i in range(n):
        total += (i * i) % 97
    return total

def worker(name, n):
    print(f"[{name}] Starting...")
    start = time.time()
    result = heavy_compute(n)
    end = time.time()
    print(f"[{name}] Done in {end - start:.4f} s, result = {result}")

if __name__ == "__main__":
    # Create two threads running heavy CPU tasks
    t1 = threading.Thread(target=worker, args=("T1", 30_000_000))
    t2 = threading.Thread(target=worker, args=("T2", 30_000_000))

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print("Both threads completed.")
