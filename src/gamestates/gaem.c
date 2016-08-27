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
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro_ttf.h>
#include "gaem.h"

char *anims[] = { "party", "cons", "baby", "at", "cons2","homepage", "mchammer", "mercy", "siren","spooky", "hacker", "counter", "new" };

int Gamestate_ProgressCount = 16; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct EmptyResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	data->blink_counter++;
	if (data->blink_counter >= 40) {
		data->blink_counter = 0;
	}
data->screensaver ++;

  if (data->noice) {
		data->noice++;
	}
	if (data->noice > 10) {
		data->noice=0;
	}
	AnimateCharacter(game, data->baby, 1);

	AnimateCharacter(game, data->mom, 1);

	if (data->screensaver == 60*30) {
		StartGamestate(game, "screensaver");
	}

	if (data->ie && data->ieconnected && !data->lost) {
		data->watched++;
		if (data->loading < 60*10){ data->loading+=1; }

		if (data->loading == 60*4) {
			int r = rand() % 4;

			PrintConsole(game, "MUUUSIC: %d", r);
			  switch (r) {
					case 0:
						al_set_audio_stream_playing(data->midi1, true);
						al_rewind_audio_stream(data->midi1);
						break;
					case 1:
						al_set_audio_stream_playing(data->midi2, true);
						al_rewind_audio_stream(data->midi2);
						break;
					case 2:
						al_set_audio_stream_playing(data->midi3, true);
						al_rewind_audio_stream(data->midi3);
						break;
					default:
						al_set_audio_stream_playing(data->midi4, true);
						al_rewind_audio_stream(data->midi4);
						break;
				}

		}
	}

}

bool ShowSplash(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->splash = true;
	}
	return true;
}


bool ShowContent(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->content = true;
	}
	return true;
}

bool ShowIE(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->ie = true;
		data->splash = false;
		data->content = false;
		data->loading = 0;
		data->ieconnected = data->connected;

		int r = rand();

		//r = data->debug;
		//data->debug++;
		SelectSpritesheet(game, data->baby, anims[r % (sizeof(anims) / sizeof(anims[0]))]);

	}
	return true;
}

bool Unbusy(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->busy = false;
		al_stop_sample_instance(game->data->data3);

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


		} else {
			al_play_sample_instance(data->ring);
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 3000, "wait");
			TM_AddBackgroundAction(data->timeline, Talk, TM_AddToArgs(NULL, 1, data), 3500, "talk");
		}
	}
	return true;
}
bool Lose(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
	//	al_stop_sample_instance(data->ring);
	//	SelectSpritesheet(game, data->mom, "wait");
		data->lost=true;
		//al_stop_sample_instance(data->modem);
		//al_start_sample_instance(data->yadayada);
	}
	return true;
}


bool Wait(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		al_stop_sample_instance(data->ring);
		SelectSpritesheet(game, data->mom, "wait");
		data->incall=false;
	}
	return true;
}

bool Sit(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "sit");
		data->incall=false;

	}
	return true;
}

bool Talk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "talk");
		data->incall = true;
		if (data->connected || data->connecting) {
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 2000, "wait");
			TM_AddBackgroundAction(data->timeline, Leave, TM_AddToArgs(NULL, 1, data), 3000, "Leave");

		} else {
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 15000, "wait");
			TM_AddBackgroundAction(data->timeline, Sit, TM_AddToArgs(NULL, 1, data), 17000, "sit");
			TM_AddBackgroundAction(data->timeline, Ring, TM_AddToArgs(NULL, 1, data), 40000, "ring");
		}
	}
	return true;
}

bool Try(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "try");
		if (data->connected || data->connecting) {
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 2000, "wait");
			TM_AddBackgroundAction(data->timeline, Leave, TM_AddToArgs(NULL, 1, data), 3000, "leave");
		} else {
			TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 2000, "wait");
			TM_AddBackgroundAction(data->timeline, Ring, TM_AddToArgs(NULL, 1, data), 8000, "ring");

		}
	}
	return true;
}

bool Leave(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct EmptyResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, data->mom, "left");
		data->incall=false;
		TM_AddBackgroundAction(data->timeline, Lose, TM_AddToArgs(NULL, 1, data), 1500, "lose");

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

