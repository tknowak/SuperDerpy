/*! \file main.c
 *  \brief Main file of SuperDerpy engine.
 *
 *   Contains basic functions shared by all views.
 */

#include <stdio.h>
#include "menu.h"
#include "loading.h"
#include "about.h"
#include "intro.h"
#include "map.h"
#include "level.h"
#include "config.h"

/*! \brief Macro for preloading gamestate.
 * 
 *  Preloading of state happens when loading screen is displayed.
 */
#define PRELOAD_STATE(state, name) case state:\
	PrintConsole(game, "Preload %s...", #state); DrawConsole(game); al_flip_display(); name ## _Preload(game); break;
/*! \brief Macro for unloading gamestate.
 * 
 *  Unloading of state happens after it's fadeout.
 */
#define UNLOAD_STATE(state, name) case state:\
	PrintConsole(game, "Unload %s...", #state); name ## _Unload(game); break;
/*! \brief Macro for loading gamestate.
 * 
 *  Loading of state means setting it as active and running it.
 */
#define LOAD_STATE(state, name) case state:\
	PrintConsole(game, "Load %s...", #state); name ## _Load(game); break;
/*! \brief Macro for sending keydown events to gamestate. */
#define KEYDOWN_STATE(state, name) else if (game.gamestate==state) { if (name ## _Keydown(&game, &ev)) break; }
/*! \brief Macro for drawing active gamestate. */
#define DRAW_STATE(state, name) case state:\
	name ## _Draw(&game); break;

void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text) {
	al_draw_text(font, al_map_rgba(0,0,0,128), x+1, y+1, flags, text);
	al_draw_text(font, color, x, y, flags, text);
}

void PrintConsole(struct Game *game, char* format, ...) {
	va_list vl;
	va_start(vl, format);
	char text[255] = {};
	vsprintf(text, format, vl);
	va_end(vl);
	if (game->debug) printf("%s\n", text);
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->console), al_get_bitmap_height(game->console));
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_text(game->font_console, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.005, al_get_bitmap_height(game->console)*0.81, ALLEGRO_ALIGN_LEFT, text);
	al_set_target_bitmap(game->console);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	//al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_bitmap(con, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(con);
}

void DrawConsole(struct Game *game) {
	if (game->showconsole) al_draw_bitmap(game->console, 0, 0, 0);
}

void PreloadGameState(struct Game *game) {
	if ((game->loadstate==GAMESTATE_MENU) && (game->menu.loaded)) {
		PrintConsole(game, "GAMESTATE_MENU already loaded, skipping...");
		return;
	}
	switch (game->loadstate) {
		PRELOAD_STATE(GAMESTATE_MENU, Menu)
		PRELOAD_STATE(GAMESTATE_LOADING, Loading)
		PRELOAD_STATE(GAMESTATE_ABOUT, About)
		PRELOAD_STATE(GAMESTATE_INTRO, Intro)
		PRELOAD_STATE(GAMESTATE_MAP, Map)
		PRELOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to preload unknown gamestate %d!", game->loadstate);
			break;
	}
	PrintConsole(game, "finished");
}

void UnloadGameState(struct Game *game) {
	switch (game->gamestate) {
		case GAMESTATE_MENU:
			if (game->shuttingdown) { 
				PrintConsole(game, "Unload GAMESTATE_MENU..."); Menu_Unload(game);
			} else {
				PrintConsole(game, "Just stopping GAMESTATE_MENU..."); Menu_Stop(game);
			}
			break;
		UNLOAD_STATE(GAMESTATE_LOADING, Loading)
		UNLOAD_STATE(GAMESTATE_ABOUT, About)
		UNLOAD_STATE(GAMESTATE_INTRO, Intro)
		UNLOAD_STATE(GAMESTATE_MAP, Map)
		UNLOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to unload unknown gamestate %d!", game->gamestate);
			break;
	}
	PrintConsole(game, "finished");
}

