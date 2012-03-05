/*! \file menu.c
 *  \brief Main Menu view.
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
#include <math.h>
#include "config.h"
#include "menu.h"

void Menu_Draw(struct Game *game) {

	al_set_target_bitmap(game->menu.pinkcloud_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float x = 1.5;//*(rand() / (float)RAND_MAX);
	int minus;
	if (game->menu.cloud_position>0) minus=1; else minus=-1;
	al_draw_bitmap(game->menu.rain_bitmap, fmod(minus*game->menu.cloud_position,3)*x*5+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.7, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.88+(fmod(-1.8*(game->menu.cloud_position+80), 6))/20.0), 0);
	al_draw_bitmap(game->menu.rain_bitmap, fmod(minus*game->menu.cloud_position,3)*x*3+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/3.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.78+(fmod(-2.8*(game->menu.cloud_position+80), 4))/18.0), 0);
	al_draw_scaled_bitmap(game->menu.rain_bitmap, 0, 0, al_get_bitmap_width(game->menu.rain_bitmap), al_get_bitmap_height(game->menu.rain_bitmap), fmod(minus*game->menu.cloud_position,3)*x*6+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.87+(fmod(-4.9*(game->menu.cloud_position+80), 8))/26.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.4, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.08, 0);
	//al_draw_scaled_bitmap(game->menu.rain, 0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), fmod(minus*game->menu.cloud_position,3)*x*6+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.4, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.9+(fmod(-5*(game->menu.cloud_position+86), 8))/20.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.35, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.07, 0);
	al_draw_bitmap(game->menu.pinkcloud, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(183,234,193));
	float tint = (sin((game->menu.cloud_position-80)/15)+1)/2;
	if (tint < 0.000004) { PrintConsole(game, "random tint %f", tint); game->menu.mountain_position = (al_get_display_width(game->display)*(rand()/(float)RAND_MAX)/2)+al_get_display_width(game->display)/2; }
	al_draw_tinted_bitmap(game->menu.mountain,al_map_rgba_f(tint,tint,tint,tint),game->menu.mountain_position, 0,0);
	al_draw_scaled_bitmap(game->menu.cloud,0,0,al_get_bitmap_width(game->menu.cloud), al_get_bitmap_height(game->menu.cloud), al_get_display_width(game->display)*(sin((game->menu.cloud_position/40)-4.5)-0.3), al_get_display_height(game->display)*0.35, al_get_bitmap_width(game->menu.cloud)/2, al_get_bitmap_height(game->menu.cloud)/2,0);
	al_draw_bitmap(game->menu.cloud2,al_get_display_width(game->display)*(game->menu.cloud2_position/100.0), al_get_display_height(game->display)/1.5,0);
	al_draw_bitmap(game->menu.image,0, al_get_display_height(game->display)*0.55,0);
	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.12) + (cos((game->menu.cloud_position/25+80)*1.74444))*40, 0,0);
	al_draw_bitmap(game->menu.cloud,al_get_display_width(game->display)*game->menu.cloud_position/100, 30,0);

	al_draw_bitmap(game->menu.pie_bitmap, al_get_display_width(game->display)/2, al_get_display_height(game->display)*(game->menu.cloud_position)/10,0);

	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Muffin Attack");

	ALLEGRO_FONT *font;
	char* text;
	font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
	text = "Start game"; if (game->menu.options) text="Control settings";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
	text = "Options"; if (game->menu.options) text="Video settings";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
	text = "About"; if (game->menu.options) {
		if ((game->music) && (game->fx))
			text="Sounds: all";
		else if (game->music)
			text="Sounds: music only";
		else if (game->fx)
			text="Sounds: fx only";
		else
			text="Sounds: none";
	}
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
	text = "Exit"; if (game->menu.options) text="Back";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, text);

	game->menu.cloud_position-=tps(game, 0.1*60);
	game->menu.cloud2_position-=tps(game, 0.025*60);
	if (game->menu.cloud_position<-80) { game->menu.cloud_position=100; PrintConsole(game, "cloud_position"); }
	if (game->menu.cloud2_position<0) { game->menu.cloud2_position=100; PrintConsole(game, "cloud2_position"); }
}

void Menu_Preload(struct Game *game) {
	game->menu.loaded = true;
	game->menu.image = LoadScaledBitmap( "menu.png", al_get_display_width(game->display), al_get_display_height(game->display)*0.45);
	game->menu.mountain = LoadScaledBitmap( "mountain.png", al_get_display_width(game->display)*0.055, al_get_display_height(game->display)/9 );
	game->menu.cloud = LoadScaledBitmap( "cloud.png", al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.25 );
	game->menu.cloud2 = LoadScaledBitmap( "cloud2.png", al_get_display_width(game->display)*0.2, al_get_display_height(game->display)*0.1 );
	game->menu.pinkcloud = LoadScaledBitmap( "pinkcloud.png", al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)*0.8122);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	game->menu.rain = al_load_bitmap( "data/rain.png" );
	game->menu.pie = al_load_bitmap( "data/pie.png" );
	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

	game->menu.sample = al_load_sample( "data/menu.flac" );
	game->menu.rain_sample = al_load_sample( "data/rain.flac" );
	game->menu.click_sample = al_load_sample( "data/click.flac" );
	game->menu.mountain_position = al_get_display_width(game->display)*0.7;

	game->menu.music = al_create_sample_instance(game->menu.sample);
	al_attach_sample_instance_to_mixer(game->menu.music, game->audio.music);
	al_set_sample_instance_playmode(game->menu.music, ALLEGRO_PLAYMODE_LOOP);

	game->menu.rain_sound = al_create_sample_instance(game->menu.rain_sample);
	al_attach_sample_instance_to_mixer(game->menu.rain_sound, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.rain_sound, ALLEGRO_PLAYMODE_LOOP);

	game->menu.click = al_create_sample_instance(game->menu.click_sample);
	al_attach_sample_instance_to_mixer(game->menu.click, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.click, ALLEGRO_PLAYMODE_ONCE);

	game->menu.font_title = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.16,0 );
	game->menu.font_subtitle = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.08,0 );
	game->menu.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.05,0 );
	game->menu.font_selected = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.065,0 );
	
	if (!game->menu.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.rain_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.click_sample){
		fprintf(stderr, "Audio clip sample#3 not loaded!\n" );
		exit(-1);
	}

	game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)); // *0.8122);

	game->menu.pie_bitmap = al_create_bitmap(al_get_display_width(game->display)/2, al_get_display_height(game->display)); //*0.8122);
	al_set_target_bitmap(game->menu.pie_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.5, 0, al_get_display_width(game->display)*0.11875, al_get_display_height(game->display)*0.0825, 0);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.1, al_get_bitmap_height(game->menu.pie_bitmap)*0.3, al_get_display_width(game->display)*0.09, al_get_display_height(game->display)*0.06, ALLEGRO_FLIP_HORIZONTAL);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.3, al_get_bitmap_height(game->menu.pie_bitmap)*0.6, al_get_display_width(game->display)*0.13, al_get_display_height(game->display)*0.1, 0);
	al_destroy_bitmap(game->menu.pie);

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	game->menu.rain_bitmap = al_create_bitmap(al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.5, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.1);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al_set_target_bitmap(game->menu.rain_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.rain,0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), 0, 0, al_get_bitmap_width(game->menu.rain_bitmap), al_get_bitmap_height(game->menu.rain_bitmap),0);
	al_destroy_bitmap(game->menu.rain);
}

void Menu_Stop(struct Game* game) {
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		//if ((game->menu.draw_while_fading) || (fadeloop==255)) {
		al_wait_for_event(game->event_queue, &ev);
		Menu_Draw(game);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_stop_sample_instance(game->menu.music);
	al_stop_sample_instance(game->menu.rain_sound);
}

void Menu_Unload(struct Game *game) {
	if (game->gamestate==GAMESTATE_MENU) Menu_Stop(game);
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
	al_destroy_bitmap(game->menu.pinkcloud);
	al_destroy_bitmap(game->menu.image);
	al_destroy_bitmap(game->menu.cloud);
	al_destroy_bitmap(game->menu.cloud2);
	al_destroy_bitmap(game->menu.pinkcloud_bitmap);
	al_destroy_bitmap(game->menu.rain_bitmap);
	al_destroy_bitmap(game->menu.mountain);
	al_destroy_bitmap(game->menu.pie_bitmap);
	al_destroy_font(game->menu.font_title);
	al_destroy_font(game->menu.font_subtitle);
	al_destroy_font(game->menu.font);
	al_destroy_font(game->menu.font_selected);
	al_destroy_sample_instance(game->menu.music);
	al_destroy_sample_instance(game->menu.rain_sound);
	al_destroy_sample_instance(game->menu.click);
	al_destroy_sample(game->menu.sample);
	al_destroy_sample(game->menu.rain_sample);
	al_destroy_sample(game->menu.click_sample);
}

void Menu_Load(struct Game *game) {
	game->menu.cloud_position = 100;
	game->menu.cloud2_position = 100;
	game->menu.options = false;
	game->menu.selected = 0;

	al_play_sample_instance(game->menu.music);
	al_play_sample_instance(game->menu.rain_sound);
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=255; fadeloop>=0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		Menu_Draw(game);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
	Menu_Draw(game);
}

int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
		al_play_sample_instance(game->menu.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
		al_play_sample_instance(game->menu.click);
	} else if ((!game->menu.options) && (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)) || (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) {
		al_play_sample_instance(game->menu.click);
		return 1;
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==0)) {
		al_play_sample_instance(game->menu.click);
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_INTRO;
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==2)) {
		al_play_sample_instance(game->menu.click);
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_ABOUT;
	} else if (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==1)) || (((game->menu.options) && ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) || (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3))))) {
		al_play_sample_instance(game->menu.click);
		game->menu.options=!game->menu.options;
		game->menu.selected=0;
		PrintConsole(game, "options state changed %d", game->menu.options);
	} else if ((game->menu.options) && (game->menu.selected==2)) {
		if ((game->music) && (game->fx)) { game->music=0; SetConfigOption("SuperDerpy", "music", "0");
			al_detach_mixer(game->audio.music);
		}
		else if (game->fx) { game->music=1; game->fx=0; SetConfigOption("SuperDerpy", "music", "1"); SetConfigOption("SuperDerpy", "fx", "0");
			al_attach_mixer_to_mixer(game->audio.music, game->audio.mixer);
			al_detach_mixer(game->audio.fx);
		}
		else if (game->music) { game->music=0; SetConfigOption("SuperDerpy", "music", "0");
			al_detach_mixer(game->audio.music);
		}
		else { game->music=1; game->fx=1; SetConfigOption("SuperDerpy", "music", "1"); SetConfigOption("SuperDerpy", "fx", "1");
			al_attach_mixer_to_mixer(game->audio.fx, game->audio.mixer);
			al_attach_mixer_to_mixer(game->audio.music, game->audio.mixer);
		}
		al_play_sample_instance(game->menu.click);
	}
	if (game->menu.selected==-1) game->menu.selected=3;
	if (game->menu.selected==4) game->menu.selected=0;
	return 0;
}
