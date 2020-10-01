/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __BrainGridThread_H_2C4CBD67__
#define __BrainGridThread_H_2C4CBD67__

#include <DataThreadHeaders.h>

#include <stdio.h>
#include <string.h>
#include <array>
#include <atomic>

#include "rhythm-api/rhd2000evalboard.h"
#include "rhythm-api/rhd2000registers.h"
#include "rhythm-api/rhd2000datablock.h"
#include "rhythm-api/okFrontPanelDLL.h"

namespace BrainGridNode
{

	class BrainGridThread : public DataThread, public Timer
	{
	public:
		BrainGridThread(SourceNode *sn);
		~BrainGridThread();

		bool foundInputSource() override;

		int getNumDataOutputs(DataChannel::DataChannelTypes type, int subProcessor) const override;

		unsigned int getNumSubProcessors() const override;

		int getNumTTLOutputs(int subprocessor) const override;

		bool usesCustomNames() const override;

		float getSampleRate(int subprocessor) const override;
		float getBitVolts(const DataChannel* chan) const override;

		GenericEditor* createEditor(SourceNode* sn);
		
	private:
		void setDefaultChannelNames() override;

		bool updateBuffer() override;

		void timerCallback() override;

		bool startAcquisition() override;
		bool stopAcquisition()  override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrainGridThread);
	};

}
#endif  // __BrainGridThread_H_2C4CBD67__