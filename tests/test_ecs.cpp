#include <BerkEngine/BerkEngine.h>

#include <cassert>
#include <functional>
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// Sample components
// ---------------------------------------------------------------------------

struct Transform {
    float x{0.f}, y{0.f}, z{0.f};
};

struct Velocity {
    float vx{0.f}, vy{0.f}, vz{0.f};
};

struct Health {
    float value{100.f};
};

// ---------------------------------------------------------------------------
// Sample systems
// ---------------------------------------------------------------------------

class PhysicsSystem : public BerkEngine::System {
public:
    void update(float deltaTime) override {
        ++updateCount;
        lastDeltaTime = deltaTime;
    }

    int   updateCount{0};
    float lastDeltaTime{0.f};
};

class HealthSystem : public BerkEngine::System {
public:
    void update(float /*deltaTime*/) override {}
};

// ---------------------------------------------------------------------------
// Minimal test runner
// ---------------------------------------------------------------------------

static int sPassCount = 0;
static int sFailCount = 0;

static void runTest(const std::string& name, std::function<void()> fn) {
    std::cout << "  " << name << " ... ";
    fn(); // asserts abort on failure; reaching here means success
    std::cout << "PASS\n";
    ++sPassCount;
}

// ---------------------------------------------------------------------------
// Test suites
// ---------------------------------------------------------------------------

static void testEntityCreation() {
    std::cout << "\n[Entity creation]\n";

    runTest("createEntity returns unique IDs", [] {
        BerkEngine::World world;
        auto e1 = world.createEntity();
        auto e2 = world.createEntity();
        auto e3 = world.createEntity();
        assert(e1 != e2);
        assert(e2 != e3);
        assert(e1 != e3);
    });

    runTest("getLivingEntityCount tracks live entities", [] {
        BerkEngine::World world;
        assert(world.getLivingEntityCount() == 0u);
        auto e1 = world.createEntity();
        auto e2 = world.createEntity();
        assert(world.getLivingEntityCount() == 2u);
        world.destroyEntity(e1);
        assert(world.getLivingEntityCount() == 1u);
        world.destroyEntity(e2);
        assert(world.getLivingEntityCount() == 0u);
    });

    runTest("destroyed entity ID is recycled", [] {
        BerkEngine::World world;
        auto e1 = world.createEntity();
        world.destroyEntity(e1);
        auto e2 = world.createEntity();
        assert(e2 == e1);
    });
}

static void testComponents() {
    std::cout << "\n[Components]\n";

    runTest("addComponent / getComponent", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();

        auto e = world.createEntity();
        world.addComponent(e, Transform{1.f, 2.f, 3.f});

        auto& t = world.getComponent<Transform>(e);
        assert(t.x == 1.f && t.y == 2.f && t.z == 3.f);
    });

    runTest("getComponent returns a mutable reference", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();

        auto e = world.createEntity();
        world.addComponent(e, Transform{0.f, 0.f, 0.f});

        world.getComponent<Transform>(e).x = 42.f;
        assert(world.getComponent<Transform>(e).x == 42.f);
    });

    runTest("hasComponent reflects presence/absence", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Health>();

        auto e = world.createEntity();
        world.addComponent(e, Transform{});

        assert(world.hasComponent<Transform>(e) == true);
        assert(world.hasComponent<Health>(e)    == false);
    });

    runTest("removeComponent removes the component", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto e = world.createEntity();
        world.addComponent(e, Transform{});
        world.addComponent(e, Velocity{1.f, 0.f, 0.f});

        assert(world.hasComponent<Velocity>(e) == true);
        world.removeComponent<Velocity>(e);
        assert(world.hasComponent<Velocity>(e)  == false);
        assert(world.hasComponent<Transform>(e) == true);
    });

    runTest("multiple entities own independent component data", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();

        auto e1 = world.createEntity();
        auto e2 = world.createEntity();
        world.addComponent(e1, Transform{1.f, 0.f, 0.f});
        world.addComponent(e2, Transform{9.f, 0.f, 0.f});

        assert(world.getComponent<Transform>(e1).x == 1.f);
        assert(world.getComponent<Transform>(e2).x == 9.f);

        world.getComponent<Transform>(e1).x = 5.f;
        assert(world.getComponent<Transform>(e2).x == 9.f);
    });

    runTest("destroyEntity cleans up all components", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto e = world.createEntity();
        world.addComponent(e, Transform{});
        world.addComponent(e, Velocity{});
        world.destroyEntity(e);

        // After the ID is recycled, the new entity starts with no components
        auto e2 = world.createEntity();
        assert(world.hasComponent<Transform>(e2) == false);
        assert(world.hasComponent<Velocity>(e2)  == false);
    });
}

