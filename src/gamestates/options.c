/*! \file empty.c
 *  \brief Empty gamestate.
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

#include <libsuperderpy.h>
#include <allegro5/allegro_primitives.h>
#include "options.h"

int Gamestate_ProgressCount = 3; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct EmptyResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	data->blink_counter++;
	if (data->blink_counter >= 40) {
		data->blink_counter = 0;
	}
}

void Gamestate_Draw(struct Game *game, struct EmptyResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
	  //PrintConsole(game, "USE: %d", al_use_shader(data->shader));

	//ALLEGRO_TRANSFORM trans;
	//al_identity_transform(&trans);
	//al_use_transform(&trans);


	al_clear_to_color(al_map_rgb(0,0,0));

al_set_target_bitmap(data->bitmap);
al_clear_to_color(al_map_rgb(35, 31, 32));


    //al_draw_bitmap(data->threef, 0, 0, 0);
    al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 3, 0, "Karczoch Windows 95 Startup Menu");
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 19, 0, "═════════════════════════════════");
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 35, 0, "");
		al_draw_filled_rectangle(2, 51 + (data->chosen - 1) * 16, 300, 67 + (data->chosen - 1) * 16, al_map_rgb(173, 173, 173));

		al_draw_text(data->font, data->chosen == 1 ? al_map_rgb(35, 31, 32) :  al_map_rgb(173, 173, 173), 3, 51, 0, game->config.fullscreen ? "1. Fullscreen: on" : "1. Fullscreen: off");
		al_draw_text(data->font, data->chosen == 2 ? al_map_rgb(35, 31, 32) :  al_map_rgb(173, 173, 173), 3, 67, 0, game->config.fx ? "2. Sounds: on" : "2. Sounds: off");
		al_draw_text(data->font, data->chosen == 3 ? al_map_rgb(35, 31, 32) :  al_map_rgb(173, 173, 173), 3, 83, 0, game->config.music ? "3. Music: on" : "3. Music: off");
		al_draw_text(data->font, data->chosen == 4 ? al_map_rgb(35, 31, 32) :  al_map_rgb(173, 173, 173), 3, 99, 0, data->homealone ? "4. Home alone mode: on" : "4. Home alone mode: off");
		al_draw_text(data->font, data->chosen == 5 ? al_map_rgb(35, 31, 32) :  al_map_rgb(173, 173, 173), 3, 99+16, 0, "5. Back");
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 115+16, 0, "");
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 131+16, 0, "Enter your choice:  ");
		if (data->blink_counter < 20) {
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 132+16, 0, "                   _");
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 133+16, 0, "                   _");
}
		char* lala = strdup("                   1");
		lala[strlen(lala)-1] += data->chosen - 1;
		al_draw_text(data->font, al_map_rgb(173, 173, 173), 3, 131+16, 0, lala);
		free(lala);

		//if (data->blink_counter < 50) {
//		al_draw_text(data->font, al_map_rgb(255,255,255), 240 / 2, game->viewport.height - 16,
//		             ALLEGRO_ALIGN_CENTRE, "Teleturniej!");
//	}

		al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);
//	al_draw_bitmap(data->bitmap, 0, 0, 0);
	al_draw_scaled_bitmap(data->bitmap, 0, 0, 480, 360, 0, 0, 240, 180, 0);
	al_use_shader(NULL);

	al_set_target_bitmap(data->bitmap);
	al_clear_to_color(al_map_rgb(35, 31, 32));
	al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);
	al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 0, 80, 60, 0);
	al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 60, 80, 60, 0);
	//al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 120, 80, 60, 0);
	al_use_shader(NULL);

	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 0, 0, 240, 180, 0);
	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 0, 80, 60, 0);
	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 60, 80, 60, 0);

	al_draw_scaled_bitmap(data->floppy, 0, 0, 240, 180, 240, 120, 80, 60, 0);


	//al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 120, 80, 60, 0);
	//al_use_transform(&game->projection);
}

void Gamestate_ProcessEvent(struct Game *game, struct EmptyResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		SwitchGamestate(game, "options", "menu"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}

	if ((ev->type==ALLEGRO_EVENT_KEY_CHAR)) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_UP) {
			data->chosen--;
			if (data->chosen < 1) data->chosen = 5;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_DOWN) {
			data->chosen++;
			if (data->chosen > 5) data->chosen = 1;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_1) {
			data->chosen=1;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_2) {
			data->chosen=2;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_3) {
			data->chosen=3;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_4) {
			data->chosen=4;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_5) {
			data->chosen=5;
		}
		data->blink_counter=0;
	}
	if (ev->type==ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_ENTER) {
			if (data->chosen == 1) {
			//	SwitchGamestate(game, "menu", "gaem");
				game->config.fullscreen = !game->config.fullscreen;
				if (game->config.fullscreen)
					  SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
				else
					  SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
				al_set_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, game->config.fullscreen);
				SetupViewport(game);
				PrintConsole(game, "Fullscreen toggled");
			}
			if (data->chosen == 2) {
				game->config.fx = game->config.fx ? 0 : 10;
				SetConfigOption(game, "SuperDerpy", "fx", game->config.fx ? "10" : "0");
				al_set_mixer_gain(game->audio.fx, game->config.fx/10.0);
			//	SwitchGamestate(game, "menu", "options");
			}
			if (data->chosen == 3) {
				game->config.music = game->config.music ? 0 : 10;
				SetConfigOption(game, "SuperDerpy", "music", game->config.music ? "10" : "0");
				al_set_mixer_gain(game->audio.music, game->config.music/10.0);
			//	SwitchGamestate(game, "menu", "abount");
			}
			if (data->chosen == 4) {
				data->homealone = !data->homealone;
				SetConfigOption(game, "KARCZOCH", "homealone", data->homealone ? "1" : "0");
			}
			if (data->chosen == 5) {
				SwitchGamestate(game, "options", "menu");
			}
		}
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct EmptyResources *data = malloc(sizeof(struct EmptyResources));
	data->font = al_load_font(GetDataFilePath(game, "fonts/PerfectDOSVGA437.ttf"), 16, 0);
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->threef = al_load_bitmap(GetDataFilePath(game, "mpv-shot0003.png"));
	data->bitmap = al_create_bitmap(480, 360);
	(*progress)(game);

	data->shader = al_create_shader(ALLEGRO_SHADER_GLSL);
	PrintConsole(game, "VERTEX: %d", al_attach_shader_source_file(data->shader, ALLEGRO_VERTEX_SHADER, "data/ex_shader_vertex.glsl"));
	PrintConsole(game, "%s", al_get_shader_log(data->shader));
	PrintConsole(game, "PIXEL: %d", al_attach_shader_source_file(data->shader, ALLEGRO_PIXEL_SHADER, "data/ex_shader_pixel.glsl"));
PrintConsole(game, "%s", al_get_shader_log(data->shader));
  al_build_shader(data->shader);
	(*progress)(game);

	data->screen = al_load_bitmap(GetDataFilePath(game, "screen.png"));

	data->floppy = al_load_bitmap(GetDataFilePath(game, "floppy.png"));
	data->floppytaken = al_load_bitmap(GetDataFilePath(game, "floppytaken.png"));
	data->floppyinuse = al_load_bitmap(GetDataFilePath(game, "floppyinuse.png"));

	return data;
}

void Gamestate_Unload(struct Game *game, struct EmptyResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Start(struct Game *game, struct EmptyResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	data->blink_counter = 0;

	data->chosen = 1;

	data->homealone = GetConfigOptionDefault(game, "KARCZOCH", "homealone", "0")[0] == '1';
}

void Gamestate_Stop(struct Game *game, struct EmptyResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
}

// Ignore those for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Pause(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Resume(struct Game *game, struct EmptyResources* data) {}