float min(float a, float b) {
	return (a > b) ? b : a;
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
    struct tm       *tm;
time_t rawtime = time(NULL);
    if((tm = localtime(&rawtime)) != NULL)
		{
#ifdef ALLEGRO_WINDOWS
	strftime(zegarek, 64, "%I:%M %p", tm);
#else
			strftime(zegarek, 64, "%l:%M %p", tm);
#endif
		}

		if (data->ie) {
			if (data->ieconnected) {
				al_set_target_bitmap(data->gif);
				al_clear_to_color(al_map_rgb(255,255,255));
				DrawCharacter(game, data->baby, al_map_rgb(255,255,255), 0);

				al_set_target_bitmap(data->bitmap);
				al_clear_to_color(al_map_rgb(255,255,255));
				float progress = ceil(min(1.0, data->loading / (60*10.0)) * 30) / 30.0;
				al_draw_bitmap_region(data->gif, 0, 0, al_get_bitmap_width(data->gif), al_get_bitmap_height(data->gif) * progress, 2, 116, 0);
				al_draw_bitmap(data->iebitmap, 0, 0, 0);

				al_draw_text(data->tahoma, al_map_rgb(0,0,0), 75, 93, ALLEGRO_ALIGN_LEFT, "http://dosowisko.net/karczoch/");


			} else {
				al_draw_bitmap(data->iebitmap2, 0, 0, 0);
			}


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

	if (!data->noice) {
	al_use_shader(data->shader);
	al_set_shader_int("scaleFactor", 1);
	}
	  al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 0, 80, 60, 0);
		al_use_shader(NULL);

		al_set_target_bitmap(data->bitmap);
		if (!data->noice) {
			al_clear_to_color(al_map_rgb(35, 31, 32));

			al_draw_bitmap(data->room2, 0, 0, 0);
		}
		al_set_target_backbuffer(game->display);


if (!data->noice) {
	  al_use_shader(data->shader);
		al_set_shader_int("scaleFactor", 1);
}
    al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 60, 80, 60, 0);
	//al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 120, 80, 60, 0);
	al_use_shader(NULL);

	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 0, 0, 240, 180, 0);
	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 0, 80, 60, 0);
	al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 60, 80, 60, 0);

	al_draw_scaled_bitmap(data->floppy, 0, 0, 240, 180, 240, 120, 80, 60, 0);


	if (data->lost) {
		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width / 2, game->viewport.height / 2 - 10, ALLEGRO_ALIGN_CENTER, "YOU LOST!!!!1");
		al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width / 2 + 80, game->viewport.height / 2 + 35, ALLEGRO_ALIGN_CENTER, "press ESC");

		char score[255];
		snprintf(score, 255, "watched GIFs for: %lld sec", data->watched / 60);
		al_draw_text(data->font, al_map_rgb(255,255,255), 5, game->viewport.height / 2 + 70, ALLEGRO_ALIGN_LEFT, score);

	}

	//al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 120, 80, 60, 0);
	//al_use_transform(&game->projection);
}

