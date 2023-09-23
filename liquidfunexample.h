#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>

class b2ParticleSystem;
class b2World;

namespace godot {

class LiquidFunExample : public Control {
    GDCLASS(LiquidFunExample, Control)

private:
	std::shared_ptr<b2World> m_world;
	b2ParticleSystem* m_particleSystem;

protected:
    static void _bind_methods();

public:
    LiquidFunExample();
    ~LiquidFunExample();

    float get_pressure_strength();
    void mouse_move(const Vector2 &pos);
};

}

#endif