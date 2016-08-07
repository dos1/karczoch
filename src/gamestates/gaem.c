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
#include <sys/time.h>
#include "gaem.h"

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct EmptyResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	data->blink_counter++;
	if (data->blink_counter >= 40) {
		data->blink_counter = 0;
	}

	if (data->noice) {
		data->noice++;
	}
	if (data->noice > 10) {
		data->noice=0;
	}

	AnimateCharacter(game, data->mom, 1);
}

bool ShowSplash(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->splash = true;
	}
	return true;
}

bool ShowIE(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->ie = true;
		data->splash = false;
	}
	return true;
}

bool Unbusy(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->busy = false;
	}
	return true;
}
bool Wait(struct Game *game, struct TM_Action *action, enum TM_ActionState state);
bool Talk(struct Game *game, struct TM_Action *action, enum TM_ActionState state);
bool Try(struct Game *game, struct TM_Action *action, enum TM_ActionState state);
bool Leave(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Ring(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
//		SelectSpritesheet(game, data->mom, "")

		if (data->connected || data->connecting) {
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 1000, "wait");
			TM_AddBackgroundAction(data->timeline, Try, TM_AddToArgs(NULL, 1, data), 11000, "try");
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 13000, "wait");
			TM_AddBackgroundAction(data->timeline, Leave, TM_AddToArgs(NULL, 1, data), 14000, "leave");

		} else {
			al_play_sample_instance(data->ring);
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 3000, "wait");
			TM_AddBackgroundAction(data->timeline, Talk, TM_AddToArgs(NULL, 1, data), 3500, "talk");
		}
	}
	return true;
}

bool Wait(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		al_stop_sample_instance(data->ring);
		SelectSpritesheet(game, data->mom, "wait");
	}
	return true;
}

bool Sit(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "sit");

	}
	return true;
}

bool Talk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "talk");
		TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 15000, "wait");
		TM_AddBackgroundAction(data->timeline, Sit, TM_AddToArgs(NULL, 1, data), 17000, "sit");
		TM_AddBackgroundAction(data->timeline, Ring, TM_AddToArgs(NULL, 1, data), 40000, "ring");

	}
	return true;
}

bool Try(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "try");

	}
	return true;
}

bool Leave(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "left");

	}
	return true;
}

bool Connect(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		if (data->connecting) {
			data->connecting=false;
			data->connected=true;
		}
	}
	return true;
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


char zegarek[64];
struct timeval  tv;
    struct tm       *tm;
gettimeofday(&tv, NULL);
    if((tm = localtime(&tv.tv_sec)) != NULL)
		{
strftime(zegarek, 64, "%l:%M %p", tm);
		}

		if (data->ie) {
			al_draw_bitmap(data->iebitmap, 0, 0, 0);


		} else {
		al_draw_bitmap(data->threef, 0, 0, 0);

		al_draw_text(data->tahoma, al_map_rgb(255,255,255), 85, 48, ALLEGRO_ALIGN_LEFT, "Dial-up");
}
		al_draw_text(data->tahoma, al_map_rgb(0,0,0), 467, 342, ALLEGRO_ALIGN_RIGHT, zegarek);

		if (!data->ie) {
			if (data->connected) {
				al_draw_bitmap(data->donebmp, 0,0 ,0);
			} else if (data->connecting) {
				al_draw_bitmap(data->conbmp, 0,0 ,0);
			}
		}

		if (data->splash) {
			al_draw_bitmap(data->iesplash, 0, 0, 0);
		}

		//if (data->blink_counter < 50) {
//		al_draw_text(data->font, al_map_rgb(255,255,255), 240 / 2, game->viewport.height - 16,
//		             ALLEGRO_ALIGN_CENTRE, "Teleturniej!");
//	}
al_draw_bitmap(data->busy ? data->busycur : data->cursor, data->mousex * 2, data->mousey * 2, 0);
    al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);
