/*
Copyright (C) 2009 Parallel Realities

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
#include "../inventory.h"
#include "../item/bomb.h"

extern Entity *self, player;

static void collectBomb(Entity *);

Entity *addBombPile(int x, int y, char *name)
{
	Entity *e = getFreeEntity();

	if (e == NULL)
	{
		printf("No free slots to add Bomb Pile\n");

		exit(1);
	}

	loadProperties(name, e);

	e->x = x;
	e->y = y;

	e->type = KEY_ITEM;

	e->face = RIGHT;

	e->action = &doNothing;
	e->touch = &collectBomb;

	e->draw = &drawLoopingAnimationToMap;

	setEntityAnimation(e, STAND);

	e->thinkTime = 0;

	return e;
}

static void collectBomb(Entity *other)
{
	Entity *bomb = NULL;

	if (other->type == PLAYER)
	{
		if (self->thinkTime <= 0 && getInventoryItem(_("Bomb")) == NULL)
		{
			bomb = addBomb(other->x, other->y, "item/bomb");

			STRNCPY(bomb->objectiveName, _("Bomb"), sizeof(bomb->objectiveName));

			addToInventory(bomb);

			self->thinkTime = self->maxThinkTime;
		}
	}
}