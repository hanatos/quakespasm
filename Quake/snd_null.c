/*
 * snd_null.c - dummy without any functionality
 *
 * Copyright (C) 2022 johannes hanika
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "quakedef.h"


qboolean SNDDMA_Init (dma_t *dma)
{
	memset ((void *) dma, 0, sizeof(dma_t));
	shm = dma;
	/* Fill the audio DMA information block */
	shm->samplebits = 16;
	shm->signed8 = 0;
	shm->speed = 44100;
	shm->channels = 2;
	int tmp = (1024 * 2) * 10;
	if (tmp & (tmp - 1))
	{	/* make it a power of two */
		int val = 1;
		while (val < tmp)
			val <<= 1;

		tmp = val;
	}
	shm->samples = tmp;
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	size_t buffersize = shm->samples * (shm->samplebits / 8);
	shm->buffer = (unsigned char *) calloc (1, buffersize);
  return true;
}

int SNDDMA_GetDMAPos (void)
{
  return shm->samplepos;
}

void SNDDMA_Shutdown (void)
{
  if(shm)
  {
		if (shm->buffer)
			free (shm->buffer);
		shm->buffer = NULL;
		shm = NULL;
  }
}
void SNDDMA_LockBuffer (void) { }
void SNDDMA_Submit (void) { }
void SNDDMA_BlockSound (void) { }
void SNDDMA_UnblockSound (void) { }

