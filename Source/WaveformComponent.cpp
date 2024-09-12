#include <JuceHeader.h>
#include "WaveformComponent.h"

WaveformComponent::WaveformComponent()
{
    // value here sets number of times the component is repainted in ms
    // keep high for low fps for testing/more performance
    // for fps equiv, 60fps is around 16, 30fps is around 33, a value of 1 would be 1000fps
    startTimer(33);
};

WaveformComponent::~WaveformComponent()
{
    stopTimer();
}

void WaveformComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
    g.setColour(juce::Colours::white);

    if (waveform.size() > 1)
    {
        juce::Path waveformPath;
        auto area = getLocalBounds();
        auto width = area.getWidth();
        auto height = area.getHeight();

        // centres the wave to the middle of component
        float halfHeight = (float)height / 2.0f;
        float scaleX = (float)width / (float)waveform.size();

        waveformPath.startNewSubPath(0, halfHeight);

        for (size_t i = 0; i < waveform.size(); ++i)
        {
            float x = (float)i * scaleX;
            // Can increase the waveheight by adding a scaling factor to the end e.g.;
            //      float y = halfHeight - (waveform[i] * halfHeight * 2.0f);
            float y = halfHeight - (waveform[i] * halfHeight * 2.0f);
            waveformPath.lineTo(x, y);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
    }
}

void WaveformComponent::setWaveform(const std::vector<float>& newWaveform)
{
    waveform = newWaveform;
}

void WaveformComponent::timerCallback()
{
    repaint();
}