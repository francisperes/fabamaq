#ifndef CREDITS_MANAGER_H
#define CREDITS_MANAGER_H

class CreditsManager {
public:
    CreditsManager();
    ~CreditsManager();

    void AddCredit();
    void TakeOutCredits();
    bool UseCredit();

    unsigned int GetCreditsIn() { return m_credits; }
    unsigned int GetCreditsOut() { return m_creditsTaken; }
private:
    unsigned int m_credits = 0;
    unsigned int m_creditsTaken = 0;
};

#endif
