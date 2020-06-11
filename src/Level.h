#include "pongTypes.h"
#include "Scene.h"
#include "Shape.h"
#include "Actor.h"
#include "Render.h"
#include "processInput.h"
#include <time.h>
#include "Utils.h"

#ifndef PONG_LEVEL
#define PONG_LEVEL

// specific config of a level
namespace Pong {
	class AbstractLevel
	{
	protected:
		Scene* _scene;
		Render* _render;
		std::vector<ProcessInput> _inputList;

		virtual void _configInputs();

		virtual void _close_level();  // close level inputs
		virtual void _level_setup();
		virtual void _frame_collisions();
		virtual void _frame_calc();
		virtual void _update_actors();
		virtual void _frame_draw();
		virtual void _components_start_level();
		virtual void _update_shader(Shader* shader);

	public:
		AbstractLevel();
		virtual ~AbstractLevel();

		virtual void run();
	};

	class Level: public AbstractLevel
	{
	public:
		void _configInputs() override;
		void run() override;
	};

	class TestLevel : public AbstractLevel
	{
	private:
		float shot_delay;

	protected:
		void _level_setup() override;
		void _frame_calc() override;

		void _barycentric_tst();
		//void _configInputs() override;
	
	public:
		TestLevel()
		{
		}
	};

	class PBRLevel : public AbstractLevel
	{
	protected:
		void _level_setup() override;
		// void _frame_calc() override;

	public:
		PBRLevel()
		{
		}
	};
}
#endif // PONG_LEVEL