/*! \file dosowisko.c
 *  \brief Init animation with dosowisko.net logo.
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

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <libsuperderpy.h>
#include "dosowisko.h"

int Gamestate_ProgressCount = 5;

static const char* text = "# dosowisko.net";

//==================================Timeline manager actions BEGIN
bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct dosowiskoResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->fade=0;
	}
	else if (state == TM_ACTIONSTATE_DESTROY) {
		data->fade=255;
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		data->fade+=2;
		data->tan++;
		return data->fade >= 255;
	}
	return false;
}

bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct dosowiskoResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		data->fadeout = true;
	}
	return true;
}

bool End(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct dosowiskoResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_RUNNING)  { data->color=true; data->noice=1; }
	return true;
}

bool RealEnd(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_RUNNING) SwitchGamestate(game, "dosowisko", "gaem");
	return true;
}

bool Play(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	ALLEGRO_SAMPLE_INSTANCE *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_RUNNING) al_play_sample_instance(data);
	return true;
}

bool Type(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct dosowiskoResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_RUNNING) {
		strncpy(data->text, text, data->pos++);
		data->text[data->pos] = 0;
		if (strcmp(data->text, text) != 0) {
			TM_AddBackgroundAction(data->timeline, Type, TM_AddToArgs(NULL, 1, data), 60 + rand() % 60, "type");
		} else{
			al_stop_sample_instance(data->kbd);
		}
		return true;
	}
	return false;
}
//==================================Timeline manager actions END


void Gamestate_Logic(struct Game *game, struct dosowiskoResources* data) {
	TM_Process(data->timeline);
	data->tick++;
	if (data->tick == 30) {
		data->underscore = !data->underscore;
		data->tick = 0;
	}

	data->blink_counter++;
	if (data->blink_counter >= 40) {
		data->blink_counter = 0;
		data->chosen++;
	}

	if (data->color && data->noice) {
		data->noice++;
	 }

}

void Gamestate_Draw(struct Game *game, struct dosowiskoResources* data) {

	al_set_target_bitmap(data->bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_set_target_bitmap(data->pixelator);
	if (data->color) {
		al_clear_to_color(al_map_rgb(0, 128, 128));
	} else {
	al_clear_to_color(al_map_rgb(35, 31, 32));
	}
	al_set_target_bitmap(data->bitmap2);
al_clear_to_color(al_map_rgb(35, 31, 32));
  if (!data->fadeout) {

		char t[255] = "";
		strcpy(t, data->text);
		if (data->underscore) {
			strncat(t, "_", 1);
		} else {
			strncat(t, " ", 1);
		}

		al_set_target_bitmap(data->bitmap);

		al_draw_text(data->font, al_map_rgba(255,255,255,10), game->viewport.width/2 - (320-240)/2,
		             game->viewport.height*0.4167, ALLEGRO_ALIGN_CENTRE, t);

		double tg = tan(-data->tan/384.0 * ALLEGRO_PI - ALLEGRO_PI/2);

		int fade = data->fadeout ? 255 : data->fade;

		al_set_target_bitmap(data->pixelator);
//PrintConsole(game, "%d %f", fade, tg);
		al_draw_tinted_scaled_bitmap(data->bitmap, al_map_rgba(fade, fade, fade, fade), 0, 0,
		                             al_get_bitmap_width(data->bitmap), al_get_bitmap_height(data->bitmap),
		                             -tg*al_get_bitmap_width(data->bitmap)*0.05,
		                             -tg*al_get_bitmap_height(data->bitmap)*0.05,
		                             al_get_bitmap_width(data->bitmap)+tg*0.1*al_get_bitmap_width(data->bitmap),
		                             al_get_bitmap_height(data->bitmap)+tg*0.1*al_get_bitmap_height(data->bitmap),
		                             0);

		al_draw_bitmap(data->checkerboard, 0, 0, 0);


		//al_use_shader(data->shader);
		//al_draw_bitmap(data->pixelator, 0, 0, 0);
		//al_use_shader(NULL);


}

	al_set_target_backbuffer(game->display);


	  al_use_shader(data->shader);
		al_set_shader_int("scaleFactor", 1);
	//	al_draw_bitmap(data->bitmap, 0, 0, 0);
		al_draw_bitmap(data->pixelator, 0, 0, 0);
//		al_draw_scaled_bitmap(data->pixelator, 0, 0, 320, 360, 0, 0, 240, 180, 0);
		al_use_shader(NULL);

		al_set_target_bitmap(data->pixelator);
		al_clear_to_color(al_map_rgb(35, 31, 32));
		al_set_target_backbuffer(game->display);

		if (data->noice > 70) {
		al_set_target_bitmap(data->bitmap2);
		WhiteNoise(game);
		al_set_target_backbuffer(game->display);
		}

		al_use_shader(data->shader);
		al_set_shader_int("scaleFactor", 1);
		al_draw_scaled_bitmap(data->bitmap2, 0, 0, 240, 180, 240, 0, 80, 60, 0);
		al_draw_scaled_bitmap(data->bitmap2, 0, 0, 240, 180, 240, 60, 80, 60, 0);
		//al_draw_scaled_bitmap(data->bitmap, 0, 0, 240, 180, 240, 120, 80, 60, 0);
		al_use_shader(NULL);

		al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 0, 0, 240, 180, 0);
		al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 0, 80, 60, 0);
		al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 60, 80, 60, 0);

		al_draw_scaled_bitmap(data->floppy, 0, 0, 240, 180, 240, 120, 80, 60, 0);


		//al_draw_scaled_bitmap(data->screen, 0, 0, al_get_bitmap_width(data->screen), al_get_bitmap_height(data->screen), 240, 120, 80, 60, 0);
		//al_use_transform(&game->projection);


}

void Gamestate_Start(struct Game *game, struct dosowiskoResources* data) {
	data->pos = 1;
	data->fade = 0;
	data->tan = 64;
	data->tick = 0;
	data->noice=0;
	data->fadeout = false;
	data->underscore=true;
	data->color=false;
	strcpy(data->text, "#");
	TM_AddDelay(data->timeline, 300);
	TM_AddQueuedBackgroundAction(data->timeline, FadeIn, TM_AddToArgs(NULL, 1, data), 0, "fadein");
	TM_AddDelay(data->timeline, 1500);
	TM_AddAction(data->timeline, Play, TM_AddToArgs(NULL, 1, data->kbd), "playkbd");
	TM_AddQueuedBackgroundAction(data->timeline, Type, TM_AddToArgs(NULL, 1, data), 0, "type");
	TM_AddDelay(data->timeline, 3200);
	TM_AddAction(data->timeline, Play, TM_AddToArgs(NULL, 1, data->key), "playkey");
	TM_AddDelay(data->timeline, 50);
	TM_AddAction(data->timeline, FadeOut, TM_AddToArgs(NULL, 1, data), "fadeout");
	TM_AddDelay(data->timeline, 1000);
	TM_AddAction(data->timeline, End,  TM_AddToArgs(NULL, 1, data), "end");
	TM_AddDelay(data->timeline, 2000);
	TM_AddAction(data->timeline, RealEnd, NULL, "realend");
	al_play_sample_instance(data->sound);
}

void Gamestate_ProcessEvent(struct Game *game, struct dosowiskoResources* data, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(data->timeline, ev);
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		SwitchGamestate(game, "dosowisko", "gaem");
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	struct dosowiskoResources *data = malloc(sizeof(struct dosowiskoResources));
	data->timeline = TM_Init(game, "main");
	data->bitmap = al_create_bitmap(240, game->viewport.height);
	data->checkerboard = al_create_bitmap(240, game->viewport.height);
	data->pixelator = al_create_bitmap(240, game->viewport.height);

	data->bitmap2 = al_create_bitmap(480, 360);

	al_set_target_bitmap(data->checkerboard);
	al_lock_bitmap(data->checkerboard, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
	int x, y;
	for (x = 0; x < al_get_bitmap_width(data->checkerboard); x=x+2) {
		for (y = 0; y < al_get_bitmap_height(data->checkerboard); y=y+2) {
			al_put_pixel(x, y, al_map_rgba(0,0,0,64));
			al_put_pixel(x+1, y, al_map_rgba(0,0,0,0));
			al_put_pixel(x, y+1, al_map_rgba(0,0,0,0));
			al_put_pixel(x+1, y+1, al_map_rgba(0,0,0,0));
		}
	}
	al_unlock_bitmap(data->checkerboard);
	al_set_target_backbuffer(game->display);
	(*progress)(game);

	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/DejaVuSansMono.ttf"),
	                              (int)(game->viewport.height*0.15 / 8) * 8 ,0 );
	(*progress)(game);
	data->sample = al_load_sample( GetDataFilePath(game, "dosowisko.flac") );
	data->sound = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->sound, game->audio.music);
	al_set_sample_instance_playmode(data->sound, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	data->kbd_sample = al_load_sample( GetDataFilePath(game, "kbd.flac") );
	data->kbd = al_create_sample_instance(data->kbd_sample);
	al_attach_sample_instance_to_mixer(data->kbd, game->audio.fx);
	al_set_sample_instance_playmode(data->kbd, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	data->key_sample = al_load_sample( GetDataFilePath(game, "key.flac") );
	data->key = al_create_sample_instance(data->key_sample);
	al_attach_sample_instance_to_mixer(data->key, game->audio.fx);
	al_set_sample_instance_playmode(data->key, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	//data->threef = al_load_bitmap(GetDataFilePath(game, "mpv-shot0003.png"));
	//data->bitmap = al_create_bitmap(480, 360);

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

void Gamestate_Stop(struct Game *game, struct dosowiskoResources* data) {
	al_stop_sample_instance(data->sound);
	al_stop_sample_instance(data->kbd);
	al_stop_sample_instance(data->key);
}

void Gamestate_Unload(struct Game *game, struct dosowiskoResources* data) {
	al_destroy_font(data->font);
	al_destroy_sample_instance(data->sound);
	al_destroy_sample(data->sample);
	al_destroy_sample_instance(data->kbd);
	al_destroy_sample(data->kbd_sample);
	al_destroy_sample_instance(data->key);
	al_destroy_sample(data->key_sample);
	al_destroy_bitmap(data->bitmap);
	al_destroy_bitmap(data->checkerboard);
	al_destroy_bitmap(data->pixelator);
	TM_Destroy(data->timeline);
	free(data);
}

void Gamestate_Reload(struct Game *game, struct dosowiskoResources* data) {}

void Gamestate_Pause(struct Game *game, struct dosowiskoResources* data) {
	TM_Pause(data->timeline);
}
void Gamestate_Resume(struct Game *game, struct dosowiskoResources* data) {
	TM_Resume(data->timeline);
}