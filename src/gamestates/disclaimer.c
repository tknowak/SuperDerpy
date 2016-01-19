/*! \file disclaimer.c
 *  \brief Disclaimer about early release preview.
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
#include <stdio.h>
#include "intro.h"
#include "menu.h"
#include "about.h"

void Disclaimer_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.3, ALLEGRO_ALIGN_CENTRE, "Niniejsza gra dedykowana jest Ani Prętkiej,");
	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "głównej odtwórczyny roli Super Kuca Prętulino.");
	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Życzymy dobrej zabawy, cukierkowej radości i porzygu z tęczy.");
	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Margarita & Tomasz Nowak");
	al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.9, ALLEGRO_ALIGN_CENTRE, "Graj w grę!");
}

void Disclaimer_Load(struct Game *game) {
	FadeGameState(game, true);
}

int Disclaimer_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	UnloadGameState(game);
	game->loadstate = GAMESTATE_INTRO;
	LoadGameState(game);
	return 0;
}

void Disclaimer_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	if (!game->menu.loaded) {
		game->menu.font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.05,0 );
		game->menu.font_selected = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.065,0 );
	}
	PrintConsole(game, "Preloading GAMESTATE_INTRO...");
	Intro_Preload(game, progress);
}

void Disclaimer_Unload(struct Game *game) {
	FadeGameState(game, false);
	if (!game->menu.loaded) {
		al_destroy_font(game->menu.font);
		al_destroy_font(game->menu.font_selected);
	}
}