void Gamestate_ProcessEvent(struct Game *game, struct EmptyResources* data, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(data->timeline, ev);
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		game->data->data = NULL;
		ChangeCurrentGamestate(game, data->lost ? "intro" : "off"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}
 if (ev->type == ALLEGRO_EVENT_MOUSE_AXES) {
	 data->screensaver=0;
	 data->mousex = ev->mouse.x / (al_get_display_width(game->display) / game->viewport.width);
	 data->mousey = ev->mouse.y / (al_get_display_height(game->display) / game->viewport.height);
 }
 if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
	 data->screensaver=0;
	 if (data->busy) return;
	 if ((data->mousex * 2 > 15) && (data->mousey * 2 > 70) && (data->mousex * 2 < 60) && (data->mousey * 2 < 140)) {
		 if (!data->ie) {
			 al_play_sample_instance(game->data->data3);

			 data->busy = true;
			 TM_AddBackgroundAction(data->timeline, ShowSplash, TM_AddToArgs(NULL, 1, data), 500, "splash");
			 TM_AddBackgroundAction(data->timeline, ShowIE, TM_AddToArgs(NULL, 1, data), 2300, "ie");
			 TM_AddBackgroundAction(data->timeline, Unbusy, TM_AddToArgs(NULL, 1, data), 2500, "cursor");
		 }
	 } else if ((data->mousex * 2 > 15) && (data->mousey * 2 < 70) && (data->mousex * 2 < 60))  {
		 if (!data->ie) {
			 ChangeCurrentGamestate(game, "bsod");
		 }
	 } else if ((data->mousex * 2 > 70) && (data->mousey * 2 < 70) && (data->mousex * 2 < 130))  {
		 if (!data->ie) {
			 data->connecting = true;
			 if (data->incall) {
				     TM_CleanBackgroundQueue(data->timeline);
						 TM_AddBackgroundAction(data->timeline, Wait, TM_AddToArgs(NULL, 1, data), 1000, "wait");
						 TM_AddBackgroundAction(data->timeline, Leave, TM_AddToArgs(NULL, 1, data), 1500, "leave");
			    }

			 TM_AddBackgroundAction(data->timeline, Connect, TM_AddToArgs(NULL, 1, data), 9000, "connect");
			 al_set_sample_instance_gain(data->modem, 0.7);
	al_play_sample_instance(data->modem);
		 }
	 }

	 if ((data->mousex * 2 > 425) && (data->mousey * 2 < 20)) {
		 if (data->ie) {
			 data->ie=false;
			 al_set_audio_stream_playing(data->midi1, false);
			 al_set_audio_stream_playing(data->midi2, false);
			 al_set_audio_stream_playing(data->midi3, false);
			 al_set_audio_stream_playing(data->midi4, false);
		 }
	 }

	 if ((data->mousex * 2 > 313) && (data->mousex * 2 < 388) && (data->mousey * 2 > 169) && (data->mousey * 2 < 191)) {
		if ((data->connected || data->connecting) && !data->splash && !data->ie) {
			data->connected = false;
			data->connecting = false;
			al_stop_sample_instance(data->modem);
		}
	 }



	 if ((data->mousex * 2 < 60) && (data->mousey * 2 > 333)) {
		 ChangeCurrentGamestate(game, "off");

}
//	 data->ie=true;
 }
 if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_S)) {
StartGamestate(game, "screensaver");
  }
 if (ev->type==ALLEGRO_EVENT_KEY_DOWN) {
	 data->screensaver=0;
 }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct EmptyResources *data = malloc(sizeof(struct EmptyResources));
	data->font = al_load_font(GetDataFilePath(game, "fonts/PerfectDOSVGA437.ttf"), 16, ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_NO_AUTOHINT);
	progress(game);
	data->tahoma = al_load_ttf_font(GetDataFilePath(game, "fonts/micross.ttf"), 11, ALLEGRO_TTF_MONOCHROME);

	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->threef = al_load_bitmap(GetDataFilePath(game, "mpv-shot0003.png"));
	data->bitmap = al_create_bitmap(480, 360);
	data->room1 = al_load_bitmap(GetDataFilePath(game, "room1.png"));
	data->room2 = al_load_bitmap(GetDataFilePath(game, "room2.png"));
	data->iebitmap = al_load_bitmap(GetDataFilePath(game, "ie.png"));
	data->iebitmap2 = al_load_bitmap(GetDataFilePath(game, "ie2.png"));
	data->iesplash = al_load_bitmap(GetDataFilePath(game, "iesplash.png"));
	progress(game);
	data->shader = al_create_shader(ALLEGRO_SHADER_GLSL);
	PrintConsole(game, "VERTEX: %d", al_attach_shader_source_file(data->shader, ALLEGRO_VERTEX_SHADER, "data/ex_shader_vertex.glsl"));
	PrintConsole(game, "%s", al_get_shader_log(data->shader));
	PrintConsole(game, "PIXEL: %d", al_attach_shader_source_file(data->shader, ALLEGRO_PIXEL_SHADER, "data/ex_shader_pixel.glsl"));
PrintConsole(game, "%s", al_get_shader_log(data->shader));
  al_build_shader(data->shader);
	progress(game);
	data->screen = al_load_bitmap(GetDataFilePath(game, "screen.png"));

	data->floppy = al_load_bitmap(GetDataFilePath(game, "floppy.png"));
	data->floppytaken = al_load_bitmap(GetDataFilePath(game, "floppytaken.png"));
	data->floppyinuse = al_load_bitmap(GetDataFilePath(game, "floppyinuse.png"));
	progress(game);
	data->conbmp = al_load_bitmap(GetDataFilePath(game, "connecting.png"));
	data->donebmp = al_load_bitmap(GetDataFilePath(game, "connected.png"));
	progress(game);
