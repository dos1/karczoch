/*! \file empty.h
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

struct EmptyResources {
		// This struct is for every resource allocated and used by your gamestate.
		// It gets created on load and then gets passed around to all other function calls.
		ALLEGRO_FONT *font, *tahoma;
		int blink_counter;
		ALLEGRO_SHADER *shader;
		ALLEGRO_BITMAP *threef;

		ALLEGRO_BITMAP *bitmap, *iebitmap, *iebitmap2, *iesplash, *busycur, *conbmp, *donebmp;

		ALLEGRO_BITMAP *floppy, *floppytaken, *floppyinuse, *cursor, *room1, *room2;

		ALLEGRO_BITMAP *screen, *gif;

		ALLEGRO_SAMPLE *startup_sample, *ring_sample, *modem_sample, *midi1_sample, *midi2_sample, *midi3_sample, *midi4_sample;
		ALLEGRO_SAMPLE_INSTANCE *startup, *ring, *modem;
		ALLEGRO_AUDIO_STREAM *midi1, *midi2, *midi3, *midi4;

		struct Character *mom, *baby;

		int mousex, mousey;

		int chosen, noice;
int debug;
    struct Timeline *timeline, *lines;

		bool busy, ie, splash, connecting, connected, incall, lost, ieconnected, content;
		long long int screensaver, watched;
		int loading;
};
