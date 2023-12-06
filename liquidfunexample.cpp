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
    gravity.Set(0.0f, -10.0f);
    m_world = std::make_shared<b2World>(gravity);
    m_particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.05f);
}

void LiquidFunExample::_ready() {
    auto size = get_size();
    auto h = 2 * size.y / size.x;
    b2BodyDef bd;
    b2Body* ground = m_world->CreateBody(&bd);

    {
        b2PolygonShape shape;
        const b2Vec2 vertices[4] = {
            b2Vec2(-4, -h-2),
            b2Vec2(4, -h-2),
            b2Vec2(4, -h),
            b2Vec2(-4, -h)};
        shape.Set(vertices, 4);
        ground->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const b2Vec2 vertices[4] = {
            b2Vec2(-4, -h-2),
            b2Vec2(-2, -h-2),
            b2Vec2(-2, h+2),
            b2Vec2(-4, h+2)};
        shape.Set(vertices, 4);
        ground->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const b2Vec2 vertices[4] = {
            b2Vec2(2, -h-2),
            b2Vec2(4, -h-2),
            b2Vec2(4, h+2),
            b2Vec2(2, h+2)};
        shape.Set(vertices, 4);
        ground->CreateFixture(&shape, 0.0f);
    }

    {
        b2PolygonShape shape;
        const b2Vec2 vertices[4] = {
            b2Vec2(-4, h),
            b2Vec2(4, h),
            b2Vec2(4, h+2),
            b2Vec2(-4, h+2)};
        shape.Set(vertices, 4);
        ground->CreateFixture(&shape, 0.0f);
    }
}

Vector2 LiquidFunExample::convert_screen_to_world(const Vector2 &position) {
    auto size = get_size();
    auto ratio = size.x / size.y;
    auto extents = Vector2(2, 2 / ratio);

	auto u = position.x / size.x;
	auto v = (size.y - position.y) / size.y;
	
	auto lower = -extents;
	auto upper = extents;
	
	Vector2 p;
	p.x = (1 - u) * lower.x + u * upper.x;
	p.y = (1 - v) * lower.y + v * upper.y;
	return p;
}

Vector2 LiquidFunExample::convert_world_to_screen(const b2Vec2 &world_position) {
    auto size = get_size();
    auto ratio = size.x / size.y;
    auto extents = Vector2(2, 2 / ratio);

    auto point = world_position;
    auto lower = -extents;
    auto upper = extents;
    auto u = (point.x - lower.x) / (upper.x - lower.x);
    auto v = (point.y - lower.y) / (upper.y - lower.y);
    return Vector2(u * size.x, (1 - v) * size.y);
}

void LiquidFunExample::mouse_move(const Vector2 &position) {
    auto pos = convert_screen_to_world(position);
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
