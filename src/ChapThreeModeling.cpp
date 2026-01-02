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

#include <cmath>

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
	, mCostPublic(0.2)
	, mDelta(0.01)
	, mAgeWin(10)
	, mRng(random_device{}())
	, mSelectors()
	, mpLeader(NULL)
	, mpChallenger(NULL)
	, mStrategyLeader()
	, mStrategyChallenger()
	, mpCoalition(NULL)
	, mpLaw(NULL)
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

		procInfLog("Size of winning coalition    (W)      %6u", mNumWinning);
		procInfLog("Size of selectorate          (S)      %6u", mNumSelectorate);
		procInfLog("Number of citizens           (N)      %6u", mNumCitizen);
		procInfLog("Loyalty norm                 (W/S)    %10.3f", (double)mNumWinning / mNumSelectorate);

		selectorsCreate();

		challengerSet();
		strategyRandomCreate(&mStrategyLeader);
		strategyRandomCreate(&mStrategyChallenger);
		newLeaderVote();
		lawEnact();
		resultsDevelop();

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
}

void ChapThreeModeling::challengerSet()
{
	while (mpChallenger = randomSelGet(), mpChallenger == mpLeader);
}

void ChapThreeModeling::strategyRandomCreate(Strategy *pStrategy)
{
	if (!pStrategy)
	{
		procWrnLog("could not create strategy");
		return;
	}

	bool isLeader = pStrategy == &mStrategyLeader;

	if (isLeader and !mpLeader)
	{
		procInfLog("No leader");
		return;
	}

	procInfLog("\033[1;36m%s\033[0m (%u) picks strategy",
				isLeader ? "Leader" : "Challenger",
				isLeader ? mpLeader->id : mpChallenger->id);

	list<Selector *> *pCoal = &pStrategy->coalition;
	Selector *pSel;
	bool *pChosen;

	pCoal->clear();

	while (pCoal->size() < mNumWinning)
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
		pCoal->push_back(pSel);
		//procInfLog("%2u.  %3u", pCoal->size(), pSel->id);
	}

	list<Selector *>::iterator iSel = pCoal->begin();
	char buf[64];
	char *pBuf = buf, *pBufEnd = buf + sizeof(buf);

	*pBuf = 0;

	for (uint8_t i = 0; i < 4; ++i, ++iSel)
		dInfo("%u,", (*iSel)->id);

	procInfLog("  Assemble coalition W = {%s...}", buf);

	procInfLog("  Creating proposal for policies");

	Policies *pProp = &pStrategy->proposal;

	pProp->rateTax_r = randomDouble();
	pProp->goodsPrivate_g = randomDouble();
	pProp->goodsPublic_x = randomDouble();

	procInfLog("    Tax rate                 %10.3f", pProp->rateTax_r);
	procInfLog("    Private goods            %10.3f", pProp->goodsPrivate_g);
	procInfLog("    Public goods             %10.3f", pProp->goodsPublic_x);

	consequencesCalc(pStrategy);
}

void ChapThreeModeling::consequencesCalc(Strategy *pStrategy)
{
	procInfLog("  Estimated consequences");

	Policies *pProp = &pStrategy->proposal;
	Consequences *pEst = &pStrategy->estimations;

	pEst->leisure_l = 1 / (2 - pProp->rateTax_r);
	pEst->effort_e = 1 - pEst->leisure_l;
	pEst->activityReturns_y = (1 - pProp->rateTax_r) * pEst->effort_e;
	pEst->payoffDisenfranchized = utility(pProp->goodsPublic_x, 0, pEst->activityReturns_y, pEst->leisure_l);

	pEst->activityEconomic_E = mNumCitizen * pEst->effort_e;
	pEst->revenuesGov_R = pProp->rateTax_r * pEst->activityEconomic_E;
	pEst->costsGov_M = pProp->goodsPublic_x * mCostPublic +
					pProp->goodsPrivate_g * pStrategy->coalition.size();

	procInfLog("    Effort                   %10.3f", pEst->effort_e);
	procInfLog("    Economic activity        %10.3f", pEst->activityEconomic_E);
	procInfLog("    Government \033[1;32mrevenues      %10.3f\033[0m", pEst->revenuesGov_R);
	procInfLog("    Government costs         %10.3f", pEst->costsGov_M);
}

void ChapThreeModeling::newLeaderVote()
{
	double contValLeader = continuationValue(&mStrategyLeader);
	procInfLog("Cont. value from leader      %10.3f", contValLeader);

	double contValChallenger = continuationValue(&mStrategyChallenger);
	procInfLog("Cont. value from challenger  %10.3f", contValChallenger);
}

double ChapThreeModeling::continuationValue(Strategy *pStrategy)
{
	if (!pStrategy)
	{
		procWrnLog("could not get payoff");
		return 0.0;
	}

	bool isLeader = pStrategy == &mStrategyLeader;

	Policies *pProp = &pStrategy->proposal;

	double leisure = 1 / (2 - pProp->rateTax_r);
	double effort = 1 - leisure;
	double activityReturns = (1 - pProp->rateTax_r) * effort;
	uint8_t numWinning = pStrategy->coalition.size();
	double loyaltyNorm = (double)numWinning / mNumSelectorate;

	if (isLeader)
		return utility(pProp->goodsPublic_x, pProp->goodsPrivate_g, activityReturns, leisure) / (1 - mDelta);

	return (utility(pProp->goodsPublic_x, pProp->goodsPrivate_g, activityReturns, leisure) * loyaltyNorm +
			utility(pProp->goodsPublic_x, 0, activityReturns, leisure) * (1 - loyaltyNorm)) / (1 - mDelta);
}

void ChapThreeModeling::lawEnact()
{
	mpCoalition = &mStrategyLeader.coalition;
	mpLaw = &mStrategyLeader.proposal;
}

void ChapThreeModeling::resultsDevelop()
{
	// Check law
}

double ChapThreeModeling::utility(double goodsPublic_x,
							double goodsPrivate_g,
							double economicActivityReturns_y,
							double leisure_l)
{
	return sqrt(goodsPublic_x) +
			sqrt(goodsPrivate_g) +
			sqrt(economicActivityReturns_y) +
			sqrt(leisure_l);
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

