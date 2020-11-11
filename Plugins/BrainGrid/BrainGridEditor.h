/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

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

//#ifndef __BRAINGRIDEDITOR_H__
//#define __BRAINGRIDEDITOR_H__

#include <EditorHeaders.h>

namespace BrainGrid
{
    class BrainGridThread;
//public Label::Listener, public ComboBox::Listener, 
    
    class BrainGridEditor: public GenericEditor//, public AsyncUpdater,
    {
    public:
        BrainGridEditor(GenericProcessor* parentNode, BrainGridThread* board_, bool useDefaultParameterEditors);
        ~BrainGridEditor(){}

        void buttonEvent(Button* button);
        void sliderEvent(Slider* slider);

        //void combBoxChanged(ComboBox* comboBox);

        //void handleAsyncUpdate();

    private:
    //		OwnedArray<HeadstageOptionsInterface> headstageOptionsInterfaces;
    //        OwnedArray<ElectrodeButton> electrodeButtons;

    //		ScopedPointer<SampleRateInterface> sampleRateInterface;
    //		ScopedPointer<BandwidthInterface> bandwidthInterface;
    //		ScopedPointer<DSPInterface> dspInterface;

    //		ScopedPointer<AudioInterface> audioInterface;
    //		ScopedPointer<ClockDivideInterface> clockInterface;

            ScopedPointer<UtilityButton> rescanButton, dacTTLButton;
            ScopedPointer<UtilityButton> auxButton;
            ScopedPointer<UtilityButton> adcButton;
            ScopedPointer<UtilityButton> ledButton;

            ScopedPointer<UtilityButton> resetButton;
            ScopedPointer<UtilityButton> startButton;

            ScopedPointer<ThresholdSlider> sampleRateSlider;

            ScopedPointer<UtilityButton> dspoffsetButton;
            //ScopedPointer<ComboBox> ttlSettleCombo, dacHPFcombo;

            ScopedPointer<Label> audioLabel, ttlSettleLabel, dacHPFlabel;

            BrainGridThread* board;

    };
}
//#endif  // __BRAINGRIDEDITOR_H__
