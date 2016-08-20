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

#include "../common.h"
#include <libsuperderpy.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "bsod.h"

int Gamestate_ProgressCount = 2; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct EmptyResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	if (data->blink_counter < 80)
	data->blink_counter++;
	//if (data->blink_counter >= 40) {
	//	data->blink_counter = 0;
	//}
}

void Gamestate_Draw(struct Game *game, struct EmptyResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
	  //PrintConsole(game, "USE: %d", al_use_shader(data->shader));

	//ALLEGRO_TRANSFORM trans;
	//al_identity_transform(&trans);
	//al_use_transform(&trans);

char* text = "Program has performed an illegal operation and will be terminated.";
  //al_clear_to_color(al_map_rgb(0,0,0));

al_set_target_bitmap(data->bitmap);


//al_set_target_backbuffer(game->display);
al_clear_to_color(al_map_rgb(0,0,170));

if (data->blink_counter > 40) {
	const char *header = "KARCZOCH WINDOWS";

	al_draw_filled_rectangle(game->viewport.width/2 - al_get_text_width(data->font, header)/2 - 4  + (960-320)/2, (int)(game->viewport.height * 1.32), 4 + game->viewport.width/2 + al_get_text_width(data->font, header)/2 + (960-320)/2, (int)(game->viewport.height * 1.32) + al_get_font_line_height(data->font), al_map_rgb(170,170,170));

	al_draw_text(data->font, al_map_rgb(0, 0, 170), game->viewport.width/2 + (960-320)/2, (int)(game->viewport.height * 1.32), ALLEGRO_ALIGN_CENTRE, header);

	const char *header2 = "A fatal exception 0xD3RP has occured at 0028:M00F11NZ in GST SD(01) +";

	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2, (int)(game->viewport.height * 1.32+2*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_CENTRE, header2);
	al_draw_textf(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2 - al_get_text_width(data->font, header2)/2, (int)(game->viewport.height * 1.32+3*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_LEFT, "%p and system just doesn't know what went wrong.", game);

	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2, (int)(game->viewport.height * 1.32+5*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_CENTRE, text);

	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2 - al_get_text_width(data->font, header2)/2, (int)(game->viewport.height * 1.32+7*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to terminate this error.");
	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2 - al_get_text_width(data->font, header2)/2, (int)(game->viewport.height * 1.32+8*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to destroy all muffins in the world.");
	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2 - al_get_text_width(data->font, header2)/2, (int)(game->viewport.height * 1.32+9*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_LEFT, "* Just kidding, please press any key anyway.");


	  al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2 - al_get_text_width(data->font, header2)/2, (int)(game->viewport.height * 1.32+11*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_LEFT, "Anything I can do to help?");

		al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2 + (960-320)/2, (int)(game->viewport.height * 1.32+13*al_get_font_line_height(data->font)*1.25), ALLEGRO_ALIGN_CENTRE, "Press any key to continue _");

		//if (data->blink_counter < 50) {
	//	al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255),  game->viewport.width/2 - (320-240)/2, game->viewport.height - 16,
	//	             ALLEGRO_ALIGN_CENTRE, "Teleturniej!");
//	}
}
    al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 2);

	//al_draw_bitmap(data->bitmap, 0, 0, 0);
	al_draw_scaled_bitmap(data->bitmap, 0, 0, 1920/2, 1440/2, 0, 0, 240, 180, 0);
	al_use_shader(NULL);

	al_set_target_bitmap(data->threef);
	al_clear_to_color(al_map_rgb(35, 31, 32));
	al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);

	al_draw_scaled_bitmap(data->threef, 0, 0, 240, 180, 240, 0, 80, 60, 0);
	al_draw_scaled_bitmap(data->threef, 0, 0, 240, 180, 240, 60, 80, 60, 0);
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
		UnloadGamestate(game, "bsod"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	} else if (ev->type==ALLEGRO_EVENT_KEY_DOWN) {
//		if (game->data == (void*)2) {
//			SwitchGamestate(game, "bsod", "off");
//		} else {
//			game->data = 0;
		  ChangeCurrentGamestate(game, "intro");
//		}
		game->data->data = NULL;
	}

}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct EmptyResources *data = malloc(sizeof(struct EmptyResources));
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/PerfectDOSVGA437.ttf"), 16, ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_NO_AUTOHINT);

	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->threef = al_create_bitmap(480, 360);
	data->bitmap = al_create_bitmap(960, 720);

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
}

void Gamestate_Stop(struct Game *game, struct EmptyResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
}

// Ignore those for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Pause(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Resume(struct Game *game, struct EmptyResources* data) {}
