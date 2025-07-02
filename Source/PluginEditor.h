//
// Created by David Richter on 12/31/24.
//

#pragma once

#include "PluginProcessor.h"


class FMSynthEditor : public juce::AudioProcessorEditor {
public:
    explicit FMSynthEditor(FMSynthProcessor &);

    ~FMSynthEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthProcessor &processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FMSynthEditor)
};