data->gif = al_create_bitmap(461, 194);

  data->cursor = al_load_bitmap(GetDataFilePath(game, "cursor.png"));
	data->busycur = al_load_bitmap(GetDataFilePath(game, "busy.png"));
	progress(game);
	data->timeline = TM_Init(game, "gaem");
	data->lines = TM_Init(game, "lines");


	progress(game);
	data->mom = CreateCharacter(game, "mom");

	RegisterSpritesheet(game, data->mom, "sit");
	RegisterSpritesheet(game, data->mom, "left");
	RegisterSpritesheet(game, data->mom, "wait");
	RegisterSpritesheet(game, data->mom, "try");
	RegisterSpritesheet(game, data->mom, "talk");
	LoadSpritesheets(game, data->mom);
	progress(game);
	data->baby = CreateCharacter(game, "baby");
	for (int i=0; i < sizeof(anims) / sizeof(anims[0]); i++) {
		RegisterSpritesheet(game, data->baby, anims[i]);
	}
	LoadSpritesheets(game, data->baby);
	progress(game);
	data->startup_sample = al_load_sample(GetDataFilePath(game, "startup.flac"));

	data->startup = al_create_sample_instance(data->startup_sample);
	al_attach_sample_instance_to_mixer(data->startup, game->audio.fx);
	progress(game);
	data->ring_sample = al_load_sample(GetDataFilePath(game, "ring.ogg"));

	data->ring = al_create_sample_instance(data->ring_sample);
	al_attach_sample_instance_to_mixer(data->ring, game->audio.fx);
	progress(game);
	data->modem_sample = al_load_sample(GetDataFilePath(game, "modem.ogg"));

	data->modem = al_create_sample_instance(data->modem_sample);
	al_attach_sample_instance_to_mixer(data->modem, game->audio.fx);

	progress(game);
	data->midi1 = al_load_audio_stream(GetDataFilePath(game, "midi1.ogg"), 4, 1024);
	al_set_audio_stream_playing(data->midi1, false);
	al_attach_audio_stream_to_mixer(data->midi1, game->audio.music);
	progress(game);
	data->midi2 = al_load_audio_stream(GetDataFilePath(game, "midi2.ogg"), 4, 1024);
	al_set_audio_stream_playing(data->midi2, false);
	al_attach_audio_stream_to_mixer(data->midi2, game->audio.music);
	progress(game);
	data->midi3 = al_load_audio_stream(GetDataFilePath(game, "midi3.ogg"), 4, 1024);
	al_set_audio_stream_playing(data->midi3, false);
	al_attach_audio_stream_to_mixer(data->midi3, game->audio.music);
	progress(game);
	data->midi4 = al_load_audio_stream(GetDataFilePath(game, "midi4.ogg"), 4, 1024);
	al_set_audio_stream_playing(data->midi4, false);
	al_attach_audio_stream_to_mixer(data->midi4, game->audio.music);

	al_set_audio_stream_playmode(data->midi1, ALLEGRO_PLAYMODE_LOOP);
	al_set_audio_stream_playmode(data->midi2, ALLEGRO_PLAYMODE_LOOP);
	al_set_audio_stream_playmode(data->midi3, ALLEGRO_PLAYMODE_LOOP);
	al_set_audio_stream_playmode(data->midi4, ALLEGRO_PLAYMODE_LOOP);

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
data->debug=0;
  al_stop_sample_instance(game->data->data3);

	data->blink_counter = 0;
	data->screensaver = 0;
	data->watched = 0;

	data->mousex = 240/2;
	data->mousey = 180/2;

	data->chosen = 1;
	data->lost = false;
	data->ie=false;
	data->splash=false;
	data->busy=false;
	data->connected = false;
	data->connecting = false;
	data->incall = false;
	data->ieconnected = false;

	data->noice = 1;
	SelectSpritesheet(game, data->mom, "sit");
	SetCharacterPosition(game, data->mom, 0, 0, 0);

	SelectSpritesheet(game, data->baby, "baby");
	SetCharacterPosition(game, data->baby, 1, 1, 0);

	if (game->data->data != (void*)2) {
		al_set_sample_instance_gain(data->startup, 1.2);
		al_play_sample_instance(data->startup);
	}

	if (GetConfigOptionDefault(game, "KARCZOCH", "homealone", "0")[0] == '1') {
		//home alone
		SelectSpritesheet(game, data->mom, "left");
	} else {
		TM_AddBackgroundAction(data->timeline, Ring, TM_AddToArgs(NULL, 1, data), 12000, "ring");
	}

	//TM_AddAction(data->timeline, Sit, TM_AddToArgs(NULL, 1, data), "sit");

}

void Gamestate_Stop(struct Game *game, struct EmptyResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
	al_stop_sample_instance(data->ring);
	al_stop_sample_instance(data->modem);
	al_stop_sample_instance(data->startup);

	al_set_audio_stream_playing(data->midi1, false);
	al_set_audio_stream_playing(data->midi2, false);
	al_set_audio_stream_playing(data->midi3, false);
	al_set_audio_stream_playing(data->midi4, false);

	TM_CleanBackgroundQueue(data->timeline);
	TM_CleanQueue(data->timeline);
}

// Ignore those for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Pause(struct Game *game, struct EmptyResources* data) {}
void Gamestate_Resume(struct Game *game, struct EmptyResources* data) {}
