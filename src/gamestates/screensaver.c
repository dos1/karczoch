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
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "screensaver.h"

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct EmptyResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	data->blink_counter++;
	if (data->blink_counter >= 40) {
		data->blink_counter = 0;
		data->chosen++;
	}

	for (int i=0; i<1000; i++) {
		//data->windowses[i].x += ((data->windowses[i].direction * 2) - 1) * 0.003;
		data->windowses[i].x += ((data->windowses[i].direction * 2) - 1) * data->windowses[i].size * 0.01;
		data->windowses[i].size += data->windowses[i].dsize;

		//PrintConsole(game, "window %d, x: %f, size: %f", i, data->windowses[i].x, data->windowses[i].size);

		if (fabs(data->windowses[i].x) > 1.0) {
			data->windowses[i].color = rand() % 14;
			data->windowses[i].size = 0.1 + ((float)rand()/(float)RAND_MAX)  * 0.1;
			data->windowses[i].dsize = (0.01 + ((float)rand()/(float)RAND_MAX) * 0.02) * 0.5;
			data->windowses[i].x = (((float)rand()/(float)RAND_MAX) * 2 - 1);
			data->windowses[i].direction = data->windowses[i].x > 0;

			//int x = rand() % 480, y = rand() % 380;

			data->windowses[i].a = ((float)rand()/(float)RAND_MAX) * 10 - 5;
			data->windowses[i].b = 0;
		}
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

for (int i=0; i<300; i++) {

ALLEGRO_COLOR color;

switch (data->windowses[i].color) {
	case 0:
		color=al_map_rgb(0,0,170);
		break;
	case 1:
		color=al_map_rgb(77,77,249);
		break;
	case 2:
		color=al_map_rgb(0,170,0);
		break;
	case 3:
		color=al_map_rgb(85,255,85);
		break;
	case 4:
		color=al_map_rgb(0, 170, 170);
		break;
	case 5:
		color=al_map_rgb(85, 255, 255);
		break;
	case 6:
		color=al_map_rgb(170,0,0);
		break;
	case 7:
		color=al_map_rgb(255,85,85);
		break;
	case 8:
		color=al_map_rgb(170,0,170);
		break;
	case 9:
		color=al_map_rgb(255,85,255);
		break;
	case 10:
		color=al_map_rgb(170,85,0);
		break;
	case 11:
		color=al_map_rgb(255,255,85);
		break;
	case 12:
		color=al_map_rgb(170,170,170);
		break;
	case 13:
		color=al_map_rgb(255,255,255);
		break;
}

  int x = data->windowses[i].x * 480 + 480 / 2;
	int y = (data->windowses[i].a * data->windowses[i].x + data->windowses[i].b) * 360 + 360 / 2;
	al_draw_tinted_scaled_bitmap(data->threef, color, 0, 0, 44, 36, x, y,data->windowses[i].size * 44, data->windowses[i].size * 36, 0);
}
    //al_draw_bitmap(data->threef, 0, 0, 0);

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
	if(data->chosen == 2) {

		if (!game->data)
			al_draw_scaled_bitmap(data->floppyinuse, 0, 0, 240, 180, 240, 120, 80, 60, 0);
}

	//al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 120, 80, 60, 0);
	//al_use_transform(&game->projection);
}

void Gamestate_ProcessEvent(struct Game *game, struct EmptyResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) || (ev->type==ALLEGRO_EVENT_MOUSE_AXES) || (ev->type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)) {
		game->data = (void*)2;
		SwitchGamestate(game, "screensaver", "gaem"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}

}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct EmptyResources *data = malloc(sizeof(struct EmptyResources));
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/wingding.ttf"), 8,  ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_NO_AUTOHINT);
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->threef = al_load_bitmap(GetDataFilePath(game, "winlogo.png"));
	data->bitmap = al_create_bitmap(480, 360);

	data->shader = al_create_shader(ALLEGRO_SHADER_GLSL);
	PrintConsole(game, "VERTEX: %d", al_attach_shader_source_file(data->shader, ALLEGRO_VERTEX_SHADER, "data/ex_shader_vertex.glsl"));
	PrintConsole(game, "%s", al_get_shader_log(data->shader));
	PrintConsole(game, "PIXEL: %d", al_attach_shader_source_file(data->shader, ALLEGRO_PIXEL_SHADER, "data/ex_shader_pixel.glsl"));
PrintConsole(game, "%s", al_get_shader_log(data->shader));
  al_build_shader(data->shader);

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

	for (int i=0; i<1000; i++) {
		data->windowses[i].color = rand() % 14;
		data->windowses[i].size = 0.1 + ((float)rand()/(float)RAND_MAX)  * 0.1;
		data->windowses[i].dsize = (0.01 + ((float)rand()/(float)RAND_MAX) * 0.02) * 0.5;
		data->windowses[i].x = (((float)rand()/(float)RAND_MAX) * 2 - 1 );
		data->windowses[i].direction = data->windowses[i].x > 0;

		//int x = rand() % 480, y = rand() % 380;

		data->windowses[i].a = ((float)rand()/(float)RAND_MAX) * 10 - 5;
		data->windowses[i].b = 0;
	}
}

void Gamestate_Stop(struct Game *game, struct EmptyResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
}

// Ignore those for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Pause(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Resume(struct Game *game, struct EmptyResources* data) {}
