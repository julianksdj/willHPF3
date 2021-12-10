/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WillHPF3AudioProcessorEditor::WillHPF3AudioProcessorEditor (WillHPF3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
    addAndMakeVisible(b1Slider);
    b1Slider.setRange (-1.0, 1.0);
    b1Slider.addListener (this);
    addAndMakeVisible (b1Label);
    b1Label.setText ("b1 cofficient", juce::dontSendNotification);
    b1Label.attachToComponent (&b1Slider, true);
    
    addAndMakeVisible(a0Slider);
    a0Slider.setRange (-1.0, 1.0);
    a0Slider.addListener (this);
    addAndMakeVisible (a0Label);
    a0Label.setText ("a0 cofficient", juce::dontSendNotification);
    a0Label.attachToComponent (&a0Slider, true);
}

WillHPF3AudioProcessorEditor::~WillHPF3AudioProcessorEditor()
{
}

//==============================================================================
void WillHPF3AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (19.0f);
    g.drawFittedText ("Will Pirkle - General First-Order Feed-Back Filter", 0, 30, getWidth(), 5, juce::Justification::centred, 1);
    g.setFont (15.0f);
    g.drawFittedText ("y(n) = a0 * x(n) - b1 * y(n-1)", 0, 60, getWidth(), 5, juce::Justification::centred, 1);
}

void WillHPF3AudioProcessorEditor::resized()
{
    b1Slider.setBounds(100,140,250,20);
    a0Slider.setBounds(100,160,250,20);
}


void WillHPF3AudioProcessorEditor::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &b1Slider)
    {
        audioProcessor.setB1(b1Slider.getValue());
    }
    if (slider == &a0Slider)
    {
        audioProcessor.setA0(a0Slider.getValue());
    }
    
}