void LoadGameState(struct Game *game) {
	switch (game->loadstate) {
		LOAD_STATE(GAMESTATE_MENU, Menu)
		LOAD_STATE(GAMESTATE_LOADING, Loading)
		LOAD_STATE(GAMESTATE_ABOUT, About)
		LOAD_STATE(GAMESTATE_INTRO, Intro)
		LOAD_STATE(GAMESTATE_MAP, Map)
		LOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to load unknown gamestate %d!", game->loadstate);
	}
	PrintConsole(game, "finished");
	game->gamestate = game->loadstate;
	game->loadstate = -1;
}

void ScaleBitmap(ALLEGRO_BITMAP* source, int width, int height, float val) {
	if ((al_get_bitmap_width(source)==width) && (al_get_bitmap_height(source)==height)) {
		al_draw_bitmap(source, 0, 0, 0);
		return;
	}
	int x, y;
	for (y = 0; y < height; y++) {
		float pixy = ((float)y / height) * al_get_bitmap_height(source);
		for (x = 0; x < width; x++) {
			float pixx = ((float)x / width) * al_get_bitmap_width(source);
			ALLEGRO_COLOR a = al_get_pixel(source, pixx-val, pixy-val);
			ALLEGRO_COLOR b = al_get_pixel(source, pixx+val, pixy-val);
			ALLEGRO_COLOR c = al_get_pixel(source, pixx-val, pixy+val);
			ALLEGRO_COLOR d = al_get_pixel(source, pixx+val, pixy+val);
			ALLEGRO_COLOR result = al_map_rgba_f(
				(a.r+b.r+c.r+d.r) / 4,
				(a.g+b.b+c.g+d.g) / 4,
				(a.b+b.g+c.b+d.b) / 4,
				(a.a+b.a+c.a+d.a) / 4
			);
			al_draw_pixel(x, y, result);
		}
	}	
}

ALLEGRO_BITMAP* LoadFromCache(struct Game *game, char* filename, int width, int height) {
	ALLEGRO_BITMAP *source, *target = al_create_bitmap(width, height);
	al_set_target_bitmap(target);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	char origfn[255] = "data/";
	char cachefn[255] = "data/cache/";
	strcat(origfn, filename);
	strcat(cachefn, filename);
	void GenerateBitmap() {
		al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
		
		source = al_load_bitmap( origfn );
		al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

		ScaleBitmap(source, width, height, 0.35);
		al_save_bitmap(cachefn, target);
		PrintConsole(game, "Cache bitmap %s generated.", filename);
	}
	
	source = al_load_bitmap( cachefn );
	if (source) {
		if ((al_get_bitmap_width(source)!=width) || (al_get_bitmap_height(source)!=height)) {
			al_destroy_bitmap(source);
			GenerateBitmap();
			return target;
		}
		return source;
	} else GenerateBitmap();
	return target;
}

