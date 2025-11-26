# server.py
from aiohttp import web
import asyncio

async def handle(request):
    # Get query param 'x'
    x = request.rel_url.query.get("x")
    if x is None:
        return web.json_response({"error": 'Missing query parameter "x"'}, status=400)

    # Convert to number
    try:
        val = float(x)
    except ValueError:
        return web.json_response({"error": '"x" must be a number'}, status=400)

    # 1 second non-blocking delay
    await asyncio.sleep(1)

    # compute square and return integer if appropriate
    result = val * val
    if result.is_integer():
        result = int(result)

    return web.json_response({"x": val, "result": result})

app = web.Application()
app.router.add_get("/", handle)

def start_server():
    web.run_app(app, host="0.0.0.0", port=8000)

if __name__ == "__main__":
    web.run_app(app, host="0.0.0.0", port=8000)
