#include "DeltaTime.h"
#include <iostream>

double DeltaTime::GetFixedDeltaTime() const {
    return m_FixedDeltaTime;
}

double DeltaTime::GetDeltaTime() const {
    return m_DeltaTime;
}

std::chrono::nanoseconds DeltaTime::SleepDuration() const {
    constexpr auto msPerFrame = std::chrono::milliseconds(static_cast<int>(1000.f / FPS));
    return (m_PrevTime + msPerFrame - std::chrono::high_resolution_clock::now());
}

void DeltaTime::Update() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    m_DeltaTime = std::chrono::duration<double>(currentTime - m_PrevTime).count();
    m_PrevTime = currentTime;
}
