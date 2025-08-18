/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 18.08.2025

  Copyright (C) 2025, Johannes Natter

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SIMON_SUPERVISING_H
#define SIMON_SUPERVISING_H

#include "Processing.h"
#include "MsgDispatching.h"

class SimonSupervising : public Processing
{

public:

	static SimonSupervising *create()
	{
		return new dNoThrow SimonSupervising;
	}

protected:

	virtual ~SimonSupervising() {}

private:

	SimonSupervising();
	SimonSupervising(const SimonSupervising &) = delete;
	SimonSupervising &operator=(const SimonSupervising &) = delete;

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success process();
	Success shutdown();
	void processInfo(char *pBuf, char *pBufEnd);

	bool servicesStart();

	/* member variables */
	//uint32_t mStartMs;
	uint32_t mStateSd;
	MsgDispatching *mpApp;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

