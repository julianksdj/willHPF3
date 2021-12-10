/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WillHPF3AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener
{
public:
    WillHPF3AudioProcessorEditor (WillHPF3AudioProcessor&);
    ~WillHPF3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //==============================================================================
    void sliderValueChanged (juce::Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WillHPF3AudioProcessor& audioProcessor;
    
    juce::Slider b1Slider;
    juce::Label b1Label;
    
    juce::Slider a0Slider;
    juce::Label a0Label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WillHPF3AudioProcessorEditor)
};


