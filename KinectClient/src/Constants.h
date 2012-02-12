#pragma once

enum ShapeType{
    UNDEFINED_SHAPE = 0,
    WATER = 1,
    SENSOR_IN = 2,
    SENSOR_OUT = 3
};

class Data {
public:
    Data(){
        type = UNDEFINED_SHAPE;
        isActive = false;
    };
    int	 label;
    ShapeType type;
    bool isActive;
};