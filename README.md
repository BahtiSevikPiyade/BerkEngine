# BerkEngine

C++17 ile yazılmış, **Entity Component System (ECS)** mimarisine dayalı bir oyun motoru.

## Mimari

ECS (Entity Component System), oyun nesnelerini üç ayrı kavrama bölerek yüksek performans ve esneklik sağlar:

| Kavram | Açıklama |
|--------|----------|
| **Entity** | Benzersiz sayısal bir kimlik (ID). |
| **Component** | Yalnızca veri tutan bir yapı (struct). |
| **System** | Belirli bileşen kombinasyonuna sahip varlıkları işleyen mantık birimi. |

### Temel Sınıflar

- `BerkEngine::World` — ECS'nin merkezi koordinatörü; entity, component ve system API'lerini tek çatı altında sunar.
- `BerkEngine::EntityManager` — Entity ID'lerini dağıtır ve bileşen imzalarını izler.
- `BerkEngine::ComponentArray<T>` — Tek bir bileşen türünü yoğun (packed) dizide depolar.
- `BerkEngine::ComponentManager` — Tüm bileşen dizilerini yönetir.
- `BerkEngine::System` — Kullanıcı tanımlı sistemlerin türediği temel sınıf.
- `BerkEngine::SystemManager` — Sistemleri kaydeder ve entity üyeliğini günceller.
- `BerkEngine::Timer` — Frame delta/fixed timestep ve lag kontrolü sağlar.
- `BerkEngine::Application` — Uygulama yaşam döngüsü callback yüzeyi sağlar.
- `BerkEngine::Engine` — Standart frame pipeline döngüsünü (`Poll Events -> Fixed Update -> Variable Update -> Render`) yürütür.
- `BerkEngine::Event` / `BerkEngine::EventQueue` — Input/platform olaylarını frame bazlı kuyruklar.
- `BerkEngine::IPlatform` — Platform/pencere/input katmanı soyutlama arayüzüdür.
- `BerkEngine::Scene` — Sahne yaşam döngüsü ve update/render delegasyon sınırını tanımlar.
- `BerkEngine::IRenderer` — Render backend soyutlama arayüzüdür.
- `BerkEngine::HeadlessRenderer` — Görselleştirme gerektirmeyen çalıştırmalar için varsayılan backend uygulamasıdır.
- `BerkEngine::AssetManager` — Runtime’da temel texture metadata kayıt/sorgu API’si sağlar.

## Runtime Pipeline Standardı

Engine her frame aşağıdaki sırayla çalışır:

1. `Poll Events`
2. `Fixed Update`
3. `Variable Update`
4. `Render`

Event kuyruğu frame başında doldurulur, update/render fazlarında tüketilebilir ve frame sonunda temizlenir.

## Zaman Politikası

- `rawDeltaTime`: gerçek frame süresi
- `deltaTime`: `maxFrameDelta` ile clamp edilmiş süre
- `fixedTimeStep`: accumulator tabanlı sabit adım
- `maxFixedUpdatesPerFrame`: bir frame'de işlenecek sabit adım üst limiti
- Limit aşılırsa birikmiş lag düşürülür (`dropAccumulatedLag`) ve sonraki frame'e taşınmaz.

## Gereksinimler

- C++17 veya üzeri
- CMake 3.15+

## Derleme

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Testler

```bash
ctest --test-dir build --output-on-failure
```

## Kullanım Örneği

```cpp
#include <BerkEngine/BerkEngine.h>

// 1. Bileşenler (yalnızca veri)
struct Transform { float x, y, z; };
struct Velocity   { float vx, vy, vz; };

// 2. Sistem (mantık)
class PhysicsSystem : public BerkEngine::System {
public:
    void update(float dt) override {
        for (auto entity : mEntities) {
            auto& t = world->getComponent<Transform>(entity);
            auto& v = world->getComponent<Velocity>(entity);
            t.x += v.vx * dt;
            t.y += v.vy * dt;
            t.z += v.vz * dt;
        }
    }
    BerkEngine::World* world{nullptr};
};

int main() {
    BerkEngine::World world;

    // Bileşenleri kaydet
    world.registerComponent<Transform>();
    world.registerComponent<Velocity>();

    // Sistemi kaydet ve imzasını belirle
    auto physics = world.registerSystem<PhysicsSystem>();
    physics->world = &world;

    BerkEngine::Signature sig;
    sig.set(world.getComponentType<Transform>());
    sig.set(world.getComponentType<Velocity>());
    world.setSystemSignature<PhysicsSystem>(sig);

    // Entity oluştur ve bileşen ekle
    BerkEngine::Entity player = world.createEntity();
    world.addComponent(player, Transform{0.f, 0.f, 0.f});
    world.addComponent(player, Velocity{1.f, 0.f, 0.f});

    // Oyun döngüsü
    float dt = 0.016f;
    physics->update(dt);  // Transform otomatik olarak güncellenir

    return 0;
}
```

## Proje Yapısı

```
BerkEngine/
├── CMakeLists.txt
├── include/
│   └── BerkEngine/
│       ├── BerkEngine.h        # Tek-include başlık
│       ├── Types.h             # Entity, ComponentType, Signature
│       ├── EntityManager.h     # Entity ID yönetimi
│       ├── ComponentArray.h    # Yoğun bileşen dizisi
│       ├── ComponentManager.h  # Tüm bileşen dizilerinin yöneticisi
│       ├── System.h            # Sistem temel sınıfı
│       ├── SystemManager.h     # Sistem yöneticisi
│       ├── World.h             # Merkezi koordinatör
│       ├── Event.h             # Temel event tipi
│       ├── EventQueue.h        # Frame event kuyruğu
│       ├── Platform.h          # Platform soyutlama arayüzü
│       ├── Timer.h             # Delta/fixed timestep yönetimi
│       ├── Asset.h             # Asset veri tipleri
│       ├── AssetManager.h      # Asset kayıt/sorgu yöneticisi
│       ├── Renderer.h          # Render soyut arayüzü
│       ├── HeadlessRenderer.h  # Basit backend implementasyonu
│       ├── Scene.h             # Sahne arayüzü
│       ├── Application.h       # App yaşam döngüsü + scene yönetimi
│       └── Engine.h            # Runtime döngü orkestrasyonu
└── tests/
    ├── CMakeLists.txt
    ├── test_ecs.cpp            # ECS birim testleri
    └── test_runtime.cpp        # Runtime pipeline testleri
```
