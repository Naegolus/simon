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
	, mNumWinning_W(10)
	, mNumSelectorate_S(50)
	, mNumCitizen_N(1000)
	, mCostPublic(100)
	, mDelta(0.01)
	, mAgeWin(10)
	, mLoyaltyNorm(0.0)
	, mNumVotesDone(0)
	, mNumVotesMax(40)
	, mRng(random_device{}())
	, mSelectors()
	, mpIncumbent(NULL)
	, mpChallenger(NULL)
	, mStrategyIncumbent()
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

		userInfLog("Modelling chapter three");

		if (!mNumWinning_W || !mNumSelectorate_S || !mNumCitizen_N)
			return procErrLog(-1, "invalid arguments");

		userInfLog("Size of winning coalition    (W)      %6u", mNumWinning_W);
		userInfLog("Size of selectorate          (S)      %6u", mNumSelectorate_S);
		userInfLog("Number of citizens           (N)      %6u", mNumCitizen_N);
		mLoyaltyNorm = (double)mNumWinning_W / mNumSelectorate_S;
		userInfLog("Loyalty norm                 (W/S)    %10.3f", mLoyaltyNorm);

		selectorsCreate();

		challengerSet();
		strategyRandomCreate(&mStrategyIncumbent);
		strategyRandomCreate(&mStrategyChallenger);
		newIncumbentVote();
		lawEnact();
		resultsDevelop();

		mState = StMain;

		break;
	case StMain:

		challengerSet();
		strategyRandomCreate(&mStrategyChallenger);
		newIncumbentVote();
		lawEnact();
		resultsDevelop();

		if (mNumVotesDone < mNumVotesMax)
			break;

		mState = StNop;

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
	userInfLog("Creating selectors");

	Selector sel;

	mSelectors.clear();
	mSelectors.reserve(mNumSelectorate_S);

	for (uint16_t i = 0; i < mNumSelectorate_S; ++i)
	{
		sel.id = i + 1;
		sel.chosenByIncumbent = false;
		sel.chosenByChallenger = false;

		mSelectors.push_back(sel);
	}
}

void ChapThreeModeling::challengerSet()
{
	userInfLog("-------------------------------------------------- Period %u", mNumVotesDone);

	vector<Selector>::iterator iSel;

	while (mpChallenger = randomSelGet(), mpChallenger == mpIncumbent);

	iSel = mSelectors.begin();
	for (; iSel != mSelectors.end(); ++iSel)
		iSel->chosenByChallenger = false;
}

void ChapThreeModeling::strategyRandomCreate(Strategy *pStrategy)
{
	if (!pStrategy)
	{
		procWrnLog("could not create strategy");
		return;
	}

	bool forIncumbent = pStrategy == &mStrategyIncumbent;

	if (forIncumbent and !mpIncumbent)
	{
		userInfLog("No leader");
		return;
	}

	userInfLog("\033[1;36m%s\033[0m (%u) picks strategy",
				forIncumbent ? "Incumbent" : "Challenger",
				forIncumbent ? mpIncumbent->id : mpChallenger->id);

	list<Selector *> *pCoal = &pStrategy->coalition;
	Selector *pSel;
	bool *pChosen;

	pCoal->clear();

	while (pCoal->size() < mNumWinning_W)
	{
		pSel = randomSelGet();

		pChosen = forIncumbent ? &pSel->chosenByIncumbent : &pSel->chosenByChallenger;

		if (*pChosen)
		{
#if 0
			procWrnLog("Selector %u already chosen by %s",
						pSel->id,
						forIncumbent ? "incumbent" : "challenger");
#endif
			continue;
		}

		if (pSel == mpIncumbent || pSel == mpChallenger)
			continue;

		*pChosen = true;
		pCoal->push_back(pSel);
		//userInfLog("%2u.  %3u", pCoal->size(), pSel->id);
	}

	list<Selector *>::iterator iSel = pCoal->begin();
	char buf[64];
	char *pBuf = buf, *pBufEnd = buf + sizeof(buf);

	*pBuf = 0;

	for (uint8_t i = 0; i < 4; ++i, ++iSel)
		dInfo("%u,", (*iSel)->id);

	userInfLog("  Assemble coalition W = {%s...}", buf);

	userInfLog("  Creating proposal for policies");

	Policies *pPol = &pStrategy->proposal;

	pPol->rateTax_r = randomDouble();
	pPol->goodsPrivate_g = randomDouble();
	pPol->goodsPublic_x = randomDouble();

	policiesPrint(pPol);

	consequencesCalc(pStrategy);
}

