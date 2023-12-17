#ifndef LIQUIDFUNEXAMPLE_H
#define LIQUIDFUNEXAMPLE_H

#include <godot_cpp/classes/control.hpp>
#include <Box2D/Particle/b2Particle.h>

class b2DestructionListener;
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
    bool m_drawing = true;
    b2Vec2 m_mouseWorld = b2Vec2_zero;
    bool m_mouseTracing = false;
    b2Vec2 m_mouseTracerPosition = b2Vec2_zero;
    b2Vec2 m_mouseTracerVelocity = b2Vec2_zero;
    uint32_t m_particleFlags = 0;
    uint32_t m_groupFlags = 0;
    b2ParticleColor m_color;
    std::shared_ptr<b2DestructionListener> m_destructionListener;

public:
    LiquidFunExample();

    int get_particle_count();
    void mouse_move(const Vector2 &pos);
    void _ready() override;
    void _draw() override;
    void step(float delta);
    void set_gravity(float x, float y);
    void particle_group_destroyed(b2ParticleGroup* group);

    void set_particle_flags(int flags) {
        m_particleFlags = flags;
    }
    
    void set_color(const Color &color) {
        m_color.Set(color.get_r8(), color.get_g8(), color.get_b8(), color.get_a8());
    }
    
    void set_group_flags(int flags) {
        m_groupFlags = flags;
    }

    void set_drawing(bool drawing) {
        m_drawing = drawing;
    }

    void set_mouse_world(const Vector2 &mouseWorld) {
        m_mouseWorld = convert_screen_to_world(mouseWorld);
    }

    void set_mouse_tracing(bool mouseTracing) {
        m_mouseTracing = mouseTracing;
    }

    void set_mouse_tracer_position(const Vector2 &mouseTracerPosition) {
        m_mouseTracerPosition = convert_screen_to_world(mouseTracerPosition);
    }

    void set_mouse_tracer_velocity(const Vector2 &mouseTracerVelocity) {
        m_mouseTracerVelocity = convert_screen_to_world(mouseTracerVelocity);
    }

    void reset_last_group() {
        m_lastGroup = nullptr;
    }

protected:
    static void _bind_methods();
    b2Vec2 convert_screen_to_world(const Vector2 &screen_position);
    Vector2 convert_world_to_screen(const b2Vec2 &world_position);
    void split_particle_groups();
};

}

#endif