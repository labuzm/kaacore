#include <chrono>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "kaacore/engine.h"
#include "kaacore/input.h"
#include "kaacore/log.h"
#include "kaacore/nodes.h"
#include "kaacore/scenes.h"
#include "kaacore/timers.h"

using namespace kaacore;

struct DemoScene : Scene {
    NodeOwnerPtr node;
    Seconds duration;
    Timer timer;

    DemoScene()
    {
        this->node = make_node();
        this->node->position({0, 0});
        this->node->color({1., 0., 0., 1});
        this->node->shape(Shape::Box({100., 100.}));
        this->root_node.add_child(this->node);
        this->duration = 1.s;

        this->timer = Timer([this](Seconds interval) {
            KAACORE_APP_LOG_INFO("Timer callback called.");
            this->node->visible(not this->node->visible());
            return interval;
        });
        this->timer.start(this->duration, this);
    }

    void update(const Seconds dt) override
    {
        for (auto const& event : this->get_events()) {
            if (auto keyboard_key = event.keyboard_key()) {
                if (keyboard_key->is_key_down()) {
                    if (keyboard_key->key() == Keycode::q) {
                        get_engine()->quit();
                    } else if (keyboard_key->key() == Keycode::s) {
                        if (this->timer.is_running()) {
                            this->timer.stop();
                        } else {
                            this->timer.start(this->duration, this);
                        }
                    }
                }
            }
        }
    }
};

extern "C" int
main(int argc, char* argv[])
{
    Engine eng({800, 600});
    DemoScene scene;
    scene.camera().position({0., 0.});
    eng.run(&scene);

    return 0;
}
