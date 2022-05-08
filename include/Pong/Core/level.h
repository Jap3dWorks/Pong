#ifndef PONG_LEVEL
#define PONG_LEVEL

#include "movements.h"

#include "Pong/Core/scene.h"
#include "Pong/Core/shape.h"
#include "Pong/Core/actor.h"
#include "Pong/Core/render.h"
#include "Pong/Core/process_input.h"
#include <ctime>

// specific config of a level
namespace Pong {
    class AbstractLevel {
    private:

    protected:
        Scene *_scene;
        Render *_render;
        std::vector<ProcessInput> _inputList;

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