//	al_draw_bitmap(data->bitmap, 0, 0, 0);
	al_draw_scaled_bitmap(data->bitmap, 0, 0, 480, 360, 0, 0, 240, 180, 0);
	al_use_shader(NULL);

	al_set_target_bitmap(data->bitmap);
	al_clear_to_color(al_map_rgb(35, 31, 32));
	if (data->noice) { WhiteNoise(game); } else {
		al_draw_bitmap(data->room1, 0, 0, 0);
		DrawCharacter(game, data->mom, al_map_rgb(255,255,255), 0);
	}


	al_set_target_backbuffer(game->display);

	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);
	  al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 0, 80, 60, 0);
		al_use_shader(NULL);

		al_set_target_bitmap(data->bitmap);
		if (!data->noice) {
			al_clear_to_color(al_map_rgb(35, 31, 32));

			al_draw_bitmap(data->room2, 0, 0, 0);
		}
		al_set_target_backbuffer(game->display);



		al_use_shader(data->shader);
		al_set_shader_int("scaleFactor", 1);
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
	TM_HandleEvent(data->timeline, ev);
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		SwitchGamestate(game, "gaem", "off"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}
 if (ev->type == ALLEGRO_EVENT_MOUSE_AXES) {
	 data->mousex = ev->mouse.x / (al_get_display_width(game->display) / game->viewport.width);
	 data->mousey = ev->mouse.y / (al_get_display_height(game->display) / game->viewport.height);
 }
 if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
	 if (data->busy) return;
	 if ((data->mousex * 2 > 15) && (data->mousey * 2 > 70) && (data->mousex * 2 < 60) && (data->mousey * 2 < 140)) {
		 if (!data->ie) {
			 data->busy = true;
			 TM_AddBackgroundAction(data->timeline, ShowSplash, TM_AddToArgs(NULL, 1, data), 500, "splash");
			 TM_AddBackgroundAction(data->timeline, ShowIE, TM_AddToArgs(NULL, 1, data), 2300, "ie");
			 TM_AddBackgroundAction(data->timeline, Unbusy, TM_AddToArgs(NULL, 1, data), 2500, "cursor");
		 }
	 } else if ((data->mousex * 2 > 15) && (data->mousey * 2 < 70) && (data->mousex * 2 < 60))  {
		 if (!data->ie) {
			 SwitchGamestate(game, "gaem", "bsod");
		 }
	 } else if ((data->mousex * 2 > 70) && (data->mousey * 2 < 70) && (data->mousex * 2 < 130))  {
		 if (!data->ie) {
			 data->connecting = true;
			 TM_AddBackgroundAction(data->timeline, Connect, TM_AddToArgs(NULL, 1, data), 9000, "connect");
	al_play_sample_instance(data->modem);
		 }
	 }

	 if ((data->mousex * 2 > 425) && (data->mousey * 2 < 20)) {
		 if (data->ie) {
			 data->ie=false;
		 }
	 }

	 if ((data->mousex * 2 < 60) && (data->mousey * 2 > 333)) {
		 SwitchGamestate(game, "gaem", "off");

}
//	 data->ie=true;
 }
 if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_S)) {
SwitchGamestate(game, "gaem", "screensaver");
  }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct EmptyResources *data = malloc(sizeof(struct EmptyResources));
	data->font = al_load_font(GetDataFilePath(game, "fonts/PerfectDOSVGA437.ttf"), 16, ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_NO_AUTOHINT);
	data->tahoma = al_load_ttf_font(GetDataFilePath(game, "fonts/micross.ttf"), 11, ALLEGRO_TTF_MONOCHROME);

	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->threef = al_load_bitmap(GetDataFilePath(game, "mpv-shot0003.png"));
	data->bitmap = al_create_bitmap(480, 360);
	data->room1 = al_load_bitmap(GetDataFilePath(game, "room1.png"));
	data->room2 = al_load_bitmap(GetDataFilePath(game, "room2.png"));
	data->iebitmap = al_load_bitmap(GetDataFilePath(game, "ie.png"));
	data->iesplash = al_load_bitmap(GetDataFilePath(game, "iesplash.png"));

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

	data->conbmp = al_load_bitmap(GetDataFilePath(game, "connecting.png"));
	data->donebmp = al_load_bitmap(GetDataFilePath(game, "connected.png"));


	data->cursor = al_load_bitmap(GetDataFilePath(game, "cursor.png"));
	data->busycur = al_load_bitmap(GetDataFilePath(game, "busy.png"));

	data->timeline = TM_Init(game, "gaem");
	data->lines = TM_Init(game, "lines");

	data->ie=false;
	data->splash=false;
	data->busy=false;

	data->mom = CreateCharacter(game, "mom");

	RegisterSpritesheet(game, data->mom, "sit");
	RegisterSpritesheet(game, data->mom, "left");
	RegisterSpritesheet(game, data->mom, "wait");
	RegisterSpritesheet(game, data->mom, "try");
	RegisterSpritesheet(game, data->mom, "talk");
	LoadSpritesheets(game, data->mom);

	data->startup_sample = al_load_sample(GetDataFilePath(game, "startup.flac"));

	data->startup = al_create_sample_instance(data->startup_sample);
	al_attach_sample_instance_to_mixer(data->startup, game->audio.fx);

	data->ring_sample = al_load_sample(GetDataFilePath(game, "ring.flac"));

	data->ring = al_create_sample_instance(data->ring_sample);
	al_attach_sample_instance_to_mixer(data->ring, game->audio.fx);

	data->modem_sample = al_load_sample(GetDataFilePath(game, "modem.flac"));

	data->modem = al_create_sample_instance(data->modem_sample);
	al_attach_sample_instance_to_mixer(data->modem, game->audio.fx);

	return data;
}

void Gamestate_Unload(struct Game *game, struct EmptyResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_font(data->font);

	TM_Destroy(data->timeline);
	TM_Destroy(data->lines);
	free(data);
}

void Gamestate_Start(struct Game *game, struct EmptyResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.

	data->blink_counter = 0;

	data->mousex = 240/2;
	data->mousey = 180/2;

	data->chosen = 1;

	data->noice = 1;
	SelectSpritesheet(game, data->mom, "sit");
	SetCharacterPosition(game, data->mom, 0, 0, 0);

	if (game->data != (void*)2) {
		al_play_sample_instance(data->startup);
	}
	TM_AddBackgroundAction(data->timeline, Ring, TM_AddToArgs(NULL, 1, data), 12000, "ring");
	//TM_AddAction(data->timeline, Sit, TM_AddToArgs(NULL, 1, data), "sit");

}

void Gamestate_Stop(struct Game *game, struct EmptyResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
	al_stop_sample_instance(data->ring);
	al_stop_sample_instance(data->modem);
	al_stop_sample_instance(data->startup);
}

// Ignore those for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Pause(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Resume(struct Game *game, struct EmptyResources* data) {}