void ChapThreeModeling::consequencesCalc(Strategy *pStrategy)
{
	userInfLog("  Estimated consequences");

	Policies *pPol = &pStrategy->proposal;
	Consequences *pEst = &pStrategy->estimations;

	pEst->leisure_l = 1 / (2 - pPol->rateTax_r);
	pEst->effort_e = 1 - pEst->leisure_l;
	pEst->activityReturns_y = (1 - pPol->rateTax_r) * pEst->effort_e;
	pEst->payoffDisenfranchized = utility(pPol->goodsPublic_x, 0, pEst->activityReturns_y, pEst->leisure_l);

	pEst->activityEconomic_E = mNumCitizen_N * pEst->effort_e;
	pEst->revenuesGov_R = pPol->rateTax_r * pEst->activityEconomic_E;
	pEst->costsPrivate = pPol->goodsPrivate_g * pStrategy->coalition.size();
	pEst->costsPublic = pPol->goodsPublic_x * mCostPublic;
	pEst->costsGov_M = pEst->costsPrivate + pEst->costsPublic;

	consequencesPrint(pEst);
}

void ChapThreeModeling::newIncumbentVote()
{
	vector<Selector>::iterator iSel;
	bool ok;
	uint16_t cntForIncumbent = 0;
	uint16_t cntForChallenger = 0;
	char maskPrint = 0;

	iSel = mSelectors.begin();
	for (; iSel != mSelectors.end(); ++iSel)
	{
		ok = challengerAccept(&(*iSel), maskPrint);
		if (ok)
			++cntForChallenger;
		else
			++cntForIncumbent;
	}

	++mNumVotesDone;

	if (cntForIncumbent >= mNumWinning_W or cntForChallenger < mNumWinning_W)
	{
		userInfLog("\033[1;32mIncumbent stays in office\033[0m");

		if (mNumVotesDone == mNumVotesMax)
			return;

		userInfLog("  Government policies");
		policiesPrint(&mStrategyIncumbent.proposal);

		userInfLog("  Government state");
		consequencesPrint(&mStrategyIncumbent.estimations);

		return;
	}

	userInfLog("\033[1;33mChallenger takes office\033[0m");

	iSel = mSelectors.begin();
	for (; iSel != mSelectors.end(); ++iSel)
	{
		iSel->chosenByIncumbent = iSel->chosenByChallenger;
		iSel->chosenByChallenger = false;
	}

	mStrategyIncumbent = mStrategyChallenger;

	mpIncumbent = mpChallenger;
	mpChallenger = NULL;
}

