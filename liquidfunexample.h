#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>

class b2World;

namespace godot {

class LiquidFunExample : public Control {
    GDCLASS(LiquidFunExample, Control)

private:
	std::shared_ptr<b2World> m_world;

protected:
    static void _bind_methods();

public:
    LiquidFunExample();
    ~LiquidFunExample();

    float get_pressure_strength();
};

}

#endif