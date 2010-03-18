/*
Copyright (C) 2009-2010 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "../headers.h"

#include "../graphics/animation.h"
#include "../audio/audio.h"
#include "../system/properties.h"
#include "../entity.h"
#include "key_items.h"
#include "item.h"
#include "../hud.h"
#include "../inventory.h"
#include "../event/script.h"
#include "../collisions.h"
#include "../system/error.h"

extern Entity *self;

static void touch(Entity *);
static void activate(int);
static void wait(void);
static void init(void);

Entity *addFinalPowerGenerator(int x, int y, char *name)
{
	Entity *e = getFreeEntity();

	if (e == NULL)
	{
		showErrorAndExit("No free slots to add a Final Power Generator");
	}

	loadProperties(name, e);

	e->x = x;
	e->y = y;

	e->type = KEY_ITEM;

	e->face = RIGHT;

	e->action = &init;
	e->activate = &activate;
	e->touch = &touch;

	e->draw = &drawLoopingAnimationToMap;

	setEntityAnimation(e, STAND);

	e->thinkTime = 0;

	return e;
}

static void wait()
{
	if (self->mental < 0)
	{
		if (self->active == TRUE)
		{
			self->mental *= -1;

			playSoundToMap("sound/item/generator.ogg", -1, self->x, self->y, 0);

			setEntityAnimation(self, self->mental);

			self->frameSpeed = 1;
		}
	}

	checkToMap(self);
}

static void touch(Entity *other)
{
	if (other->type == PLAYER && self->active == FALSE)
	{
		setInfoBoxMessage(0, _("Press Action to interact"));
	}
}

static void activate(int val)
{
	if (self->active == FALSE)
	{
		runScript("final_generator");
	}
}

static void init()
{
	if (self->active == TRUE)
	{
		setEntityAnimation(self, self->mental);
	}

	else if (self->active == FALSE && self->mental < 0)
	{
		setEntityAnimation(self, -self->mental);

		self->frameSpeed = 0;
	}

	self->action = &wait;
}