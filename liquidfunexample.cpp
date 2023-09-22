#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Particle/b2ParticleSystem.h>

using namespace godot;

void LiquidFunExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_pressure_strength"), &LiquidFunExample::get_pressure_strength);
}

LiquidFunExample::LiquidFunExample() {
	const b2ParticleSystemDef particleSystemDef;
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = std::make_shared<b2World>(gravity);
}

LiquidFunExample::~LiquidFunExample() {
    // Add your cleanup here.
}

float LiquidFunExample::get_pressure_strength() {
	const b2ParticleSystemDef particleSystemDef;

    return particleSystemDef.pressureStrength;
}
