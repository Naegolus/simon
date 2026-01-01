/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 01.01.2026

  Copyright (C) 2026, Johannes Natter

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

#include "ChapThreeModeling.h"

#define dForEach_ProcState(gen) \
		gen(StStart) \
		gen(StMain) \
		gen(StNop) \

#define dGenProcStateEnum(s) s,
dProcessStateEnum(ProcState);

#if 1
#define dGenProcStateString(s) #s,
dProcessStateStr(ProcState);
#endif

using namespace std;

const uint32_t ChapThreeModeling::cNumCitizen = 1000;
const uint16_t ChapThreeModeling::cNumSelectorate = 50;
const uint8_t ChapThreeModeling::cNumWinning = 10;

ChapThreeModeling::ChapThreeModeling()
	: Processing("ChapThreeModeling")
	//, mStartMs(0)
	, mSelectors()
{
	mState = StStart;
}

/* member functions */

Success ChapThreeModeling::process()
{
	//uint32_t curTimeMs = millis();
	//uint32_t diffMs = curTimeMs - mStartMs;
	//Success success;
#if 0
	dStateTrace;
#endif
	switch (mState)
	{
	case StStart:

		procInfLog("Modelling chapter three");

		procInfLog("Number of citizens         %u", cNumCitizen);
		procInfLog("Size of selectorate        %u", cNumSelectorate);
		procInfLog("Size of winning coalition  %u", cNumWinning);

		selectorsCreate();

		mState = StMain;

		break;
	case StMain:

		break;
	case StNop:

		break;
	default:
		break;
	}

	return Pending;
}

void ChapThreeModeling::selectorsCreate()
{
	procInfLog("Creating selectors");

	Selector s;

	mSelectors.clear();
	mSelectors.reserve(cNumSelectorate);

	for (uint16_t i = 0; i < cNumSelectorate; ++i)
	{
		s.id = i;

		mSelectors.push_back(s);
	}
}

void ChapThreeModeling::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */

