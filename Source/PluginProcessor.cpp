/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WillHPF3AudioProcessor::WillHPF3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

WillHPF3AudioProcessor::~WillHPF3AudioProcessor()
{
}

//==============================================================================
const juce::String WillHPF3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WillHPF3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WillHPF3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WillHPF3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WillHPF3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WillHPF3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WillHPF3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void WillHPF3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WillHPF3AudioProcessor::getProgramName (int index)
{
    return {};
}

void WillHPF3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WillHPF3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    z1L = 0.0;
    z1R = 0.0;
    setB1(0.0);
    setA0(0.0);
}

void WillHPF3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WillHPF3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WillHPF3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //LEFT CHANNEL PROCESSING
    int channel = 0;
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto* channelData = buffer.getWritePointer (channel);
        // Input sample is x(n)
        float xn = channelData[sample];
        // READ: Delay sample is y(n-1)
        float yn_1 = z1L;
        // Difference Equation
        float yn = a0L*xn - b1L*yn_1;
        // WRITE: Delay with current y(n)
        z1L = yn;
        // Output sample is y(n)
        channelData[sample] = yn;
    }
    //RIGHT CHANNEL PROCESSING
    channel = 1; //right channel
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto* channelData = buffer.getWritePointer (channel);
        // Input sample is x(n)
        float xn = channelData[sample];
        // READ: Delay sample is y(n-1)
        float yn_1 = z1R;
        // Difference Equation
        float yn = a0R*xn - b1R*yn_1;
        // WRITE: Delay with current y(n)
        z1R = yn;
        // Output sample is y(n)
        channelData[sample] = yn;
    }
}

//==============================================================================
bool WillHPF3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WillHPF3AudioProcessor::createEditor()
{
    return new WillHPF3AudioProcessorEditor (*this);
}

//==============================================================================
void WillHPF3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WillHPF3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WillHPF3AudioProcessor();
}
