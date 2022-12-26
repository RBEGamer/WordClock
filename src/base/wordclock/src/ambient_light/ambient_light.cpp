#include "ambient_light.h"

ambient_light::ambient_light(){

}
ambient_light::~ambient_light(){

}
    
int ambient_light::get_brightness(){
    return (WORDCLOC_BRIGHTNESS_MODE_AUTO_MIN+WORDCLOC_BRIGHTNESS_MODE_AUTO_MAX)/2;
}

int ambient_light::get_average_brightness(){
    return ambient_light::get_brightness();
}

void ambient_light::init(){

}

