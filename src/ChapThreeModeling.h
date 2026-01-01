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

	uint32_t mNumCitizen;
	uint16_t mNumSelectorate;
	uint8_t mNumWinning;

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
		bool chosenByLeader;
		bool chosenByChallenger;
	};

	struct Proposal
	{
		double rateTax_r;
		double goodsPublic_x;
		double goodsPrivate_g;
	};

	/* member functions */
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	void selectorsCreate();
	void challengerSet();
	void selectorsPick(std::list<Selector *> *winningCoalition);
	void proposalsCreate();

	Selector *randomSelGet();
	uint32_t randomInt(uint32_t nMax);
	double randomDouble();

	/* member variables */
	//uint32_t mStartMs;
	std::mt19937 mRng;
	std::vector<Selector> mSelectors;
	Selector *mpLeader;
	Selector *mpChallenger;
	std::list<Selector *> mWl;
	std::list<Selector *> mWc;
	Proposal mPropLeader;
	Proposal mPropChallenger;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

