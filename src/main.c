/*! \file main.c
 *  \brief Main file of Super Examples.
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
#include <signal.h>
#include <libsuperderpy.h>

void derp(int sig) {
	ssize_t __attribute__((unused)) n = write(STDERR_FILENO, "Segmentation fault\nI just don't know what went wrong!\n", 54);
	abort();
}

int main(int argc, char** argv) {
	signal(SIGSEGV, derp);

	srand(time(NULL));

	al_set_org_name("dosowisko.net");
	al_set_app_name("KARCZOCH");

	struct Game *game = libsuperderpy_init(argc, argv, "karczoch");
	if (!game) { return 1; }

	al_set_window_title(game->display, "KARCZOCH");

	game->data = (void*)0;

	LoadGamestate(game, "intro");
	LoadGamestate(game, "menu");
	LoadGamestate(game, "options");
	LoadGamestate(game, "about");
	LoadGamestate(game, "gaem");
	LoadGamestate(game, "dosowisko");
	LoadGamestate(game, "bsod");
	LoadGamestate(game, "off");
	LoadGamestate(game, "screensaver");
	StartGamestate(game, "intro");

	al_hide_mouse_cursor(game->display);


	libsuperderpy_run(game);

	libsuperderpy_destroy(game);

	return 0;
}
