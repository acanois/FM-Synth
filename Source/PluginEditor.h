//
// Created by David Richter on 12/31/24.
//

#pragma once

#include "PluginProcessor.h"

#include <juce_audio_utils/juce_audio_utils.h>

class FMSynthEditor : public juce::AudioProcessorEditor,
                      public juce::MidiKeyboardState::Listener {
public:
    explicit FMSynthEditor(FMSynthProcessor &);

    ~FMSynthEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    // juce::MidiKeyboardState::Listener
    void handleNoteOn(
        juce::MidiKeyboardState *source,
        int midiChannel,
        int midiNoteNumber,
        float velocity
    ) override;

    void handleNoteOff(
        juce::MidiKeyboardState *source,
        int midiChannel,
        int midiNoteNumber,
        float velocity
    ) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthProcessor &processorRef;

    juce::MidiKeyboardState mKeyboardState;
    juce::MidiKeyboardComponent mKeyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FMSynthEditor)
};
