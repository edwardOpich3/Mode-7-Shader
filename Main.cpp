#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>

using namespace std;

int main()
{
	bool quit = false;
	bool redraw = false;
	bool loading = true;
	float FOV = 150.0f;
	float FOVmult = tan((FOV * (ALLEGRO_PI / 180.0f)) / 2.0f);
	float FPS = 60.0f;
	int width = 1024;
	int height = 896;

	enum BUTTONS { LEFT, RIGHT, UP, DOWN, Z, X, A, D, W, S };
	bool buttons[10] = { false, false, false, false, false, false, false, false, false, false };

	float angle = 0.0f;

	float x = 924;
	float y = 546;
	float z = -32.0f;	// -16.0f for split screen!

	float horizon = 32.0f; // From the top

	if (!al_init())
	{
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE/* | ALLEGRO_OPENGL*/);		// Un-comment the "| ALLEGRO_OPENGL" to run in OpenGL mode. It slows things down a bit though!
	ALLEGRO_DISPLAY* display = al_create_display(width, height);

	if (!display)
	{
		return -1;
	}

	ALLEGRO_TIMER* timer = al_create_timer(1.0f / FPS);

	if (!timer)
	{
		al_destroy_display(display);
		return -1;
	}

	ALLEGRO_EVENT_QUEUE* event = al_create_event_queue();

	if (!event)
	{
		al_destroy_timer(timer);
		al_destroy_display(display);
		return -1;
	}

	al_set_window_title(display, "Kart Racing Engine");

	al_install_keyboard();

	al_init_image_addon();
	//al_init_primitives_addon();

	al_register_event_source(event, al_get_keyboard_event_source());
	al_register_event_source(event, al_get_timer_event_source(timer));
	al_register_event_source(event, al_get_display_event_source(display));

	al_start_timer(timer);

	ALLEGRO_BITMAP* buffer = al_create_bitmap(256, 224);
	ALLEGRO_BITMAP* mapBuffer = al_create_bitmap(256, 224);
	//cout << "Pixel format " << al_get_bitmap_format(buffer) << "\n";

	ALLEGRO_BITMAP* map = NULL;
	ALLEGRO_BITMAP* oob = NULL;
	ALLEGRO_BITMAP* parallax1 = NULL;
	ALLEGRO_BITMAP* parallax2 = NULL;

	//ALLEGRO_SHADER* defaultShader = NULL;
	ALLEGRO_SHADER* mode7Shader = NULL;
	ALLEGRO_SHADER* BGShader = NULL;

	// Game Loop
	while (!quit)
	{
		ALLEGRO_EVENT e;
		al_wait_for_event(event, &e);

		if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			quit = true;
		}

		else if (e.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
		}

		if (loading == true)
		{
			loading = false;

			//al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
			map = al_load_bitmap("data/map.png");
			oob = al_load_bitmap("data/oob.png");
			parallax1 = al_load_bitmap("data/parallax1.png");
			parallax2 = al_load_bitmap("data/parallax2.png");
			//al_set_new_bitmap_flags(!ALLEGRO_MEMORY_BITMAP);

			if (!(al_get_display_flags(display) & ALLEGRO_OPENGL))
			{
				mode7Shader = al_create_shader(ALLEGRO_SHADER_HLSL);
				if (!al_attach_shader_source_file(mode7Shader, ALLEGRO_VERTEX_SHADER, "Vertex Shader.hlsl"))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_attach_shader_source_file(mode7Shader, ALLEGRO_PIXEL_SHADER, "Mode7 Pixel Shader.hlsl"))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_build_shader(mode7Shader))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}

				BGShader = al_create_shader(ALLEGRO_SHADER_HLSL);
				if (!al_attach_shader_source_file(BGShader, ALLEGRO_VERTEX_SHADER, "Vertex Shader.hlsl"))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_attach_shader_source_file(BGShader, ALLEGRO_PIXEL_SHADER, "BG Pixel Shader.hlsl"))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_build_shader(BGShader))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
			}

			else if (al_get_display_flags(display) & ALLEGRO_OPENGL)
			{
				mode7Shader = al_create_shader(ALLEGRO_SHADER_GLSL);
				if (!al_attach_shader_source_file(mode7Shader, ALLEGRO_VERTEX_SHADER, "Vertex Shader.glsl"))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_attach_shader_source_file(mode7Shader, ALLEGRO_PIXEL_SHADER, "Mode7 Pixel Shader.glsl"))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_build_shader(mode7Shader))
				{
					std::cout << al_get_shader_log(mode7Shader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}

				BGShader = al_create_shader(ALLEGRO_SHADER_GLSL);
				if (!al_attach_shader_source_file(BGShader, ALLEGRO_VERTEX_SHADER, "Vertex Shader.glsl"))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_attach_shader_source_file(BGShader, ALLEGRO_PIXEL_SHADER, "BG Pixel Shader.glsl"))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
				else if (!al_build_shader(BGShader))
				{
					std::cout << al_get_shader_log(BGShader);
					std::cin.get();
					std::cin.get();
					quit = true;
				}
			}

			//cout << al_get_default_shader_source(ALLEGRO_SHADER_GLSL, ALLEGRO_PIXEL_SHADER);
			//cout << "Pixel format " << al_get_bitmap_format(map) << "\n";	// 9; ALLEGRO_PIXEL_FORMAT_ARGB_8888
			if (!map)
			{
				return -1;
			}
		}

		else if (e.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				buttons[LEFT] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				buttons[RIGHT] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				buttons[UP] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				buttons[DOWN] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_Z)
			{
				buttons[Z] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_X)
			{
				buttons[X] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_A)
			{
				buttons[A] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_D)
			{
				buttons[D] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_W)
			{
				buttons[W] = true;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_S)
			{
				buttons[S] = true;
			}
		}

		else if (e.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				buttons[LEFT] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				buttons[RIGHT] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				buttons[UP] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				buttons[DOWN] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_Z)
			{
				buttons[Z] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_X)
			{
				buttons[X] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_A)
			{
				buttons[A] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_D)
			{
				buttons[D] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_W)
			{
				buttons[W] = false;
			}

			else if (e.keyboard.keycode == ALLEGRO_KEY_S)
			{
				buttons[S] = false;
			}
		}

		// Update
		if (redraw == true)
		{
			double start = al_get_time();
			if (buttons[LEFT])
			{
				angle -= 1.5f;
				//x--;
			}
			if (buttons[RIGHT])
			{
				angle += 1.5f;
				//x++;
			}
			if (buttons[UP])
			{
				y -= 2.0f * cos(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
				x += 2.0f * sin(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
			}
			if (buttons[DOWN])
			{
				y += 2.0f * cos(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
				x -= 2.0f * sin(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
			}
			if (buttons[Z])
			{
				z -= 1 * (z / 32.0f);
			}
			if (buttons[X])
			{
				z += 1 * (z / 32.0f);
				if (z > 0.0f)
				{
					z = 0.0f;
				}
			}
			if (buttons[A])
			{
				y -= 2.0f * sin(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
				x -= 2.0f * cos(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
			}
			if (buttons[D])
			{
				y += 2.0f * sin(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
				x += 2.0f * cos(angle * (ALLEGRO_PI / 180.0f))/* * (-z / 16.0f)*/;
			}
			/*if (buttons[W])
			{
				horizon += 1.0f;
				if (horizon > 224.0f)
				{
					horizon = 224.0f;
				}
			}
			if (buttons[S])
			{
				horizon -= 1.0f;
				if (horizon < 0.0f)
				{
					horizon = 0.0f;
				}
			}*/

			//FOVmult = tan((FOV * (ALLEGRO_PI / 180.0f)) / 2.0f);

			ALLEGRO_TRANSFORM rotation;
			al_identity_transform(&rotation);
			al_translate_transform(&rotation, 0, 0);							// Move bitmap's origin to -x, -y

			if (!(al_get_display_flags(display) & ALLEGRO_OPENGL))
			{
				al_scale_transform(&rotation, 1.0f, 2.0f);							// Scale it; 2.0f for single player, 4.0f for vertical split-screen
			}

			al_rotate_transform(&rotation, angle * (ALLEGRO_PI / 180.0f));		// Rotate it

			if (al_get_display_flags(display) & ALLEGRO_OPENGL)
			{
				al_scale_transform(&rotation, 1.0f, 2.0f);
			}

			al_translate_transform(&rotation, 128, 112);						// Center the rotation point at 128, 112

			al_set_target_bitmap(mapBuffer);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_set_target_bitmap(buffer);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			// Software rendered Mode 7: VERY SLOW (by about 10x!)
			/*ALLEGRO_LOCKED_REGION *readlock, *oobreadlock, *writelock;
			readlock = al_lock_bitmap(map, ALLEGRO_PIXEL_FORMAT_ARGB_8888, ALLEGRO_LOCK_READONLY);
			oobreadlock = al_lock_bitmap(oob, ALLEGRO_PIXEL_FORMAT_ARGB_8888, ALLEGRO_LOCK_READONLY);
			writelock = al_lock_bitmap(buffer, ALLEGRO_PIXEL_FORMAT_ARGB_8888, ALLEGRO_LOCK_WRITEONLY);
			float curX, curY;
			for (int i = 256 * /*32 0; i < 256 * /*107 (224 - 32); i++)		// 256 = width, second number = height. Start at the top row, draw to row 224 - 32
			{
				curX = (i % 256);
				curY = (i / 256);
				Draw::applyTransform(&rotation, &curX, &curY);
				Draw::mode7(&curX, &curY, i, z, FOVmult);
				Draw::putPixel(writelock,
					(i % 256),
					(i / 256) + 32,												// Everything's shifted 32 rows down, to place the horizon at y = 32.
					Draw::getPixel(readlock,
						oobreadlock,
						curX + x,
						curY + y));
			}
			al_unlock_bitmap(buffer);
			al_unlock_bitmap(oob);
			al_unlock_bitmap(map);*/

			al_use_shader(mode7Shader);


			float gamePos[3] = { x, y, z };

			if (al_get_display_flags(display) & ALLEGRO_OPENGL)
			{
				if (!al_set_shader_float_vector("gamePos", 3, gamePos, 1))
				{
					std::cout << "Failed to update gamePos\n";
				}
			}
			else
			{
				if (!al_set_shader_float_vector("gamePos", 1, gamePos, 3))
				{
					std::cout << "Failed to update gamePos\n";
				}
			}

			if (!al_set_shader_float("horizon", horizon))
			{
				std::cout << "Failed to update horizon\n";
			}
			if (!al_set_shader_sampler("map_tex", map, 1))
			{
				std::cout << "Failed to upload map\n";
			}
			if (!al_set_shader_sampler("oob_tex", oob, 2))
			{
				std::cout << "Failed to upload oob\n";
			}
			if (!al_set_shader_matrix("transform", &rotation))
			{
				std::cout << "Failed to upload matrix\n";
			}

			al_draw_bitmap(mapBuffer, 0, horizon, NULL);
			al_use_shader(BGShader);
			al_identity_transform(&rotation);

			if (!al_set_shader_float("angle", angle))
			{
				std::cout << "Failed to update angle\n";
			}
			if (!al_set_shader_float("width", 1280.0f))
			{
				std::cout << "Failed to update width 1\n";
			}

			al_draw_bitmap(parallax2, 0, 0, NULL);

			if (!al_set_shader_float("width", 512.0f))
			{
				std::cout << "Failed to update width 2\n";
			}

			al_draw_bitmap(parallax1, 0, 0, NULL);

			al_use_shader(BGShader);

			//al_draw_bitmap(map, -x + 128, -y + 112, NULL);

			al_set_target_backbuffer(display);

			al_draw_scaled_bitmap(buffer, 0, 0, 256, 224, 0, 0, width, height, NULL);

			al_flip_display();
			double time = al_get_time() - start;
			std::cout << time * FPS << "\n";
		}
	}

	al_destroy_bitmap(map);
	al_destroy_bitmap(oob);
	al_destroy_bitmap(buffer);
	al_destroy_bitmap(mapBuffer);
	al_destroy_bitmap(parallax1);
	al_destroy_bitmap(parallax2);

	al_destroy_shader(mode7Shader);
	al_destroy_shader(BGShader);

	al_destroy_timer(timer);
	al_destroy_event_queue(event);
	al_destroy_display(display);

	return 0;
}