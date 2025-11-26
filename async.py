# client.py
import asyncio
import aiohttp
from func import timeit


URL = "http://localhost:8000/?x={}"



async def fetch(session, x):
    async with session.get(URL.format(x)) as resp:
        data = await resp.json()
        return data

async def tasks():
    async with aiohttp.ClientSession() as session:
        tasks = [fetch(session, i) for i in range(1, 101)]
        results = await asyncio.gather(*tasks)

    for r in results:
        print(r)

@timeit
def main():
    asyncio.run(tasks())


if __name__ == "__main__":
    # from server import start_server
    # from multiprocessing import Process
    # # Start the server in a separate process
    # server_process = Process(target=start_server)
    # server_process.start()
    
    main()

    # server_process.terminate()