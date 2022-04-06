/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FftFilterAudioProcessor::FftFilterAudioProcessor()
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
    
    mWinSizeComboBoxValue = 3; // 3 is the ID for a window size of 4096 in the associated ComboBox
    mOverlapComboBoxValue = 3; // 3 is the ID for an overlap factor of 4 in the associated ComboBox
}

FftFilterAudioProcessor::~FftFilterAudioProcessor()
{
}

//==============================================================================
void FftFilterAudioProcessor::setOlaBufWindowSize(int windowSize)
{
    windowSize = (windowSize > 4) ? 4 : windowSize;
    windowSize = (windowSize < 1) ? 1 : windowSize;
    
    mWinSizeComboBoxValue = windowSize;
    DBG("windowSize: " + juce::String(mWinSizeComboBoxValue));

    switch(mWinSizeComboBoxValue)
    {
        case 1:
            mOlaBuf.setWindowSize(1024);
            break;
        case 2:
            mOlaBuf.setWindowSize(2048);
            break;
        case 3:
            mOlaBuf.setWindowSize(4096);
            break;
        case 4:
            mOlaBuf.setWindowSize(8192);
            break;
        default:
            break;
    }
    
    // need to update the magnitude scalar array based on the new window size
    computeFftMagScalars();
}

void FftFilterAudioProcessor::setOlaBufOverlap(int overlap)
{
    overlap = (overlap > 4) ? 4 : overlap;
    overlap = (overlap < 1) ? 1 : overlap;
    
    mOverlapComboBoxValue = overlap;
    DBG("overlap: " + juce::String(mOverlapComboBoxValue));

    switch(mOverlapComboBoxValue)
    {
        case 1:
            mOlaBuf.setOverlap(1);
            break;
        case 2:
            mOlaBuf.setOverlap(2);
            break;
        case 3:
            mOlaBuf.setOverlap(4);
            break;
        case 4:
            mOlaBuf.setOverlap(8);
            break;
        default:
            break;
    }
}

void FftFilterAudioProcessor::computeFftMagScalars()
{
    int fftWindowSize = mOlaBuf.getWindowSize();
    
    mFftMagScalars.resize(fftWindowSize);
    mFftMagScalars.fill(0.0f);
    
    for (int band = 0; band < NUMFREQ; band++)
    {
        double thisScalar = mEQBandGain[band];
        
        double lowBound = mEQFREQ[band];
        
        double highBound;
        
        if (band == NUMFREQ-1)
            highBound = mSampleRate/2.0f;
        else
            highBound = mEQFREQ[band+1];
        
        int lowBin = juce::roundToInt((fftWindowSize/mSampleRate)* lowBound);
        
        int highBin = juce::roundToInt((fftWindowSize/mSampleRate)* highBound);
        
        for (int bin = lowBin; bin <= highBin; bin++)
            mFftMagScalars.set(bin, thisScalar);
    }
    
    // now, put those scalars in reverse order for the negative frequencies
    for (int bin = fftWindowSize * 0.5 + 1; bin < fftWindowSize; bin++)
    {
        double thisScalar = mFftMagScalars[fftWindowSize - bin];
        mFftMagScalars.set(bin, thisScalar);
    }
    
        
       // for (int i = 0; i < fftWindowSize; i++)
            //DBG("magScalar[" + juce::String(i) + "]: " + juce::String(mFftMagScalars[i]));
        
        //DBG("Target magnitude scalars computed");
}

