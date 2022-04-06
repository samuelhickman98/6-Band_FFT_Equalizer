/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FftFilterAudioProcessorEditor::FftFilterAudioProcessorEditor (FftFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);
    
    mWinSizeComboBox.addItem("1024", 1);
    mWinSizeComboBox.addItem("2048", 2);
    mWinSizeComboBox.addItem("4096", 3);
    mWinSizeComboBox.addItem("8192", 4);
    mWinSizeComboBox.setSelectedId(audioProcessor.mWinSizeComboBoxValue);
    addAndMakeVisible(&mWinSizeComboBox);
    mWinSizeComboBox.addListener(this);

    mOverlapComboBox.addItem("1", 1);
    mOverlapComboBox.addItem("2", 2);
    mOverlapComboBox.addItem("4", 3);
    mOverlapComboBox.addItem("8", 4);
    mOverlapComboBox.setSelectedId(audioProcessor.mOverlapComboBoxValue);
    addAndMakeVisible(&mOverlapComboBox);
    mOverlapComboBox.addListener(this);
    
    mBandOneGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandOneGainSlider.setRange(-30.0f, 30.0f);
    mBandOneGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandOneGainSlider);
    mBandOneGainSlider.addListener(this);
    
    mBandTwoGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandTwoGainSlider.setRange(-30.0f, 30.0f);
    mBandTwoGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandTwoGainSlider);
    mBandTwoGainSlider.addListener(this);
    
    mBandThreeGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandThreeGainSlider.setRange(-30.0f, 30.0f);
    mBandThreeGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandThreeGainSlider);
    mBandThreeGainSlider.addListener(this);
    
    mBandFourGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandFourGainSlider.setRange(-30.0f, 30.0f);
    mBandFourGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandFourGainSlider);
    mBandFourGainSlider.addListener(this);
    
    mBandFiveGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandFiveGainSlider.setRange(-30.0f, 30.0f);
    mBandFiveGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandFiveGainSlider);
    mBandFiveGainSlider.addListener(this);
    
    mBandSixGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    mBandSixGainSlider.setRange(-30.0f, 30.0f);
    mBandSixGainSlider.setValue(0.0f);
    addAndMakeVisible(&mBandSixGainSlider);
    mBandSixGainSlider.addListener(this);
    
    /*
    mBandOneQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandOneQSlider.setRange(0.10f, 10.0f, 0.01f);
    mBandOneQSlider.setValue(0.0f);
    addAndMakeVisible(&mBandOneQSlider);
    mBandOneQSlider.addListener(this);
    
    mBandTwoQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandTwoQSlider.setRange(0.10f, 10.0f, 0.01f);
    mBandTwoQSlider.setValue(0.0f);
    addAndMakeVisible(&mBandTwoQSlider);
    mBandTwoQSlider.addListener(this);
    
    mBandThreeQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandThreeQSlider.setRange(0.10f, 10.0f, 0.01f);
    mBandThreeQSlider.setValue(0.0f);
    addAndMakeVisible(&mBandThreeQSlider);
    mBandThreeQSlider.addListener(this);
    
    mBandFourQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandFourQSlider.setRange(0.10f, 10.0f, 0.01f);
    //mBandFourQSlider.setValue(audioProcessor.x);
    addAndMakeVisible(&mBandFourQSlider);
    mBandFourQSlider.addListener(this);
    
    mBandFiveQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandFiveQSlider.setRange(0.10f, 10.0f, 0.01f);
    //mBandFiveQSlider.setValue(audioProcessor.x);
    addAndMakeVisible(&mBandFiveQSlider);
    mBandFiveQSlider.addListener(this);
    
    mBandSixQSlider.setSliderStyle(juce::Slider::Rotary);
    mBandSixQSlider.setRange(0.10f, 10.0f, 0.01f);
    //mBandSixQSlider.setValue(audioProcessor.x);
    addAndMakeVisible(&mBandSixQSlider);
    mBandSixQSlider.addListener(this);
    */
    mBandOneFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandOneFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandOneFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandOneFreqSlider);
    mBandOneFreqSlider.addListener(this);
    
    mBandTwoFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandTwoFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandTwoFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandTwoFreqSlider);
    mBandTwoFreqSlider.addListener(this);
    
    mBandThreeFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandThreeFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandThreeFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandThreeFreqSlider);
    mBandThreeFreqSlider.addListener(this);
    
    mBandFourFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandFourFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandFourFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandFourFreqSlider);
    mBandFourFreqSlider.addListener(this);
    
    mBandFiveFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandFiveFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandFiveFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandFiveFreqSlider);
    mBandFiveFreqSlider.addListener(this);
    
    mBandSixFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mBandSixFreqSlider.setRange(20.0f, 20000.0f, 0.01f);
    mBandSixFreqSlider.setValue(0.0f);
    addAndMakeVisible(&mBandSixFreqSlider);
    mBandSixFreqSlider.addListener(this);
    
    addAndMakeVisible(&mBandOneGainLabel);
    mBandOneGainLabel.setText("Low", juce::dontSendNotification);
    mBandOneGainLabel.attachToComponent(&mBandOneGainSlider, true);
    mBandOneGainLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    mBandOneGainLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(&mBandTwoGainLabel);
    mBandTwoGainLabel.setText("Low-Mid", juce::dontSendNotification);
    mBandTwoGainLabel.attachToComponent(&mBandTwoGainSlider, true);
    mBandTwoGainLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    mBandTwoGainLabel.setJustificationType(juce::Justification::bottomRight);
    
    addAndMakeVisible(&mBandThreeGainLabel);
    mBandThreeGainLabel.setText("Mid 1", juce::dontSendNotification);
    mBandThreeGainLabel.attachToComponent(&mBandThreeGainSlider, true);
    mBandThreeGainLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    mBandThreeGainLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(&mBandFourGainLabel);
    mBandFourGainLabel.setText("Mid 2", juce::dontSendNotification);
    mBandFourGainLabel.attachToComponent(&mBandFourGainSlider, true);
    mBandFourGainLabel.setColour(juce::Label::textColourId, juce::Colours::limegreen);
    mBandFourGainLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(&mBandFiveGainLabel);
    mBandFiveGainLabel.setText("High-Mid", juce::dontSendNotification);
    mBandFiveGainLabel.attachToComponent(&mBandFiveGainSlider, true);
    mBandFiveGainLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    mBandFiveGainLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(&mBandSixGainLabel);
    mBandSixGainLabel.setText("High", juce::dontSendNotification);
    mBandSixGainLabel.attachToComponent(&mBandSixGainSlider, true);
    mBandSixGainLabel.setColour(juce::Label::textColourId, juce::Colours::indigo);
    mBandSixGainLabel.setJustificationType(juce::Justification::centredBottom);
    /*
    addAndMakeVisible(&mBandOneQLabel);
    mBandOneQLabel.setText("Q", juce::dontSendNotification);
    mBandOneQLabel.attachToComponent(&mBandOneQSlider, true);
    mBandOneQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandOneQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandTwoQLabel);
    mBandTwoQLabel.setText("Q", juce::dontSendNotification);
    mBandTwoQLabel.attachToComponent(&mBandTwoQSlider, true);
    mBandTwoQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandTwoQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandThreeQLabel);
    mBandThreeQLabel.setText("Q", juce::dontSendNotification);
    mBandThreeQLabel.attachToComponent(&mBandThreeQSlider, true);
    mBandThreeQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandThreeQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandFourQLabel);
    mBandFourQLabel.setText("Q", juce::dontSendNotification);
    mBandFourQLabel.attachToComponent(&mBandFourQSlider, true);
    mBandFourQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandFourQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandFiveQLabel);
    mBandFiveQLabel.setText("Q", juce::dontSendNotification);
    mBandFiveQLabel.attachToComponent(&mBandFiveQSlider, true);
    mBandFiveQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandFiveQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandFiveQLabel);
    mBandFiveQLabel.setText("Q", juce::dontSendNotification);
    mBandFiveQLabel.attachToComponent(&mBandFiveQSlider, true);
    mBandFiveQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandFiveQLabel.setJustificationType(juce::Justification::right);
    
    addAndMakeVisible(&mBandSixQLabel);
    mBandSixQLabel.setText("Q", juce::dontSendNotification);
    mBandSixQLabel.attachToComponent(&mBandSixQSlider, true);
    mBandSixQLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mBandSixQLabel.setJustificationType(juce::Justification::right);
    */
    addAndMakeVisible(&mBandOneFreqLabel);
    mBandOneFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandOneFreqLabel.attachToComponent(&mBandOneFreqSlider, true);
    mBandOneFreqLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    mBandOneFreqLabel.setJustificationType(juce::Justification::left);
    
    addAndMakeVisible(&mBandTwoFreqLabel);
    mBandTwoFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandTwoFreqLabel.attachToComponent(&mBandTwoFreqSlider, true);
    mBandTwoFreqLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    mBandTwoFreqLabel.setJustificationType(juce::Justification::left);
    
    addAndMakeVisible(&mBandThreeFreqLabel);
    mBandThreeFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandThreeFreqLabel.attachToComponent(&mBandThreeFreqSlider, true);
    mBandThreeFreqLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    mBandThreeFreqLabel.setJustificationType(juce::Justification::left);
    
    addAndMakeVisible(&mBandFourFreqLabel);
    mBandFourFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandFourFreqLabel.attachToComponent(&mBandFourFreqSlider, true);
    mBandFourFreqLabel.setColour(juce::Label::textColourId, juce::Colours::limegreen);
    mBandFourFreqLabel.setJustificationType(juce::Justification::left);
    
    addAndMakeVisible(&mBandFiveFreqLabel);
    mBandFiveFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandFiveFreqLabel.attachToComponent(&mBandFiveFreqSlider, true);
    mBandFiveFreqLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    mBandFiveFreqLabel.setJustificationType(juce::Justification::left);
    
    addAndMakeVisible(&mBandSixFreqLabel);
    mBandSixFreqLabel.setText("Frequency", juce::dontSendNotification);
    mBandSixFreqLabel.attachToComponent(&mBandSixFreqSlider, true);
    mBandSixFreqLabel.setColour(juce::Label::textColourId, juce::Colours::indigo);
    mBandSixFreqLabel.setJustificationType(juce::Justification::left);
}

