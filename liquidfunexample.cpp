#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Particle/b2ParticleGroup.h>
#include <Box2D/Particle/b2ParticleSystem.h>

using namespace godot;

void LiquidFunExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("step"), &LiquidFunExample::step);
    ClassDB::bind_method(D_METHOD("mouse_move"), &LiquidFunExample::mouse_move);
    ClassDB::bind_method(D_METHOD("get_particle_count"), &LiquidFunExample::get_particle_count);
    ClassDB::bind_method(D_METHOD("set_particle_flags"), &LiquidFunExample::set_particle_flags);
    ClassDB::bind_method(D_METHOD("set_group_flags"), &LiquidFunExample::set_group_flags);
    ClassDB::bind_method(D_METHOD("set_color"), &LiquidFunExample::set_color);
    ClassDB::bind_method(D_METHOD("reset_last_group"), &LiquidFunExample::reset_last_group);
}

LiquidFunExample::LiquidFunExample() {
    const b2ParticleSystemDef particleSystemDef;
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = std::make_shared<b2World>(gravity);
    m_particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.05f);
}

void LiquidFunExample::mouse_move(const Vector2 &pos) {
    b2CircleShape shape;
    shape.m_p = b2Vec2(pos.x, pos.y);
    shape.m_radius = 0.2f;
    b2Transform xf;
    xf.SetIdentity();

    m_particleSystem->DestroyParticlesInShape(shape, xf);

    b2ParticleGroupDef pd;
    pd.shape = &shape;
    pd.flags = m_particleFlags;
    pd.groupFlags = m_groupFlags;
    pd.color = b2ParticleColor(m_color.get_r8(), m_color.get_g8(), m_color.get_b8(), m_color.get_a8());
    pd.group = m_lastGroup;
    m_lastGroup = m_particleSystem->CreateParticleGroup(pd);
}

void LiquidFunExample::_draw() {
    auto particleCount = m_particleSystem->GetParticleCount();
    auto positionBuffer = m_particleSystem->GetPositionBuffer();
    auto colorBuffer = m_particleSystem->GetColorBuffer();

    auto size = get_size();
    auto ratio = size.x / size.y;
    auto extents = Vector2(25, 25 / ratio) * 0.1;
    auto settings_view_center = Vector2(0, 2);

    for (int i = 0; i < particleCount; ++i) {
        auto point = positionBuffer[i];
        auto color = colorBuffer[i].GetColor();
        auto lower = settings_view_center - extents;
        auto upper = settings_view_center + extents;
        auto u = (point.x - lower.x) / (upper.x - lower.x);
        auto v = (point.y - lower.y) / (upper.y - lower.y);
        auto center = Vector2(u * size.x, (1 - v) * size.y);
        draw_circle(center, 10, Color(color.r, color.g, color.b));
    }
}

void LiquidFunExample::step(float delta) {
    m_world->Step(delta, 8, 3, 3);
}

int LiquidFunExample::get_particle_count() {
    return m_particleSystem->GetParticleCount();
}
