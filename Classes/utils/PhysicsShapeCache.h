#ifndef PHYSICS_SHAPE_CACHE_H
#define PHYSICS_SHAPE_CACHE_H

#include "cocos2d.h"
#include <unordered_map>

class PhysicsShapeCache {
public:
    // Singleton
    static PhysicsShapeCache* getInstance() {
        static PhysicsShapeCache instance;
        return &instance;
    }

    // Load shapes from a plist file
    bool addShapesWithFile(const std::string& plist);

    // Create a PhysicsBody for a specific shape
    cocos2d::PhysicsBody* createBody(const std::string& shapeName, const cocos2d::Size& originalSize, const cocos2d::Size& scaledSize);
    void resizeBody(cocos2d::PhysicsBody* physicsBody, const std::string& shapeName, const cocos2d::Size& originalSize, float scaleFactor);
    // Clear all loaded shapes
    void clear();
    cocos2d::PhysicsBody* createBodyFromPlist(const std::string& plist, const std::string& shapeName, const cocos2d::Size& originalSize, const cocos2d::Size& scaledSize);

private:
    PhysicsShapeCache() = default;
    ~PhysicsShapeCache() = default;
    PhysicsShapeCache(const PhysicsShapeCache&) = delete;
    PhysicsShapeCache& operator=(const PhysicsShapeCache&) = delete;

    struct FixtureData {
        float density;
        float friction;
        float restitution;
        bool isSensor;
        std::vector<std::vector<cocos2d::Vec2>> polygons;
    };

    struct BodyData {
        cocos2d::Vec2 anchorPoint;
        std::vector<FixtureData> fixtures;
    };

    std::unordered_map<std::string, BodyData> shapeObjects;

    // Helper function to parse Vec2 from a string
    cocos2d::Vec2 parseVec2(const std::string& pointStr);
};

#endif // PHYSICS_SHAPE_CACHE_H
