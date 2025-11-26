#include <stdlib.h>
#include <iostream>
#include <typeinfo> 

class LargeObject{
public:
    int x;
    int y;
    int z;
};


void function_use_large_object(LargeObject* obj){
    // Function implementation that uses LargeObject
}

int main(){
    LargeObject* obj = new LargeObject();
    std::cout<<sizeof(*obj)<<std::endl;
    obj->x = 10;
    obj->y = 20;
    obj->z = 30;

    // Use the object...
    function_use_large_object(obj);

    delete obj; // Free the allocated memory
    return 0;
}
