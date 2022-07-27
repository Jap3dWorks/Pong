#ifndef PONG_LEVEL
#define PONG_LEVEL

#include "movements.h"

#include "map.h"
#include "graphic_component.h"
#include "actor.h"
#include "render.h"
#include "process_input.h"
#include <ctime>

// specific config of a level
namespace Pong {
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