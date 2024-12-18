#ifndef PLAYERATTRIBUTES_H
#define PLAYERATTRIBUTES_H

class PlayerAttributes
{
private:
    int health;  // Player's health
    int ammo;    // Player's ammo


public:
    // Delete copy constructor and assignment operator
    PlayerAttributes(const PlayerAttributes&) = delete;
    PlayerAttributes& operator=(const PlayerAttributes&) = delete;
    PlayerAttributes(int initialHealth, int initialAmmo);
    // Static method to get the singleton instance
    static PlayerAttributes& getInstance();

    void TakeDamage(int damage);          // Reduces player's health
    bool IsDead() const;                  // Checks if player is dead
    int GetHealth() const;                // Returns the current health
    void SetHealth(int newHealth);        // Sets the player's health to a specific value

    void IncreaseHealth(int amount);      // Increases player's health
    void SetAmmo(int ammo);
    int GetAmmo() const;
};

#endif // PLAYERATTRIBUTES_H
