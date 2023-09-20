#include "liquidfunexample.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void LiquidFunExample::_bind_methods() {
}

LiquidFunExample::LiquidFunExample() {
    // Initialize any variables here.
    time_passed = 0.0;
}

LiquidFunExample::~LiquidFunExample() {
    // Add your cleanup here.
}

void LiquidFunExample::_process(double delta) {
    time_passed += delta;
}