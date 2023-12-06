#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Dynamics/b2Body.h>
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
    ClassDB::bind_method(D_METHOD("set_gravity"), &LiquidFunExample::set_gravity);
}

LiquidFunExample::LiquidFunExample() {
    const b2ParticleSystemDef particleSystemDef;
    b2Vec2 gravity;
    gravity.Set(0.0f, 10.0f);
    m_world = std::make_shared<b2World>(gravity);
    m_particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.05f);
    set_color(Color("AQUA"));
}

void LiquidFunExample::_ready() {
    b2BodyDef bd;
    b2PolygonShape shape;
    auto ground = m_world->CreateBody(&bd);
    auto size = get_size();
    auto h = 2 * size.y / size.x + 1;

    shape.SetAsBox(4, 1, 0, -h, 0);
    ground->CreateFixture(&shape, 0.0f);

    shape.SetAsBox(1, h, -3, 0, 0);
    ground->CreateFixture(&shape, 0.0f);

    shape.SetAsBox(1, h, 3, 0, 0);
    ground->CreateFixture(&shape, 0.0f);

    shape.SetAsBox(4, 1, 0, h, 0);
    ground->CreateFixture(&shape, 0.0f);
}

b2Vec2 LiquidFunExample::convert_screen_to_world(const Vector2 &position) {
    auto size = get_size();
	return b2Vec2(position.x * 4 / size.x - 2, (position.y * 4 - size.y * 2) / size.x);
}

Vector2 LiquidFunExample::convert_world_to_screen(const b2Vec2 &position) {
    auto size = get_size();
    return Vector2((position.x + 2) * size.x / 4, (position.y * size.x + size.y * 2) / 4);
}

void LiquidFunExample::mouse_move(const Vector2 &position) {
    b2CircleShape shape;
    shape.m_p = convert_screen_to_world(position);
    shape.m_radius = 0.2f;
    b2Transform xf;
    xf.SetIdentity();

    m_particleSystem->DestroyParticlesInShape(shape, xf);

    b2ParticleGroupDef pd;
    pd.shape = &shape;
    pd.flags = m_particleFlags;
    pd.groupFlags = m_groupFlags;
    pd.color = m_color;
    pd.group = m_lastGroup;
    m_lastGroup = m_particleSystem->CreateParticleGroup(pd);
}

void LiquidFunExample::_draw() {
    auto particleCount = m_particleSystem->GetParticleCount();
    auto positionBuffer = m_particleSystem->GetPositionBuffer();
    auto colorBuffer = m_particleSystem->GetColorBuffer();

    for (int i = 0; i < particleCount; ++i) {
        auto point = positionBuffer[i];
        auto center = convert_world_to_screen(point);
        auto color = colorBuffer[i].GetColor();
        draw_circle(center, 10, Color(color.r, color.g, color.b));
    }
}

void LiquidFunExample::step(float delta) {
    m_world->Step(delta, 8, 3, 3);
}

int LiquidFunExample::get_particle_count() {
    return m_particleSystem->GetParticleCount();
}

void LiquidFunExample::set_gravity(const Vector2 &gravity) {
    m_world->SetGravity(b2Vec2(gravity.x, gravity.y));
}
