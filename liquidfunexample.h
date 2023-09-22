#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>

namespace godot {

class LiquidFunExample : public Control {
    GDCLASS(LiquidFunExample, Control)

private:
    double time_passed;

protected:
    static void _bind_methods();

public:
    LiquidFunExample();
    ~LiquidFunExample();

    float get_pressure_strength();
};

}

#endif