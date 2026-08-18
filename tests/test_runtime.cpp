#include <BerkEngine/BerkEngine.h>

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FakePlatform : public BerkEngine::IPlatform {
public:
    void initialize() override { ++initializeCount; }
    void pollEvents(BerkEngine::EventQueue& events) override {
        ++pollCount;
        events.push(BerkEngine::Event{BerkEngine::EventType::WindowResized, false});
    }
    bool shouldClose() const override { return closeRequested; }
    void shutdown() override { ++shutdownCount; }

    int initializeCount{0};
    int pollCount{0};
    int shutdownCount{0};
    bool closeRequested{false};
};

class CountingRenderer : public BerkEngine::IRenderer {
public:
    void initialize() override { ++initializeCount; }
    void beginFrame() override { ++beginFrameCount; }
    void draw(BerkEngine::World&, double alpha) override {
        ++drawCount;
        lastAlpha = alpha;
    }
    void endFrame() override { ++endFrameCount; }
    void shutdown() override { ++shutdownCount; }

    int initializeCount{0};
    int beginFrameCount{0};
    int drawCount{0};
    int endFrameCount{0};
    int shutdownCount{0};
    double lastAlpha{0.0};
};

class RuntimeApplication : public BerkEngine::Application {
public:
    void onPollEvents(BerkEngine::EventQueue& events) override {
        callOrder.push_back("poll");
        events.push(BerkEngine::Event{BerkEngine::EventType::User, false});
    }

    void onFixedUpdate(BerkEngine::World&, double) override {
        callOrder.push_back("fixed");
        ++fixedCount;
    }

    void onVariableUpdate(BerkEngine::World&, double dt) override {
        callOrder.push_back("variable");
        variableDelta = dt;
        hasEventInVariable = !events().empty();
    }

    void onRender(BerkEngine::World&, double alpha) override {
        callOrder.push_back("render");
        renderAlpha = alpha;
        BerkEngine::Application::onRender(world(), alpha);
    }

    std::vector<std::string> callOrder{};
    int fixedCount{0};
    double variableDelta{0.0};
    double renderAlpha{0.0};
    bool hasEventInVariable{false};
};

class TrackingScene : public BerkEngine::Scene {
public:
    void onEnter(BerkEngine::Application&) override { ++enterCount; }
    void onExit(BerkEngine::Application&) override { ++exitCount; }
    void onRender(BerkEngine::Application&, BerkEngine::World& world, BerkEngine::IRenderer& renderer, double alpha) override {
        ++renderCount;
        renderer.draw(world, alpha);
    }

    int enterCount{0};
    int exitCount{0};
    int renderCount{0};
};

static void testLifecycleAndTiming() {
    BerkEngine::Engine::Config cfg;
    cfg.timer.fixedTimeStepSeconds = 0.01;
    cfg.timer.maxFrameDeltaSeconds = 0.03;
    cfg.maxFixedUpdatesPerFrame = 5;

    BerkEngine::Engine engine(cfg);
    RuntimeApplication app;
    CountingRenderer renderer;
    app.setRenderer(&renderer);

    engine.runSingleFrame(app, 0.2); // clamped to 0.03 -> 3 fixed steps

    assert((app.callOrder.size() >= 6));
    assert(app.callOrder.front() == "poll");
    assert(app.callOrder[1] == "fixed");
    assert(app.callOrder[2] == "fixed");
    assert(app.callOrder[3] == "fixed");
    assert(app.callOrder[4] == "variable");
    assert(app.callOrder[5] == "render");
    assert(app.fixedCount == 3);
    assert(app.variableDelta == 0.03);
    assert(app.hasEventInVariable == true);
    assert(app.events().empty() == true);
    assert(renderer.beginFrameCount == 1);
    assert(renderer.drawCount == 1);
    assert(renderer.endFrameCount == 1);
}

static void testLagDropRule() {
    BerkEngine::Engine::Config cfg;
    cfg.timer.fixedTimeStepSeconds = 0.01;
    cfg.timer.maxFrameDeltaSeconds = 0.2;
    cfg.maxFixedUpdatesPerFrame = 2;

    BerkEngine::Engine engine(cfg);
    RuntimeApplication app;

    engine.runSingleFrame(app, 0.2); // would schedule many ticks, cap at 2 then drop lag
    assert(app.fixedCount == 2);
    assert(app.renderAlpha == 0.0);
}

