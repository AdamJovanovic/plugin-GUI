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
    class CalibrationInterface;
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
            ScopedPointer<CalibrationInterface> calibrationInterface;
            ScopedPointer<UtilityButton> ledButton;
            ScopedPointer<UtilityButton> resetButton;
            ScopedPointer<UtilityButton> startButton;
            ScopedPointer<UtilityButton> recalibrateButton;

            ScopedPointer<ThresholdSlider> sampleRateSlider;

            ScopedPointer<Label> audioLabel, ttlSettleLabel, dacHPFlabel;

            BrainGridThread* board;

    };

    class CalibrationInterface : public Component,
		public Button::Listener
	{
	public:
		CalibrationInterface(BrainGridThread*, BrainGridEditor*);
		~CalibrationInterface();

		void paint(Graphics& g);

		void buttonClicked(Button* button_);

		void checkCalibrationState();

	private:

		bool isCalibrated;
		String name;

		BrainGridThread* board;
		BrainGridEditor* editor;

		ScopedPointer<UtilityButton> button;

	};
}
//#endif  // __BRAINGRIDEDITOR_H__
