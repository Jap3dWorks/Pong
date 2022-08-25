#ifndef PONG_LEVEL
#define PONG_LEVEL

#include "Pong/core/movements.h"
#include "Pong/core/map/map.h"
#include "Pong/components/graphic_component.h"
#include "Pong/core/actor/actor.h"
#include "Pong/core/render/render.h"
#include "Pong/core/process_input.h"

#include <ctime>

// specific config of a level
namespace pong {
    class AbstractLevel {
    private:

    protected:
        Map *_scene;
        Render *_render;
        std::vector<ProcessInput> _inputList;

        using super = AbstractLevel;

    protected:
        virtual void _configInputs();

        virtual void _close_level();  // close level inputs
        virtual void _level_setup();
        virtual void _frame_collisions();

        /**While loop calculations*/
        virtual void _frame_calc();

        virtual void _update_actors();
        virtual void _frame_draw();
        virtual void _components_start_level();


    public:
        AbstractLevel();
        virtual ~AbstractLevel();

        virtual void run();
    };

}
#endif // PONG_LEVEL