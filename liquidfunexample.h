#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>

class b2ParticleGroup;
class b2ParticleSystem;
class b2World;
struct b2Vec2;

namespace godot {

class LiquidFunExample : public Control {
    GDCLASS(LiquidFunExample, Control)

private:
    std::shared_ptr<b2World> m_world;
    b2ParticleSystem* m_particleSystem = nullptr;

	b2ParticleGroup* m_lastGroup = nullptr;
    uint32_t m_particleFlags = 0;
    uint32_t m_groupFlags = 0;
    Color m_color;

public:
    LiquidFunExample();

    int get_particle_count();
    void mouse_move(const Vector2 &pos);
    void _ready() override;
    void _draw() override;
    void step(float delta);
    void set_gravity(const Vector2 &gravity);

    void set_particle_flags(int flags) {
        m_particleFlags = flags;
    }
    
    void set_color(const Color &color) {
        m_color = color;
    }
    
    void set_group_flags(int flags) {
        m_groupFlags = flags;
    }

    void reset_last_group() {
        m_lastGroup = nullptr;
    }

protected:
    static void _bind_methods();
    Vector2 convert_screen_to_world(const Vector2 &screen_position);
    Vector2 convert_world_to_screen(const b2Vec2 &world_position);
};

}

#endif