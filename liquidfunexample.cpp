#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <Box2D/Particle/b2ParticleSystem.h>

using namespace godot;

void LiquidFunExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_pressure_strength"), &LiquidFunExample::get_pressure_strength);
}

LiquidFunExample::LiquidFunExample() {
    // Initialize any variables here.
    time_passed = 0.0;

	const b2ParticleSystemDef particleSystemDef;
}

LiquidFunExample::~LiquidFunExample() {
    // Add your cleanup here.
}

float LiquidFunExample::get_pressure_strength() {
	const b2ParticleSystemDef particleSystemDef;

    return particleSystemDef.pressureStrength;
}
