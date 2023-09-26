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
    b2ParticleSystem* m_particleSystem = nullptr;

protected:
    static void _bind_methods();

public:
    LiquidFunExample();

    int get_particle_count();
    void mouse_move(const Vector2 &pos);
    void _draw() override;
    void step(float delta);
};

}

#endif