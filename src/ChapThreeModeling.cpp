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

ChapThreeModeling::ChapThreeModeling()
	: Processing("ChapThreeModeling")
	//, mStartMs(0)
	, mNumCitizen(1000)
	, mNumSelectorate(50)
	, mNumWinning(10)
	, mRng(random_device{}())
	, mSelectors()
	, mpLeader(NULL)
	, mpChallenger(NULL)
	, mWl()
	, mWc()
{
	mState = StStart;
}

/* member functions */

void ChapThreeModeling::seedSet(uint32_t seed)
{
	mRng.seed(seed);
}

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

		if (!mNumCitizen || !mNumSelectorate || !mNumWinning)
			return procErrLog(-1, "invalid arguments");

		procInfLog("Size of winning coalition  (W)      %u", mNumWinning);
		procInfLog("Size of selectorate        (S)      %u", mNumSelectorate);
		procInfLog("Number of citizens         (N)      %u", mNumCitizen);

		selectorsCreate();

		challengerSet();
		selectorsPick(&mWl);
		selectorsPick(&mWc);
		proposalCreate(&mPropLeader);
		proposalCreate(&mPropChallenger);

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

	Selector sel;

	mSelectors.clear();
	mSelectors.reserve(mNumSelectorate);

	for (uint16_t i = 0; i < mNumSelectorate; ++i)
	{
		sel.id = i + 1;
		sel.chosenByLeader = false;
		sel.chosenByChallenger = false;

		mSelectors.push_back(sel);
	}

	mpLeader = randomSelGet();
}

void ChapThreeModeling::challengerSet()
{
	while (mpChallenger = randomSelGet(), mpChallenger == mpLeader);
}

void ChapThreeModeling::selectorsPick(list<Selector *> *pWinningCoalition)
{
	if (!pWinningCoalition)
	{
		procWrnLog("could not create winning coalition");
		return;
	}

	bool isLeader = pWinningCoalition == &mWl;

	procInfLog("%s (%u) picks selectors for winning coalition %s",
				isLeader ? "Leader" : "Challenger",
				isLeader ? mpLeader->id : mpChallenger->id,
				isLeader ? "(Wl)" : "(Wc)");

	Selector *pSel;
	bool *pChosen;

	pWinningCoalition->clear();

	while (pWinningCoalition->size() < mNumWinning)
	{
		pSel = randomSelGet();

		pChosen = isLeader ? &pSel->chosenByLeader : &pSel->chosenByChallenger;

		if (*pChosen)
		{
			//procWrnLog("chosen already");
			continue;
		}

		if (pSel == mpLeader || pSel == mpChallenger)
			continue;

		*pChosen = true;
		pWinningCoalition->push_back(pSel);
		//procInfLog("%2u.  %3u", pWinningCoalition->size(), pSel->id);
	}
}

void ChapThreeModeling::proposalCreate(Proposal *pProp)
{
	if (!pProp)
	{
		procWrnLog("could not create proposal");
		return;
	}

	bool isLeader = pProp == &mPropLeader;

	procInfLog("Creating %s's proposal",
				isLeader ? "leader" : "challenger");

	pProp->rateTax_r = randomDouble();
	pProp->goodsPrivate_g = randomDouble();
	pProp->goodsPublic_x = randomDouble();

	procInfLog("Tax rate          %.3f", pProp->rateTax_r);
	procInfLog("Private goods     %.3f", pProp->goodsPrivate_g);
	procInfLog("Public goods      %.3f", pProp->goodsPublic_x);
}

ChapThreeModeling::Selector *ChapThreeModeling::randomSelGet()
{
	return &mSelectors[randomInt(mNumSelectorate - 1)];
}

uint32_t ChapThreeModeling::randomInt(uint32_t nMax)
{
	uniform_int_distribution<uint32_t> dist(0, nMax);
	return dist(mRng);
}

double ChapThreeModeling::randomDouble()
{
	uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(mRng);
}

void ChapThreeModeling::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */

