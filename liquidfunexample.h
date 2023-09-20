#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>

namespace godot {

class LiquidFunExample : public Object {
    GDCLASS(LiquidFunExample, Object)

private:
    double time_passed;

protected:
    static void _bind_methods();

public:
    LiquidFunExample();
    ~LiquidFunExample();

    void _process(double delta);
};

}

#endif