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
class FftFilterAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ComboBox::Listener, public juce::Slider::Listener
{
public:
    FftFilterAudioProcessorEditor (FftFilterAudioProcessor&);
    ~FftFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FftFilterAudioProcessor& audioProcessor;
    
    juce::ComboBox mWinSizeComboBox;
    juce::ComboBox mOverlapComboBox;
    
    juce::Slider mBandOneGainSlider;
    juce::Slider mBandTwoGainSlider;
    juce::Slider mBandThreeGainSlider;
    juce::Slider mBandFourGainSlider;
    juce::Slider mBandFiveGainSlider;
    juce::Slider mBandSixGainSlider;
    
    juce::Label mBandOneGainLabel;
    juce::Label mBandTwoGainLabel;
    juce::Label mBandThreeGainLabel;
    juce::Label mBandFourGainLabel;
    juce::Label mBandFiveGainLabel;
    juce::Label mBandSixGainLabel;
    
    juce::Slider mBandOneQSlider;
    juce::Slider mBandTwoQSlider;
    juce::Slider mBandThreeQSlider;
    juce::Slider mBandFourQSlider;
    juce::Slider mBandFiveQSlider;
    juce::Slider mBandSixQSlider;
    
    juce::Label mBandOneQLabel;
    juce::Label mBandTwoQLabel;
    juce::Label mBandThreeQLabel;
    juce::Label mBandFourQLabel;
    juce::Label mBandFiveQLabel;
    juce::Label mBandSixQLabel;
    
    juce::Slider mBandOneFreqSlider;
    juce::Slider mBandTwoFreqSlider;
    juce::Slider mBandThreeFreqSlider;
    juce::Slider mBandFourFreqSlider;
    juce::Slider mBandFiveFreqSlider;
    juce::Slider mBandSixFreqSlider;
    
    juce::Label mBandOneFreqLabel;
    juce::Label mBandTwoFreqLabel;
    juce::Label mBandThreeFreqLabel;
    juce::Label mBandFourFreqLabel;
    juce::Label mBandFiveFreqLabel;
    juce::Label mBandSixFreqLabel;
    
    juce::Slider mEQGainSlider;
    
    juce::Label mEQGainLabel;
    
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void sliderValueChanged(juce::Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FftFilterAudioProcessorEditor)
};
