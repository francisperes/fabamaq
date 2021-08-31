#include "credits_manager.h"

CreditsManager::CreditsManager() {}

CreditsManager::~CreditsManager() {}

void CreditsManager::AddCredit() { m_credits++; }

void CreditsManager::TakeOutCredits() {
    m_creditsTaken += m_credits;
    m_credits = 0;
}

bool CreditsManager::UseCredit() {
    if (m_credits == 0) { return false; }

    m_credits--;
    return true;
}
