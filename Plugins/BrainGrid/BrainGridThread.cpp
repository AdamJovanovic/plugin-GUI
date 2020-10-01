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

#include "BrainGridThread.h"
#include "BrainGridEditor.h"
using namespace BrainGridNode;

#if defined(_WIN32)
#define okLIB_NAME "okFrontPanel.dll"
#define okLIB_EXTENSION "*.dll"
#elif defined(__APPLE__)
#define okLIB_NAME "libokFrontPanel.dylib"
#define okLIB_EXTENSION "*.dylib"
#elif defined(__linux__)
#define okLIB_NAME "./libokFrontPanel.so"
#define okLIB_EXTENSION "*.so"
#endif

bool BrainGridThread::foundInputSource()
{
    return true;
}

int BrainGridThread::getNumDataOutputs(DataChannel::DataChannelTypes type, int subProcessor) const
{
    return DataChannel::DataChannelTypes::HEADSTAGE_CHANNEL;
}

unsigned int BrainGridThread::getNumSubProcessors() const
{
    return 1;
}

int BrainGridThread::getNumTTLOutputs(int subprocessor) const
{
    return 1;
}

bool BrainGridThread::usesCustomNames() const
{
    return true;
}

float BrainGridThread::getSampleRate(int subprocessor) const
{
    return 1.0;
}

float BrainGridThread::getBitVolts(const DataChannel* chan) const
{
    return 1.0;
}

GenericEditor* BrainGridThread::createEditor(SourceNode *sn)
{
    return new BrainGridEditor(sn, this, true);
}

BrainGridThread::BrainGridThread(SourceNode *sn): DataThread(sn)
{
    //Initialize FPGA
}

void BrainGridThread::setDefaultChannelNames()
{

}

bool BrainGridThread::updateBuffer()
{
    return true;
}

void BrainGridThread::timerCallback()
{

}

bool BrainGridThread::startAcquisition()
{
    return true;
}

bool BrainGridThread::stopAcquisition()
{
    return true;
}