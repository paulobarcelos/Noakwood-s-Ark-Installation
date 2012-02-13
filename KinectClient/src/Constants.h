#pragma once

enum ShapeType{
    UNDEFINED_SHAPE = 0,
    WATER
};

class Data {
public:
    Data(){
        type = UNDEFINED_SHAPE;
        label = -1;
    };
    int	 label;
    ShapeType type;
};