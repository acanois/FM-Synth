//
// Created by David Richter on 12/31/24.
//

#include "PluginEditor.h"

FMSynthEditor::FMSynthEditor(FMSynthProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p) {
    juce::ignoreUnused(processorRef);

    setSize(400, 300);
}

FMSynthEditor::~FMSynthEditor() = default;

void FMSynthEditor::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void FMSynthEditor::resized() {
}
