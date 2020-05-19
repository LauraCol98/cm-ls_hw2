/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
ChorusFxAudioProcessor::ChorusFxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    wet = 0;
    amt_delay = 0.020f; //20ms
    phase = 0;
    sweep_width = 0.005; // 5ms
    dpw = 0;
}

ChorusFxAudioProcessor::~ChorusFxAudioProcessor()
{
}

//==============================================================================
const String ChorusFxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusFxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusFxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusFxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusFxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusFxAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusFxAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusFxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusFxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delaybuf.setSize(2, sampleRate * 2);
}

void ChorusFxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusFxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void ChorusFxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* channelOutDataL = buffer.getWritePointer(0);
    auto* channelOutDataR = buffer.getWritePointer(1);
    float sw_now = sweep_width;
    float wet_now = wet;

    int buflen = buffer.getNumSamples();
    int delaylen = delaybuf.getNumSamples();

   
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelInData = buffer.getReadPointer (channel);

        for (int sample = 0; sample < buflen; ++sample)
        {
            float delay_now = ( amt_delay + (double)sweep_width * get_LFO(phase)) * getSampleRate(); //in samples
            float out = (1 - wet_now) * channelInData[sample] + wet_now * linear_int(delay_now, delaylen, channel);

            buffer.setSample(channel,sample, out);

            phase = get_nextPhase(phase);
            delaybuf.setSample(channel, sample, channelInData[sample]);

            ++dpw % delaylen;
        }

       
    }
}

//==============================================================================
bool ChorusFxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusFxAudioProcessor::createEditor()
{
    return new ChorusFxAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusFxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChorusFxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float ChorusFxAudioProcessor::get_LFO(float phase) {
    return 0.5 + 0.5 * sinf(2 * M_PI * phase);
}

float ChorusFxAudioProcessor::linear_int(float delay_now, int delaylen, int channel) {
    float dpr = fmodf((float)dpw - (float)delay_now + (float)delaylen, (float)delaylen);
    int prev = floor(dpr);
    float diff = dpr - prev;
    int next =  (prev + 1) % delaylen;

    return diff * delaybuf.getSample(channel, next) + (1 - diff) * delaybuf.getSample(channel, prev);

}

float ChorusFxAudioProcessor::get_nextPhase(float phi) {
    float ir = 1 / getSampleRate();
    float freq = 3;
    phi += freq * ir;
    if (phi >= 1)
        phi -= 1.0;

    return phi;

}

void ChorusFxAudioProcessor::set_amt_delay(float val)
{
    wet = val;
}

void ChorusFxAudioProcessor::set_sweep_width(float val)
{
    wet = val;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusFxAudioProcessor();
}
