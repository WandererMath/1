import ctypes


def ref_count(address):
    return ctypes.c_long.from_address(address).value

numbers = [1, 2, 3]
b = numbers
numbers_id = id(numbers)

print(numbers_id==id(b))
print(ref_count(numbers_id))