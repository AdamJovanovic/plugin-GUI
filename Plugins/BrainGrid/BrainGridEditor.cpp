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

    ledButton = new UtilityButton("LED", Font("Small Text", 13, Font::plain));
    ledButton->setRadius(3.0f);
    ledButton->setBounds(50, 50, 50, 50);
    ledButton->addListener(this);
    ledButton->setClickingTogglesState(true);
    ledButton->setTooltip("Toggle board LEDs");
    addAndMakeVisible(ledButton);
    ledButton->setToggleState(true, dontSendNotification);

}

void BrainGridEditor::buttonEvent(Button* button)
{
    if (button == ledButton)
    {
        board->enableBoardLeds(button->getToggleState());
    }
}