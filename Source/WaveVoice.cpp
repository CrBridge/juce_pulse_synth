#include <JuceHeader.h>
#include "WaveVoice.h"

WaveVoice::WaveVoice()
{
    //working good
    processorChain.get<gainIndex>().setGainLinear(1.0f);
}

bool WaveVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<WaveSound*> (sound) != nullptr;
}

void WaveVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();

    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

void WaveVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (angleDelta != 0.0)
    {
        if (tailOff > 0.0)
        {
            while (--numSamples >= 0)
            {
                //auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);
                //square wave test
                auto currentSample = ((float)(std::sin(currentAngle) > (1.0f - dutyCycle) * 2.0f - 1.0f) ? 0.3 : -0.3) * level * tailOff;

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;

                tailOff *= 0.99;

                if (tailOff <= 0.005)
                {
                    clearCurrentNote();

                    angleDelta = 0.0;
                    break;
                }
            }
        }
        else
        {
            while (--numSamples >= 0)
            {
                //auto currentSample = (float)(std::sin(currentAngle) * level);
                //square wave test
                auto currentSample = ((float)(std::sin(currentAngle) > (1.0f - dutyCycle) * 2.0f - 1.0f) ? 0.3 : -0.3) * level;

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;
            }
        }
        juce::dsp::AudioBlock<float> audioBlock(outputBuffer);
        juce::dsp::ProcessContextReplacing<float> context(audioBlock);
        processorChain.process(context);
    }
}

void WaveVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0)
            tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

void WaveVoice::pitchWheelMoved(int newPitchWheelValue) {
    juce::ignoreUnused(newPitchWheelValue);
}

void WaveVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    juce::ignoreUnused(controllerNumber);
    juce::ignoreUnused(newControllerValue);
}

void WaveVoice::setDutyCycle(float newDutyCycle)
{
    dutyCycle = juce::jlimit(0.0f, 1.0f, newDutyCycle);
}

