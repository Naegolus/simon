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

#ifndef CHAP_THREE_MODELING_H
#define CHAP_THREE_MODELING_H

#include <vector>
#include <random>

#include "Processing.h"

class ChapThreeModeling : public Processing
{

public:

	static ChapThreeModeling *create()
	{
		return new dNoThrow ChapThreeModeling;
	}

	uint8_t mNumWinning_W;
	uint16_t mNumSelectorate_S;
	uint32_t mNumCitizen_N;

	double mCostPublic;
	double mDelta;
	uint8_t mAgeWin;

	void seedSet(uint32_t seed);

protected:

	virtual ~ChapThreeModeling() {}

private:

	ChapThreeModeling();
	ChapThreeModeling(const ChapThreeModeling &) = delete;
	ChapThreeModeling &operator=(const ChapThreeModeling &) = delete;

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	struct Selector
	{
		uint32_t id;
		bool chosenByIncumbent;
		bool chosenByChallenger;
	};

	struct Policies
	{
		double rateTax_r;
		double goodsPrivate_g;
		double goodsPublic_x;
	};

	struct Consequences
	{
		double leisure_l;
		double effort_e;
		double activityReturns_y;
		double payoffDisenfranchized;
		double activityEconomic_E;
		double revenuesGov_R;
		double costsGov_M;
	};

	struct Strategy
	{
		std::list<Selector *> coalition;
		Policies proposal;
		Consequences estimations;
	};

	/* member functions */
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	void selectorsCreate();
	void challengerSet();
	void strategyRandomCreate(Strategy *pStrategy);
	void consequencesCalc(Strategy *pStrategy);
	void newIncumbentVote();
	bool challengerAccept(Selector *pSel);
	double continuationValue(Strategy *pStrategy, Selector *pSel);
	void lawEnact();
	void resultsDevelop();

	double utility(double goodsPublic_x,
				double goodsPrivate_g,
				double economicActivityReturns_y,
				double leisure_l);

	Selector *randomSelGet();
	uint32_t randomInt(uint32_t nMax);
	double randomDouble();

	/* member variables */
	//uint32_t mStartMs;
	double mLoyaltyNorm;
	std::mt19937 mRng;
	std::vector<Selector> mSelectors;
	Selector *mpIncumbent;
	Selector *mpChallenger;
	Strategy mStrategyIncumbent;
	Strategy mStrategyChallenger;
	std::list<Selector *> *mpCoalition;
	Policies *mpLaw;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

