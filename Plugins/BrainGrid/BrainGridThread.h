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
#pragma once

//#ifndef __BRAINGRIDTHREAD_H__
//#define __BRAINGRIDTHREAD_H__

#include <DataThreadHeaders.h>

#include <stdio.h>
#include <string.h>
#include <array>
#include <atomic>
#include <csignal>

#include "rhythm-api/braingridboard.h"
#include "rhythm-api/okFrontPanelDLL.h"

namespace BrainGrid
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
		bool setSampleRate(float aRate);

		float getBitVolts(const DataChannel* chan) const override;
		int getNumChannels() const;

		//int getNumInputs() const override;

		void enableBoardLeds(bool enable);
		void resetBoard(bool toReset);
		void startBoard(bool toStart);

		bool isChipCalibrated(void);

		GenericEditor* createEditor(SourceNode* sn);

		static DataThread* createDataThread(SourceNode* sn);
		
	private:
		void setDefaultChannelNames() override;

		bool updateBuffer() override;

		void timerCallback() override;

		bool startAcquisition() override;
		bool stopAcquisition()  override;

		ScopedPointer<BraingridBoard> braingridBoard;
		String libraryFilePath;

		bool openBoard();
		bool uploadBitfile(String pathToBitfile);
		void initializeBoard();
		void scanSPIPorts();
		
		float sampleRate;
		int samplePeriodCount;
		int currentBufferUpdate;
		bool deviceFound;
		unsigned int blockSize;
		bool isTransmitting;
		BrainGridThread* board;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrainGridThread);
	};

}
//#endif  // __BRAINGRIDTHREAD_H__
