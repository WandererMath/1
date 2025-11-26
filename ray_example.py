import ray
import time

ray.init() 

@ray.remote
def slow_square(x):
    time.sleep(1)
    return x * x

if __name__ == "__main__":
    tasks = [slow_square.remote(i) for i in range(100)]
    results = ray.get(tasks)
    print("Results:", results)

ray.shutdown()