static void testSystems() {
    std::cout << "\n[Systems]\n";

    runTest("system only tracks entities whose signature matches", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature sig;
        sig.set(world.getComponentType<Transform>());
        sig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(sig);

        auto e1 = world.createEntity();
        world.addComponent(e1, Transform{});
        world.addComponent(e1, Velocity{1.f, 0.f, 0.f});

        auto e2 = world.createEntity();
        world.addComponent(e2, Transform{}); // no Velocity

        assert(physSystem->mEntities.size()   == 1u);
        assert(physSystem->mEntities.count(e1) == 1u);
        assert(physSystem->mEntities.count(e2) == 0u);
    });

    runTest("addComponent triggers system membership update", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature sig;
        sig.set(world.getComponentType<Transform>());
        sig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(sig);

        auto e = world.createEntity();
        world.addComponent(e, Transform{});
        assert(physSystem->mEntities.count(e) == 0u);

        world.addComponent(e, Velocity{});
        assert(physSystem->mEntities.count(e) == 1u);
    });

    runTest("removeComponent triggers system membership update", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature sig;
        sig.set(world.getComponentType<Transform>());
        sig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(sig);

        auto e = world.createEntity();
        world.addComponent(e, Transform{});
        world.addComponent(e, Velocity{});
        assert(physSystem->mEntities.count(e) == 1u);

        world.removeComponent<Velocity>(e);
        assert(physSystem->mEntities.count(e) == 0u);
    });

    runTest("destroyEntity removes entity from all systems", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();
        world.registerComponent<Health>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature physSig;
        physSig.set(world.getComponentType<Transform>());
        physSig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(physSig);

        auto healthSystem = world.registerSystem<HealthSystem>();
        BerkEngine::Signature healthSig;
        healthSig.set(world.getComponentType<Health>());
        world.setSystemSignature<HealthSystem>(healthSig);

        auto e = world.createEntity();
        world.addComponent(e, Transform{});
        world.addComponent(e, Velocity{});
        world.addComponent(e, Health{50.f});

        assert(physSystem->mEntities.count(e)   == 1u);
        assert(healthSystem->mEntities.count(e) == 1u);

        world.destroyEntity(e);

        assert(physSystem->mEntities.count(e)   == 0u);
        assert(healthSystem->mEntities.count(e) == 0u);
    });

    runTest("system update() is called with correct deltaTime", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature sig;
        sig.set(world.getComponentType<Transform>());
        sig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(sig);

        assert(physSystem->updateCount == 0);
        physSystem->update(0.016f);
        assert(physSystem->updateCount    == 1);
        assert(physSystem->lastDeltaTime  == 0.016f);
    });

    runTest("multiple systems coexist independently", [] {
        BerkEngine::World world;
        world.registerComponent<Transform>();
        world.registerComponent<Velocity>();
        world.registerComponent<Health>();

        auto physSystem = world.registerSystem<PhysicsSystem>();
        BerkEngine::Signature physSig;
        physSig.set(world.getComponentType<Transform>());
        physSig.set(world.getComponentType<Velocity>());
        world.setSystemSignature<PhysicsSystem>(physSig);

        auto healthSystem = world.registerSystem<HealthSystem>();
        BerkEngine::Signature healthSig;
        healthSig.set(world.getComponentType<Health>());
        world.setSystemSignature<HealthSystem>(healthSig);

        auto e1 = world.createEntity();
        world.addComponent(e1, Transform{});
        world.addComponent(e1, Velocity{});

        auto e2 = world.createEntity();
        world.addComponent(e2, Health{80.f});

        auto e3 = world.createEntity();
        world.addComponent(e3, Transform{});
        world.addComponent(e3, Velocity{});
        world.addComponent(e3, Health{50.f});

        assert(physSystem->mEntities.size()   == 2u); // e1, e3
        assert(healthSystem->mEntities.size() == 2u); // e2, e3
    });
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int main() {
    std::cout << "BerkEngine ECS Tests\n";
    std::cout << "====================\n";

    testEntityCreation();
    testComponents();
    testSystems();

    std::cout << "\n============================\n";
    std::cout << "Results: " << sPassCount << " passed";
    if (sFailCount > 0) {
        std::cout << ", " << sFailCount << " failed";
    }
    std::cout << "\n";

    return (sFailCount == 0) ? 0 : 1;
}
