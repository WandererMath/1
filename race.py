# Source - https://stackoverflow.com/q
# Posted by culebrón, modified by community. See post 'Timeline' for change history
# Retrieved 2025-11-23, License - CC BY-SA 4.0

import threading

ITERS = 100000
x = [0]

def worker():
    for _ in range(ITERS):
        x[0] += 1  # this line creates a race condition
        # because it takes a value, increments and then writes
        # some inrcements can be done together, and lost

def main():
    x[0] = 0  # you may use `global x` instead of this list trick too
    t1 = threading.Thread(target=worker)
    t2 = threading.Thread(target=worker)
    t1.start()
    t2.start()
    t1.join()
    t2.join()

for i in range(5):
    main()
    print(f'iteration {i}. expected x = {ITERS*2}, got {x[0]}')