FftFilterAudioProcessorEditor::~FftFilterAudioProcessorEditor()
{
    mWinSizeComboBox.removeListener(this);
    mOverlapComboBox.removeListener(this);
    
    mBandOneGainSlider.removeListener(this);
    mBandTwoGainSlider.removeListener(this);
    mBandThreeGainSlider.removeListener(this);
    mBandFourGainSlider.removeListener(this);
    mBandFiveGainSlider.removeListener(this);
    mBandSixGainSlider.removeListener(this);
    
    /*
    mBandOneQSlider.removeListener(this);
    mBandTwoQSlider.removeListener(this);
    mBandThreeQSlider.removeListener(this);
    mBandFourQSlider.removeListener(this);
    mBandFiveQSlider.removeListener(this);
    mBandSixQSlider.removeListener(this);
    */
    mBandOneFreqSlider.removeListener(this);
    mBandTwoFreqSlider.removeListener(this);
    mBandThreeFreqSlider.removeListener(this);
    mBandFourFreqSlider.removeListener(this);
    mBandFiveFreqSlider.removeListener(this);
    mBandSixFreqSlider.removeListener(this);
}

//==============================================================================
void FftFilterAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &mWinSizeComboBox)
        audioProcessor.setOlaBufWindowSize(mWinSizeComboBox.getSelectedId());
    else if (comboBox == &mOverlapComboBox)
        audioProcessor.setOlaBufOverlap(mOverlapComboBox.getSelectedId());
}

void FftFilterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mBandOneGainSlider)
    {
        audioProcessor.mEQBandGain[0] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    else if (slider == &mBandTwoGainSlider)
    {
        audioProcessor.mEQBandGain[1] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandThreeGainSlider)
    {
        audioProcessor.mEQBandGain[2] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandFourGainSlider)
    {
        audioProcessor.mEQBandGain[3] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandFiveGainSlider)
    {
        audioProcessor.mEQBandGain[4] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandSixGainSlider)
    {
        audioProcessor.mEQBandGain[5] = juce::Decibels::decibelsToGain(slider->getValue());
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandOneFreqSlider)
    {
        audioProcessor.mEQFREQ[0] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandTwoFreqSlider)
    {
        audioProcessor.mEQFREQ[1] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandThreeFreqSlider)
    {
        audioProcessor.mEQFREQ[2] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandFourFreqSlider)
    {
        audioProcessor.mEQFREQ[3] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandFiveFreqSlider)
    {
        audioProcessor.mEQFREQ[4] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
    
    else if (slider == &mBandSixFreqSlider)
    {
        audioProcessor.mEQFREQ[5] = slider->getValue();
        audioProcessor.computeFftMagScalars();
    }
}

void FftFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::teal);
    
    g.setColour (juce::Colours::skyblue);
    g.setFont (40.0f);
    g.drawFittedText ("6-Band EQ", getLocalBounds(), juce::Justification::centredTop, 1);
}

void FftFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mWinSizeComboBox.setBounds(50, 50, 100, 30);
    mOverlapComboBox.setBounds(650, 50, 100, 30);
    
    mBandOneGainSlider.setBounds(60, 100, 80, 200);
    mBandTwoGainSlider.setBounds(200, 100, 80, 200);
    mBandThreeGainSlider.setBounds(320, 100, 80, 200);
    mBandFourGainSlider.setBounds(440, 100, 80, 200);
    mBandFiveGainSlider.setBounds(580, 100, 80, 200);
    mBandSixGainSlider.setBounds(700, 100, 80, 200);
    
    /*
    mBandOneQSlider.setBounds(70, 300, 120, 120);
    mBandTwoQSlider.setBounds(170, 300, 120, 120);
    mBandThreeQSlider.setBounds(270, 300, 120, 120);
    mBandFourQSlider.setBounds(370, 300, 120, 120);
    mBandFiveQSlider.setBounds(470, 300, 120, 120);
    mBandSixQSlider.setBounds(570, 300, 120, 120);
    
    mBandOneFreqSlider.setBounds(110, 330, 130, 50);
    mBandTwoFreqSlider.setBounds(210, 300, 130, 50);
    mBandThreeFreqSlider.setBounds(310, 330, 130, 50);
    mBandFourFreqSlider.setBounds(410, 300, 130, 50);
    mBandFiveFreqSlider.setBounds(510, 330, 130, 50);
    mBandSixFreqSlider.setBounds(610, 300, 130, 50);
     */
}
