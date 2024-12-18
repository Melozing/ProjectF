#include "PhysicsShapeCache.h"
#include <unordered_set>
#include <algorithm>

using namespace cocos2d;

// Helper function to check if two points are the same (with a small tolerance for precision errors)
bool arePointsEqual(const Vec2& p1, const Vec2& p2, float tolerance = 0.01f) {
    return (std::abs(p1.x - p2.x) < tolerance) && (std::abs(p1.y - p2.y) < tolerance);
}

// Function to remove duplicate points from a polygon
std::vector<Vec2> removeDuplicatePoints(const std::vector<Vec2>& polygon) {
    std::vector<Vec2> uniquePolygon;
    for (const auto& point : polygon) {
        bool isDuplicate = false;
        for (const auto& uniquePoint : uniquePolygon) {
            if (arePointsEqual(point, uniquePoint)) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            uniquePolygon.push_back(point);
        }
    }
    return uniquePolygon;
}

// Function to sort points in a polygon based on their angle (for consistency)
std::vector<Vec2> sortPointsClockwise(std::vector<Vec2>& points) {
    // Find the centroid of the polygon
    Vec2 centroid(0, 0);
    for (const auto& point : points) {
        centroid += point;
    }

    // Manually divide the components of centroid by the size of the points vector
    if (!points.empty()) {
        centroid.x /= points.size();
        centroid.y /= points.size();
    }

    // Sort points based on angle relative to the centroid
    std::sort(points.begin(), points.end(), [&centroid](const Vec2& p1, const Vec2& p2) {
        float angle1 = std::atan2(p1.y - centroid.y, p1.x - centroid.x);
        float angle2 = std::atan2(p2.y - centroid.y, p2.x - centroid.x);
        return angle1 < angle2;
        });

    return points;
}


bool PhysicsShapeCache::addShapesWithFile(const std::string& plist) {
    auto dict = FileUtils::getInstance()->getValueMapFromFile(plist);

    if (dict.empty()) {
        CCLOG("Failed to load physics shapes from file: %s", plist.c_str());
        return false;
    }

    auto bodies = dict["bodies"].asValueMap();
    for (const auto& body : bodies) {
        BodyData bodyData;

        auto bodyDict = body.second.asValueMap();
        bodyData.anchorPoint = parseVec2(bodyDict["anchorpoint"].asString());

        auto fixturesArray = bodyDict["fixtures"].asValueVector();
        for (const auto& fixtureValue : fixturesArray) {
            auto fixtureDict = fixtureValue.asValueMap();

            FixtureData fixtureData;
            fixtureData.density = fixtureDict["density"].asFloat();
            fixtureData.friction = fixtureDict["friction"].asFloat();
            fixtureData.restitution = fixtureDict["restitution"].asFloat();
            fixtureData.isSensor = fixtureDict["isSensor"].asBool();

            auto polygonsArray = fixtureDict["polygons"].asValueVector();
            for (const auto& polygonValue : polygonsArray) {
                std::vector<Vec2> polygon;
                auto pointsArray = polygonValue.asValueVector();
                for (const auto& pointStr : pointsArray) {
                    polygon.push_back(parseVec2(pointStr.asString()));
                }
                fixtureData.polygons.push_back(polygon);
            }

            bodyData.fixtures.push_back(fixtureData);
        }

        shapeObjects[body.first] = bodyData;
    }

    return true;
}

PhysicsBody* PhysicsShapeCache::createBody(const std::string& shapeName, const Size& originalSize, const Size& scaledSize) {
    auto it = shapeObjects.find(shapeName);
    if (it == shapeObjects.end()) {
        CCLOG("Shape %s not found in PhysicsShapeCache", shapeName.c_str());
        return nullptr;
    }

    const auto& bodyData = it->second;
    auto physicsBody = PhysicsBody::create();

    float scaleX = scaledSize.width / originalSize.width;
    float scaleY = scaledSize.height / originalSize.height;

    // Loop over all fixtures in the shape
    for (const auto& fixture : bodyData.fixtures) {
        // Loop over all polygons in the fixture
        for (auto polygon : fixture.polygons) {
            // Remove duplicate points
            polygon = removeDuplicatePoints(polygon);

            // Sort points in clockwise order to ensure consistent shape rendering
            polygon = sortPointsClockwise(polygon);

            // Scale the points
            std::vector<Vec2> scaledPolygon;
            for (const auto& point : polygon) {
                scaledPolygon.push_back(Vec2(point.x * scaleX, point.y * scaleY));
            }

            // Create the physics shape
            auto shape = PhysicsShapePolygon::create(scaledPolygon.data(), scaledPolygon.size(), PHYSICSBODY_MATERIAL_DEFAULT);
            shape->setDensity(fixture.density);
            shape->setFriction(fixture.friction);
            shape->setRestitution(fixture.restitution);
            shape->setSensor(fixture.isSensor);

            // Add the shape to the physics body
            physicsBody->addShape(shape);
        }
    }

    // Apply the anchor point transformation
    physicsBody->setPositionOffset(Vec2(bodyData.anchorPoint.x * scaleX, bodyData.anchorPoint.y * scaleY));
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);

    return physicsBody;
}

void PhysicsShapeCache::clear() {
    shapeObjects.clear();
}

Vec2 PhysicsShapeCache::parseVec2(const std::string& pointStr) {
    Vec2 point;
    sscanf(pointStr.c_str(), "{ %f,%f }", &point.x, &point.y);
    return point;
}

void PhysicsShapeCache::resizeBody(PhysicsBody* physicsBody, const std::string& shapeName, const Size& originalSize, float scaleFactor) {
    if (physicsBody == nullptr) {
        CCLOG("PhysicsBody is nullptr, cannot resize.");
        return;
    }

    auto shapes = physicsBody->getShapes();

    Size screenSize = Director::getInstance()->getVisibleSize();

    float scaleX = (screenSize.width / originalSize.width) * scaleFactor;
    float scaleY = (screenSize.height / originalSize.height) * scaleFactor;

    float scale = std::min(scaleX, scaleY);

    for (auto shape : shapes) {
        physicsBody->removeShape(shape);
    }

    auto it = shapeObjects.find(shapeName);
    if (it == shapeObjects.end()) {
        CCLOG("Shape %s not found in PhysicsShapeCache", shapeName.c_str());
        return;
    }

    const auto& bodyData = it->second;

    for (const auto& fixture : bodyData.fixtures) {
        for (const auto& polygon : fixture.polygons) {
            std::vector<Vec2> scaledPolygon;
            for (const auto& point : polygon) {
                scaledPolygon.push_back(Vec2(point.x * scale, point.y * scale));
            }
            auto shape = PhysicsShapePolygon::create(scaledPolygon.data(), scaledPolygon.size(), PHYSICSBODY_MATERIAL_DEFAULT);
            shape->setDensity(fixture.density);
            shape->setFriction(fixture.friction);
            shape->setRestitution(fixture.restitution);
            shape->setSensor(fixture.isSensor);
            physicsBody->addShape(shape);
        }
    }

    physicsBody->setPositionOffset(Vec2(bodyData.anchorPoint.x * scale, bodyData.anchorPoint.y * scale));
}

cocos2d::PhysicsBody* PhysicsShapeCache::createBodyFromPlist(const std::string& plist, const std::string& shapeName, const cocos2d::Size& originalSize, const cocos2d::Size& scaledSize) {
    this->addShapesWithFile(plist);
    return this->createBody(shapeName, originalSize, scaledSize);
}

