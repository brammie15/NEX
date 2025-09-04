#include "Object.h"

#include <cassert>
#include <iostream>

Object::~Object() {
    if (!m_BeingDestroyed) {
        assert(false && "Objects destructor called before destroy");
    }
    std::cout << "Object Destroyed: " << m_Name << std::endl;
}

void Object::Destroy() {
    std::cout << "Marked Object for destruction: " << m_Name << std::endl;
    m_BeingDestroyed = true;
}

Object::Object(std::string name): m_Name(std::move(name)) {
}