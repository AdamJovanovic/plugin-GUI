/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2012 Open Ephys

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

#ifndef __SIGNALGENERATOR_H_EAA44B0B__
#define __SIGNALGENERATOR_H_EAA44B0B__


#include "../../JuceLibraryCode/JuceHeader.h"
#include "GenericProcessor.h"
#include "Editors/SignalGeneratorEditor.h"

/**

  Outputs 16 channels of sine wave data.

  @see GenericProcessor, SignalGeneratorEditor

*/

class SignalGenerator : public GenericProcessor

{
public:
	
	SignalGenerator();
	~SignalGenerator();
	
	void process(AudioSampleBuffer &buffer, MidiBuffer &midiMessages, int& nSamples);

	void setParameter (int parameterIndex, float newValue);

	void setConfiguration(Configuration* cf);

	float getSampleRate() {return 44100.0;}

	AudioProcessorEditor* createEditor();
	bool hasEditor() const {return true;}

	bool enable();
	bool disable();

	bool isSource() {return true;}

private:
	
	double frequency, sampleRate;
	double currentPhase, phasePerSample;
	float amplitude;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SignalGenerator);

};





#endif  // __SIGNALGENERATOR_H_EAA44B0B__