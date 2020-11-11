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

#include "BrainGridEditor.h"
#include <cmath>
#include <stdio.h>

#include "BrainGridThread.h"

using namespace BrainGrid;

#ifdef WIN32
#if (_MSC_VER < 1800) //round doesn't exist on MSVC prior to 2013 version
inline double round(double x)
{
    return floor(x+0.5);
}
#endif
#endif

BrainGridEditor::BrainGridEditor(GenericProcessor* parentNode,
                             BrainGridThread* board_,
                             bool useDefaultParameterEditors=true
                            )
    : GenericEditor(parentNode, useDefaultParameterEditors), board(board_)
{

    /*
    ledButton = new UtilityButton("LED", Font("Small Text", 13, Font::plain));
    ledButton->setRadius(3.0f);
    ledButton->setBounds(80, 80, 20, 20);
    ledButton->addListener(this);
    ledButton->setClickingTogglesState(false);
    ledButton->setTooltip("Toggle board LEDs");
    addAndMakeVisible(ledButton);
    ledButton->setToggleState(true, dontSendNotification);
    */
    resetButton = new UtilityButton("RESET", Font("Small Text", 13, Font::plain));
    resetButton->setBounds(10,20,50,50);
    resetButton->addListener(this);
    resetButton->setClickingTogglesState(true);
    resetButton->setTooltip("Toggles the Reset");
    addAndMakeVisible(resetButton);
    resetButton->setToggleState(true, dontSendNotification);
    
    startButton = new UtilityButton("START", Font("Small Text", 13, Font::plain));
    startButton->setBounds(65,20,50,50);
    startButton->addListener(this);
    startButton->setClickingTogglesState(true);
    startButton->setTooltip("Toggles the Reset");
    addAndMakeVisible(startButton);
    startButton->setToggleState(true, dontSendNotification);
    /*
    sampleRateSlider = new ThresholdSlider(Font::plain);
    sampleRateSlider->setBounds(150,10,75,75);
    addAndMakeVisible(sampleRateSlider);
    sampleRateSlider->addListener(this);
    sampleRateSlider->setActive(true);
    sampleRateSlider->setRange(1000.0f, 30000.0f, 1000.0f);
    */
}

void BrainGridEditor::buttonEvent(Button* button)
{
    if (button == ledButton)
    {
        board->enableBoardLeds(button->getToggleState());
    }
    else if (button == resetButton)
    {
        board->resetBoard(button->getToggleState());
        std::cout << "Reset Button state: " << button->getToggleState() << std::endl;
    }
    else if (button == startButton)
    {
        std::cout << "Start Button clicked. Ignore" << std::endl;
    }
    
}

void BrainGridEditor::sliderEvent(Slider* slider)
{
    board->setSampleRate(slider->getValue());
    printf("Sample Rate has been set to %2f\n",slider->getValue());
}