void FftFilterAudioProcessor::doFftProcessing(int overlapChannel)
{
    int fftWindowSize = mOlaBuf.getWindowSize();
    int fftOrder = std::log2(fftWindowSize);
    juce::dsp::FFT thisFft(fftOrder);
    
    for (int stereoChannel = 0; stereoChannel < 2; ++stereoChannel)
    {
        // the dsp module's Complex type is just std::complex<type>
        juce::dsp::Complex<float> inBuffer[fftWindowSize];
        juce::dsp::Complex<float> outBuffer[fftWindowSize];
        float* bufPtr;
        
        switch(stereoChannel)
        {
            case 0:
                bufPtr = mOlaBuf.getWritePointerL(overlapChannel);
                break;
            case 1:
                bufPtr = mOlaBuf.getWritePointerR(overlapChannel);
                break;
            default:
                break;
        }
        
        // put the samples from the time-domain buffer into the real part of the forward FFT input buffer
        for (int i = 0; i < fftWindowSize; i++)
        {
            inBuffer[i].real(bufPtr[i]);
            inBuffer[i].imag(0.0f);
            
            // clear the forward FFT output buffer for good measure
            outBuffer[i].real(0.0f);
            outBuffer[i].imag(0.0f);
        }
        
        // do the forward FFT (inverse argument set to FALSE)
        thisFft.perform(inBuffer, outBuffer, false);
        
        // do freq domain work
        
        // data is packed as expected:
        // 0 = DC, N/2 = Nyquist
        // 1 through N/2 - 1 = positive frequency components
        // N/2 + 1 through N - 1 = negative frequency components in reverse order
        
        // scale the positive and negative frequency magnitudes
        for (int i = 0; i < fftWindowSize; i++)
        {
            double thisScalar = mFftMagScalars[i];

            outBuffer[i].real(outBuffer[i].real() * thisScalar);
            outBuffer[i].imag(outBuffer[i].imag() * thisScalar);
        }
        
        // do the inverse FFT (inverse argument set to TRUE, outBuffer/inBuffer swapped)
        // note that juce::FFT does normalization by 1/mFftWindowSizes on the inverse
        thisFft.perform(outBuffer, inBuffer, true);

        // copy the real part of the inverse FFT's output buffer (which is still named inBuffer relative to the forward FFT) into our time-domain buffer
        for (int i = 0; i < fftWindowSize; i++)
            bufPtr[i] = inBuffer[i].real();
    }
}

const juce::String FftFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FftFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FftFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FftFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FftFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FftFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FftFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FftFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FftFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FftFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FftFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampleRate = sampleRate;
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mOlaBuf.setOwnerBlockSize(samplesPerBlock);
    setOlaBufWindowSize(mWinSizeComboBoxValue);
    setOlaBufOverlap(mOverlapComboBoxValue);
    mOlaBuf.debug(true);
    
    
    // now that the OlaBufferStereo window size is set, we can compute the magnitude scalars
//    computeFftMagScalars();
}

void FftFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FftFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FftFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int bufSize = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //======================== OlaBufferStereo ============================
    // copy this block from the host into our ring buffer starting at mRingBufWriteIdx
    mOlaBuf.fillRingBuf(buffer);
    
    // copy the latest n samples from the ring buffer into the appropriate spots in the overlap buffer
    mOlaBuf.fillOverlapBuf();
    
    // check if any channels in the overlap buffer are ready for FFT processing
    for (int overlapChannel = 0; overlapChannel < mOlaBuf.getOverlap(); ++overlapChannel)
    {
        if(mOlaBuf.getProcessFlag(overlapChannel))
        {
            // make a local Hann window sized according to the OlaBufferStereo window size
            juce::dsp::WindowingFunction<float> hannWindow(mOlaBuf.getWindowSize(), juce::dsp::WindowingFunction<float>::hann, false);
            
            // do initial stage of windowing for L and R channels
            mOlaBuf.doWindowing(overlapChannel, hannWindow);
            
            // do it
            doFftProcessing(overlapChannel);
            
            // do final stage of windowing for L and R channels
            mOlaBuf.doWindowing(overlapChannel, hannWindow);
            
            // CLEAR THE FLAG now that processing is done
            mOlaBuf.clearProcessFlag(overlapChannel);
        }
    }
    
    // extract the newest available block from the OLA buffer
    mOlaBuf.outputOlaBlock(buffer);
    
    // advance the write index of the OLA buffer by this buffer's size. this must be called after all other OlaBufferStereo methods above have been called
    mOlaBuf.advanceWriteIdx(bufSize);
    //======================== end OlaBufferStereo =======================
}

//==============================================================================
bool FftFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FftFilterAudioProcessor::createEditor()
{
    return new FftFilterAudioProcessorEditor (*this);
}

//==============================================================================
void FftFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FftFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FftFilterAudioProcessor();
}
