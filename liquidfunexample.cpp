#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Particle/b2ParticleGroup.h>
#include <Box2D/Particle/b2ParticleSystem.h>

using namespace godot;

class DestructionListener : public b2DestructionListener {
    LiquidFunExample* m_app;
public:
    DestructionListener(LiquidFunExample* app) : m_app(app) {
    }

    void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
    void SayGoodbye(b2Joint* joint) { B2_NOT_USED(joint); }
    void SayGoodbye(b2ParticleGroup* group) {
        m_app->particle_group_destroyed(group);
    }
};

void LiquidFunExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("step"), &LiquidFunExample::step);
    ClassDB::bind_method(D_METHOD("mouse_move"), &LiquidFunExample::mouse_move);
    ClassDB::bind_method(D_METHOD("get_particle_count"), &LiquidFunExample::get_particle_count);
    ClassDB::bind_method(D_METHOD("set_particle_flags"), &LiquidFunExample::set_particle_flags);
    ClassDB::bind_method(D_METHOD("set_group_flags"), &LiquidFunExample::set_group_flags);
    ClassDB::bind_method(D_METHOD("set_color"), &LiquidFunExample::set_color);
    ClassDB::bind_method(D_METHOD("reset_last_group"), &LiquidFunExample::reset_last_group);
    ClassDB::bind_method(D_METHOD("set_gravity"), &LiquidFunExample::set_gravity);
    ClassDB::bind_method(D_METHOD("set_drawing"), &LiquidFunExample::set_drawing);
    ClassDB::bind_method(D_METHOD("set_mouse_world"), &LiquidFunExample::set_mouse_world);
    ClassDB::bind_method(D_METHOD("set_mouse_tracing"), &LiquidFunExample::set_mouse_tracing);
    ClassDB::bind_method(D_METHOD("set_mouse_tracer_position"), &LiquidFunExample::set_mouse_tracer_position);
    ClassDB::bind_method(D_METHOD("set_mouse_tracer_velocity"), &LiquidFunExample::set_mouse_tracer_velocity);
}

LiquidFunExample::LiquidFunExample() {
    const b2ParticleSystemDef particleSystemDef;
    m_world = std::make_shared<b2World>(0.0f, 10.0f);
    m_particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.05f);
    m_destructionListener = std::make_shared<DestructionListener>(this);
    m_world->SetDestructionListener(m_destructionListener.get());
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
    if (m_drawing) {
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
}

void LiquidFunExample::_draw() {
    auto particleCount = m_particleSystem->GetParticleCount();
    auto positionBuffer = m_particleSystem->GetPositionBuffer();
    auto colorBuffer = m_particleSystem->GetColorBuffer();

    for (int i = 0; i < particleCount; ++i) {
        auto color = colorBuffer[i].GetColor();
        draw_circle(convert_world_to_screen(positionBuffer[i]), 10, Color(color.r, color.g, color.b));
    }
}

class QueryCallback2 : public b2QueryCallback
{
public:
	QueryCallback2(b2ParticleSystem* particleSystem,
				   const b2Shape* shape, const b2Vec2& velocity)
	{
		m_particleSystem = particleSystem;
		m_shape = shape;
		m_velocity = velocity;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		B2_NOT_USED(fixture);
		return false;
	}

	bool ReportParticle(const b2ParticleSystem* particleSystem, int32 index)
	{
		if (particleSystem != m_particleSystem)
			return false;

		b2Transform xf;
		xf.SetIdentity();
		b2Vec2 p = m_particleSystem->GetPositionBuffer()[index];
		if (m_shape->TestPoint(xf, p))
		{
			b2Vec2& v = m_particleSystem->GetVelocityBuffer()[index];
			v = m_velocity;
		}
		return true;
	}

	b2ParticleSystem* m_particleSystem;
	const b2Shape* m_shape;
	b2Vec2 m_velocity;
};

void LiquidFunExample::step(float timeStep) {
    if (m_particleSystem->GetAllParticleFlags() & b2_zombieParticle) {
        split_particle_groups();
    }
    m_world->Step(timeStep, 8, 3, 3);

    if (m_mouseTracing) {
        float32 delay = 0.1f;
        b2Vec2 acceleration = 2 / delay * (1 / delay * (m_mouseWorld - m_mouseTracerPosition) - m_mouseTracerVelocity);
        m_mouseTracerVelocity += timeStep * acceleration;
        m_mouseTracerPosition += timeStep * m_mouseTracerVelocity;
        b2CircleShape shape;
        shape.m_p = m_mouseTracerPosition;
        shape.m_radius = 0.2f;
        QueryCallback2 callback(m_particleSystem, &shape, m_mouseTracerVelocity);
        b2AABB aabb;
        b2Transform xf;
        xf.SetIdentity();
        shape.ComputeAABB(&aabb, xf, 0);
        m_world->QueryAABB(&callback, aabb);
    }
}

int LiquidFunExample::get_particle_count() {
    return m_particleSystem->GetParticleCount();
}

void LiquidFunExample::set_gravity(float x, float y) {
    m_world->SetGravity(x, -y);
}

void LiquidFunExample::particle_group_destroyed(b2ParticleGroup* group) {
    if (group == m_lastGroup)
    {
        m_lastGroup = nullptr;
    }
}

void LiquidFunExample::split_particle_groups() {
    for (b2ParticleGroup* group = m_particleSystem->GetParticleGroupList(); group; group = group->GetNext()) {
        if (group != m_lastGroup && (group->GetGroupFlags() & b2_rigidParticleGroup) && (group->GetAllParticleFlags() & b2_zombieParticle)) {
            // Split a rigid particle group which may be disconnected
            // by destroying particles.
            m_particleSystem->SplitParticleGroup(group);
        }
    }
}

