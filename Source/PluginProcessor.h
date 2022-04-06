/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define NUMFREQ 6
#define EQGAIN 0.0

//==============================================================================
/**
*/
class FftFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FftFilterAudioProcessor();
    ~FftFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    int mWinSizeComboBoxValue;
    int mOverlapComboBoxValue;
    
    void setOlaBufWindowSize(int windowSize);
    void setOlaBufOverlap(int overlap);
    
    void computeFftMagScalars();
    
    double mEQBandGain[NUMFREQ] = {1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f};
    double mEQQ[NUMFREQ] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    double mEQFREQ[NUMFREQ] = {50.0f, 200.0f, 400.0f, 600.0f, 800.0f, 1200.0f};
    double mEQGainSliderValue;
    
    //int mEQBandGain
    
private:
    //==============================================================================
    atec::OlaBufferStereo mOlaBuf;
    void doFftProcessing(int overlapChannel);

    juce::Array<double> mFftMagScalars;
    
    double mSampleRate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FftFilterAudioProcessor)
};
