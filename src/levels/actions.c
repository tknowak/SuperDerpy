/*! \file actions.c
 *  \brief Level actions for Timeline Manager.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "actions.h"
#include "../gamestates/level.h"

bool LevelFailed(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_filled_rectangle(0, 0, game->viewportWidth, game->viewportHeight, al_map_rgba(0,0,0,100));
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "Następnym razem się uda!");
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		// FIXME: this should be more generic. Some callback function?
		game->level.speed-=0.00001;
		if (game->level.speed<=0) {
			return true;
		}
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		Level_Unload(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
	}
	return false;
}

bool ShowMeter(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.meter_alpha+=4;
	if (game->level.meter_alpha>=255) {
		game->level.meter_alpha=255;
		return true;
	}
	return false;
}

bool Stop(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed=0;
	SelectDerpySpritesheet(game, "stand");
	return true;
}

bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(game->viewportWidth, game->viewportHeight));
	}
	float* fadeloop;
	ALLEGRO_BITMAP* fade_bitmap;
	fadeloop = (float*)action->arguments->value;
	fade_bitmap = (ALLEGRO_BITMAP*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*fadeloop = 255;
		al_set_target_bitmap(fade_bitmap);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_set_target_bitmap(al_get_backbuffer(game->display));
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		*fadeloop-=10;
		if (*fadeloop<=0) return true;
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		al_destroy_bitmap(fade_bitmap);
		free(fadeloop);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
		al_play_sample_instance(game->level.music);
	}
	return false;
}

bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(game->viewportWidth, game->viewportHeight));
	}
	float* fadeloop;
	ALLEGRO_BITMAP* fade_bitmap;
	fadeloop = (float*)action->arguments->value;
	fade_bitmap = (ALLEGRO_BITMAP*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*fadeloop = 0;
		al_set_target_bitmap(fade_bitmap);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_set_target_bitmap(al_get_backbuffer(game->display));
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		*fadeloop+=10;
		if (*fadeloop>=256) return true;
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		PrintConsole(game, "Leaving level with %d HP", (int)(game->level.hp*100));
		al_destroy_bitmap(fade_bitmap);
		free(fadeloop);
		Level_Unload(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool Welcome(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	float* tmp; bool* in;
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(bool)));
	}
	tmp = (float*)action->arguments->value;
	in = (bool*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*tmp = 0;
		*in = true;
		/*PrintConsole(game, "WELCOME INIT");*/
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		/*PrintConsole(game, "WELCOME RUNNING FADE=%f, IN=%d", *in); */
		float fade = *tmp;
		if (fade>255) fade=255;
		if (*tmp > 2048) { *tmp=255; *in=false; }
		if (*in) {
			*tmp+=10;
		} else {
			*tmp-=10;
			if (*tmp<=0) { return true; }
		}
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(game->level.welcome, al_map_rgba_f(*tmp/255.0,*tmp/255.0,*tmp/255.0,*tmp/255.0), 0, 0, 0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		free(action->arguments->value);
		free(action->arguments->next->value);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool PassLevel(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_DESTROY) {
		Level_Passed(game);
		Level_Unload(game);
		if (game->level.current_level<6) {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_MAP;
		} else {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_ABOUT;
		}
	}
	return true;
}

bool Letter(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) {
		float* f = (float*)malloc(sizeof(float));
		*f = 0;
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)malloc(sizeof(ALLEGRO_AUDIO_STREAM*));
		*stream = al_load_audio_stream(GetDataFilePath(GetLevelFilename(game, "levels/?/letter.flac")), 4, 1024);
		al_set_audio_stream_playing(*stream, false);
		al_set_audio_stream_gain(*stream, 2.00);
		action->arguments = TM_AddToArgs(action->arguments, (void*)f);
		action->arguments = TM_AddToArgs(action->arguments, (void*)stream);
		action->arguments->next->next = NULL;
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)action->arguments->next->value;
		al_set_audio_stream_playing(*stream, false);
		al_destroy_audio_stream(*stream);
		free(action->arguments->next->value);
		free(action->arguments->value);
		TM_DestroyArgs(action->arguments);
	} else if (state == TM_ACTIONSTATE_DRAW) {
		float* f = (float*)action->arguments->value;
		al_draw_tinted_bitmap(game->level.letter, al_map_rgba(*f,*f,*f,*f), (game->viewportWidth-al_get_bitmap_width(game->level.letter))/2.0, al_get_bitmap_height(game->level.letter)*-0.05, 0);
		return false;
	} else if (state == TM_ACTIONSTATE_PAUSE) {
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)action->arguments->next->value;
		al_set_audio_stream_playing(*stream, false);
	}	else if ((state == TM_ACTIONSTATE_RESUME) || (state == TM_ACTIONSTATE_START)) {
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)action->arguments->next->value;
		al_set_audio_stream_playing(*stream, true);
	}
	if (state != TM_ACTIONSTATE_RUNNING) return false;

	float* f = (float*)action->arguments->value;
	*f+=5;
	if (*f>255) *f=255;
	al_draw_tinted_bitmap(game->level.letter, al_map_rgba(*f,*f,*f,*f), (game->viewportWidth-al_get_bitmap_width(game->level.letter))/2.0, al_get_bitmap_height(game->level.letter)*-0.05, 0);
	struct ALLEGRO_KEYBOARD_STATE keyboard;
	al_get_keyboard_state(&keyboard);
	// FIXME: do it the proper way
	if (al_key_down(&keyboard, ALLEGRO_KEY_ENTER)) {
		return true;
	}
	return false;
}