static void testPlatformIntegrationAndQuit() {
    BerkEngine::Engine engine;
    RuntimeApplication app;
    FakePlatform platform;
    app.setPlatform(&platform);

    engine.runSingleFrame(app, 0.016);
    assert(platform.pollCount == 1);
    assert(app.isRunning() == true);

    platform.closeRequested = true;
    engine.runSingleFrame(app, 0.016);
    assert(platform.pollCount == 2);
    assert(app.isRunning() == false);
}

static void testAssetManagerBasics() {
    RuntimeApplication app;

    const bool firstInsert = app.assets().registerTexture(
        BerkEngine::TextureAsset{"hero_idle", "assets/hero_idle.png", 64, 64});
    assert(firstInsert == true);
    assert(app.assets().hasTexture("hero_idle") == true);

    const bool secondInsert = app.assets().registerTexture(
        BerkEngine::TextureAsset{"hero_idle", "assets/hero_idle_v2.png", 128, 128});
    assert(secondInsert == false);

    const auto* texture = app.assets().findTexture("hero_idle");
    assert(texture != nullptr);
    assert(texture->sourcePath == "assets/hero_idle_v2.png");
    assert(texture->width == 128);
    assert(texture->height == 128);
}

static void testAssetManifestLoading() {
    RuntimeApplication app;

    const std::string manifestPath = "/tmp/berkengine_texture_manifest.txt";
    {
        std::ofstream out(manifestPath);
        out << "# name path width height\n";
        out << "hero_idle assets/hero_idle.png 64 64\n";
        out << "hero_run assets/hero_run.png 128 64\n";
    }

    const bool loaded = app.assets().loadTextureManifest(manifestPath);
    assert(loaded == true);

    const auto* heroIdle = app.assets().findTexture("hero_idle");
    assert(heroIdle != nullptr);
    assert(heroIdle->width == 64);
    assert(heroIdle->height == 64);

    const auto* heroRun = app.assets().findTexture("hero_run");
    assert(heroRun != nullptr);
    assert(heroRun->sourcePath == "assets/hero_run.png");

    std::remove(manifestPath.c_str());
}

static void testAssetManifestLoadingRejectsMalformedInput() {
    RuntimeApplication app;

    const std::string manifestPath = "/tmp/berkengine_texture_manifest_invalid.txt";
    {
        std::ofstream out(manifestPath);
        out << "missing_columns_only_name\n";
    }

    const bool loaded = app.assets().loadTextureManifest(manifestPath);
    assert(loaded == false);
    assert(app.assets().hasTexture("missing_columns_only_name") == false);

    std::remove(manifestPath.c_str());
}

static void testHeadlessRenderer() {
    BerkEngine::HeadlessRenderer renderer;
    BerkEngine::World world;

    renderer.initialize();
    assert(renderer.isInitialized() == true);
    renderer.beginFrame();
    renderer.draw(world, 0.5);
    renderer.endFrame();

    assert(renderer.frameCount() == 1);
    assert(renderer.lastInterpolationAlpha() == 0.5);
    renderer.shutdown();
    assert(renderer.isInitialized() == false);
}

static void testSDLPlatformCompileTimeAvailability() {
    BerkEngine::SDLPlatform platform;
    assert(platform.isInitialized() == false);
    platform.shutdown();
}

static void testSceneLifecycle() {
    RuntimeApplication app;
    auto sceneA = std::make_shared<TrackingScene>();
    auto sceneB = std::make_shared<TrackingScene>();

    app.setScene(sceneA);
    assert(sceneA->enterCount == 1);
    assert(sceneA->exitCount == 0);

    app.setScene(sceneB);
    assert(sceneA->exitCount == 1);
    assert(sceneB->enterCount == 1);
}

int main() {
    std::cout << "BerkEngine Runtime Tests\n";
    testLifecycleAndTiming();
    testLagDropRule();
    testPlatformIntegrationAndQuit();
    testAssetManagerBasics();
    testAssetManifestLoading();
    testAssetManifestLoadingRejectsMalformedInput();
    testHeadlessRenderer();
    testSDLPlatformCompileTimeAvailability();
    testSceneLifecycle();
    std::cout << "All runtime tests passed\n";
    return 0;
}
