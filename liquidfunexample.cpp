#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Particle/b2ParticleGroup.h>
#include <Box2D/Particle/b2ParticleSystem.h>

using namespace godot;

void LiquidFunExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_pressure_strength"), &LiquidFunExample::get_pressure_strength);
    ClassDB::bind_method(D_METHOD("mouse_move"), &LiquidFunExample::mouse_move);
}

LiquidFunExample::LiquidFunExample() {
	const b2ParticleSystemDef particleSystemDef;
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = std::make_shared<b2World>(gravity);
	m_particleSystem = m_world->CreateParticleSystem(&particleSystemDef);
}

LiquidFunExample::~LiquidFunExample() {
    // Add your cleanup here.
}

float LiquidFunExample::get_pressure_strength() {
	const b2ParticleSystemDef particleSystemDef;

    return particleSystemDef.pressureStrength;
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
    m_particleSystem->CreateParticleGroup(pd);
}