int main(int argc, char **argv){
	srand(time(NULL));

	InitConfig();
   
	bool redraw = true;

	struct Game game;

	game.fullscreen = atoi(GetConfigOptionDefault("[SuperDerpy]", "fullscreen", "1"));
	game.music = atoi(GetConfigOptionDefault("[SuperDerpy]", "music", "1"));
	game.fx = atoi(GetConfigOptionDefault("[SuperDerpy]", "fx", "1"));
	game.fps = atoi(GetConfigOptionDefault("[SuperDerpy]", "fps", "60"));
	if (game.fps<1) game.fps=60;
	game.debug = atoi(GetConfigOptionDefault("[SuperDerpy]", "debug", "0"));
	game.width = atoi(GetConfigOptionDefault("[SuperDerpy]", "width", "800"));
	if (game.width<320) game.width=320;
	game.height = atoi(GetConfigOptionDefault("[SuperDerpy]", "height", "500"));
	if (game.height<200) game.height=200;

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
   
	game.timer = al_create_timer(ALLEGRO_BPS_TO_SECS(game.fps));
	if(!game.timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	if(!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		//al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
		//                           NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if(!al_init_acodec_addon()){
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if(!al_install_audio()){
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if(!al_install_keyboard()){
		fprintf(stderr, "failed to initialize keyboard!\n");
		return -1;
	}

	if (!al_reserve_samples(10)){
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}
   
	al_init_font_addon();

	if(!al_init_ttf_addon()){
		fprintf(stderr, "failed to initialize fonts!\n");
		return -1;
	}

	if (game.fullscreen) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_OPENGL, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 0, ALLEGRO_REQUIRE);
	game.display = al_create_display(game.width, game.height);
	if(!game.display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	al_set_window_title(game.display, "Super Derpy: Muffin Attack");
	if (game.fullscreen) al_hide_mouse_cursor(game.display);

	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);
	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);

	game.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game.display)*0.09,0 );
	game.font_console = al_load_ttf_font("data/DejaVuSansMono.ttf",al_get_display_height(game.display)*0.018,0 );
   
	game.event_queue = al_create_event_queue();
	if(!game.event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(game.display);
		return -1;
	}

	al_register_event_source(game.event_queue, al_get_display_event_source(game.display));
	al_register_event_source(game.event_queue, al_get_timer_event_source(game.timer));
	al_register_event_source(game.event_queue, al_get_keyboard_event_source());

	game.showconsole = game.debug;
	game.console = al_create_bitmap(al_get_display_width(game.display), al_get_display_height(game.display)*0.12);
	al_set_target_bitmap(game.console);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_set_target_bitmap(al_get_backbuffer(game.display));

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();

	al_start_timer(game.timer);

	game.shuttingdown = false;
	game.menu.loaded = false;
	game.loadstate = GAMESTATE_LOADING;
	PreloadGameState(&game);
	LoadGameState(&game);
	game.loadstate = GAMESTATE_MENU;
	while(1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(game.event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			if ((ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_TILDE)) {
				game.showconsole = !game.showconsole;
			}
			KEYDOWN_STATE(GAMESTATE_MENU, Menu)
			KEYDOWN_STATE(GAMESTATE_LOADING, Loading)
			KEYDOWN_STATE(GAMESTATE_ABOUT, About)
			KEYDOWN_STATE(GAMESTATE_INTRO, Intro)
			KEYDOWN_STATE(GAMESTATE_MAP, Map)
			KEYDOWN_STATE(GAMESTATE_LEVEL, Level)
			else {
				game.showconsole = true;
				PrintConsole(&game, "ERROR: Keystroke in unknown (%d) gamestate! (5 sec sleep)", game.gamestate);
				DrawConsole(&game);
				al_flip_display();
				al_rest(5.0);
				PrintConsole(&game, "Returning to menu...");
				game.gamestate = GAMESTATE_LOADING;
				game.loadstate = GAMESTATE_MENU;
			}
		}
	
		if(redraw && al_is_event_queue_empty(game.event_queue)) {
			redraw = false;
			switch (game.gamestate) {
				DRAW_STATE(GAMESTATE_MENU, Menu)
				DRAW_STATE(GAMESTATE_LOADING, Loading)
				DRAW_STATE(GAMESTATE_ABOUT, About)
				DRAW_STATE(GAMESTATE_INTRO, Intro)
				DRAW_STATE(GAMESTATE_MAP, Map)
				DRAW_STATE(GAMESTATE_LEVEL, Level)
				default:
					game.showconsole = true;
					PrintConsole(&game, "ERROR: Unknown gamestate %d reached! (5 sec sleep)", game.gamestate);
					DrawConsole(&game);
					al_flip_display();
					al_rest(5.0);
					PrintConsole(&game, "Returning to menu...");
					game.gamestate = GAMESTATE_LOADING;
					game.loadstate = GAMESTATE_MENU;
					break;
			}
			DrawConsole(&game);
			al_flip_display();
		}
	}
	game.shuttingdown = true;
	UnloadGameState(&game);
	if (game.gamestate != GAMESTATE_LOADING) {
		game.gamestate = GAMESTATE_LOADING;
		UnloadGameState(&game);
	}
	al_clear_to_color(al_map_rgb(0,0,0));
	PrintConsole(&game, "Shutting down...");
	DrawConsole(&game);
	al_flip_display();
	al_rest(0.1);
	al_destroy_timer(game.timer);
	al_destroy_display(game.display);
	al_destroy_event_queue(game.event_queue);
	al_destroy_font(game.font);
	al_destroy_font(game.font_console);
	al_uninstall_audio();
	DeinitConfig();
	return 0;
}
