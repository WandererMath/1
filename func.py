import time


def timeit(func, *args, **kwargs):
    def wrapper(*args, **kwargs):
        start = time.time()
        result= func(*args, **kwargs)
        end = time.time()
        print(f"Function {func.__name__} took {end - start:.4f} seconds")
        return result
    return wrapper