bool ChapThreeModeling::challengerAccept(Selector *pSel, char &maskPrint)
{
	if (!pSel)
		return false;

	if (!mpIncumbent)
		return true;

	Policies *pPolIncumbent = &mStrategyIncumbent.proposal;
	Policies *pPolChallenger = &mStrategyChallenger.proposal;
	Consequences *pConIncumbent = &mStrategyIncumbent.estimations;
	Consequences *pConChallenger = &mStrategyChallenger.estimations;
	bool feasableIncumbent = pConIncumbent->revenuesGov_R > pConIncumbent->costsGov_M;
	bool feasableChallenger = pConChallenger->revenuesGov_R > pConChallenger->costsGov_M;
	double payoffReservation_v0 = 0.0;
	double utilityFromIncumbent_VL = feasableIncumbent ? utility(
				pPolIncumbent->goodsPublic_x,
				pSel->chosenByIncumbent ? pPolIncumbent->goodsPrivate_g : 0,
				pConIncumbent->activityReturns_y,
				pConIncumbent->leisure_l) : payoffReservation_v0;
	double utilityFromChallenger_VC = feasableChallenger ? utility(
				pPolChallenger->goodsPublic_x,
				pPolChallenger->goodsPrivate_g,
				pConChallenger->activityReturns_y,
				pConChallenger->leisure_l) : payoffReservation_v0;
	double continuationFromIncumbent_ZL = continuationValue(&mStrategyIncumbent, pSel);
	double continuationFromChallenger_ZC = continuationValue(&mStrategyChallenger, pSel);
	double payoffFromIncumbent_UL = utilityFromIncumbent_VL +  mDelta * continuationFromIncumbent_ZL;
	double payoffFromChallenger_UC = utilityFromChallenger_VC + mDelta * continuationFromChallenger_ZC;
	char bitPrint = 1 << ((pSel->chosenByIncumbent ? 2 : 0) + (pSel->chosenByChallenger ? 1 : 0));
	bool ok = payoffFromChallenger_UC > payoffFromIncumbent_UL;

	if (maskPrint & bitPrint)
		return ok;

	userInfLog("Selector %2u, %s%s: %.3f + %.3f = %.3f | %.3f + %.3f = %.3f => %s%c\033[0m",
				pSel->id,
				utilityFromIncumbent_VL,
				mDelta * continuationFromIncumbent_ZL,
				payoffFromIncumbent_UL,
				utilityFromChallenger_VC,
				mDelta * continuationFromChallenger_ZC,
				payoffFromChallenger_UC,
				pSel->chosenByIncumbent ? "\033[1;36mL\033[0m" : "-",
				pSel->chosenByChallenger ? "C" : "-",
				pSel->chosenByIncumbent and ok ? "\033[1;33m" : "",
				ok ? 'C' : 'L');

	maskPrint |= bitPrint;

	return ok;
}

double ChapThreeModeling::continuationValue(Strategy *pStrategy, Selector *pSel)
{
	if (!pStrategy || !pSel)
	{
		procWrnLog("could not get payoff");
		return 0.0;
	}

	Policies *pPol = &pStrategy->proposal;
	Consequences *pCon= &pStrategy->estimations;

	bool forIncumbent = pStrategy == &mStrategyIncumbent;
	double util;

	if (forIncumbent)
	{
		util = utility(pPol->goodsPublic_x,
					pSel->chosenByIncumbent ? pPol->goodsPrivate_g : 0,
					pCon->activityReturns_y,
					pCon->leisure_l);

		util /= 1 - mDelta;
		return util;
	}

	util = utility(pPol->goodsPublic_x,
				pPol->goodsPrivate_g,
				pCon->activityReturns_y,
				pCon->leisure_l) * mLoyaltyNorm;

	util += utility(pPol->goodsPublic_x,
				0,
				pCon->activityReturns_y,
				pCon->leisure_l) * (1 - mLoyaltyNorm);

	util /= 1 - mDelta;
	return util;
}

void ChapThreeModeling::lawEnact()
{
	mpCoalition = &mStrategyIncumbent.coalition;
	mpLaw = &mStrategyIncumbent.proposal;
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
	return &mSelectors[randomInt(mNumSelectorate_S - 1)];
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

void ChapThreeModeling::policiesPrint(Policies *pPol)
{
	if (!pPol)
	{
		procWrnLog("could not print policies");
		return;
	}

	userInfLog("    Tax rate                 %10.3f", pPol->rateTax_r);
	userInfLog("    Private goods            %10.3f", pPol->goodsPrivate_g);
	userInfLog("    Public goods             %10.3f", pPol->goodsPublic_x);
}

void ChapThreeModeling::consequencesPrint(Consequences *pCon)
{
	if (!pCon)
	{
		procWrnLog("could not print consequenzes");
		return;
	}

	userInfLog("    Effort                   %10.3f", pCon->effort_e);
	userInfLog("    Economic activity        %10.3f", pCon->activityEconomic_E);
	userInfLog("    Government \033[1;32mrevenues      %10.3f\033[0m", pCon->revenuesGov_R);
	userInfLog("    Government costs         %10.3f", pCon->costsGov_M);
	userInfLog("      Private                %10.3f", pCon->costsPrivate);
	userInfLog("      Public                 %10.3f", pCon->costsPublic);
}

void ChapThreeModeling::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */

