#include "plugin.hpp"
#include <random>

struct SineBank : Module {
	enum ParamId {
		RANDOMFREQUENCYBUTTON_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		SINEOUTPUT1_OUTPUT,
		SINEOUTPUT2_OUTPUT,
		SINEOUTPUT3_OUTPUT,
		SINEOUTPUT4_OUTPUT,
		SINEOUTPUT5_OUTPUT,
		SINEOUTPUT6_OUTPUT,
		SINEOUTPUT7_OUTPUT,
		SINEOUTPUT8_OUTPUT,
		SINEOUTPUT9_OUTPUT,
		SINEOUTPUT10_OUTPUT,
		SINEOUTPUT11_OUTPUT,
		SINEOUTPUT12_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist{0.f, 10000.f};
    float randomFreq = 0.f;

	// Button trigger
    dsp::SchmittTrigger randomButtonTrigger;

	SineBank() {

		gen.seed(rd()); // create random distribution
        randomFreq = dist(gen); // random frequency call setup
		
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(RANDOMFREQUENCYBUTTON_PARAM, 0.f, 1.f, 0.f, "");
		configOutput(SINEOUTPUT1_OUTPUT, "");
		configOutput(SINEOUTPUT2_OUTPUT, "");
		configOutput(SINEOUTPUT3_OUTPUT, "");
		configOutput(SINEOUTPUT4_OUTPUT, "");
		configOutput(SINEOUTPUT5_OUTPUT, "");
		configOutput(SINEOUTPUT6_OUTPUT, "");
		configOutput(SINEOUTPUT7_OUTPUT, "");
		configOutput(SINEOUTPUT8_OUTPUT, "");
		configOutput(SINEOUTPUT9_OUTPUT, "");
		configOutput(SINEOUTPUT10_OUTPUT, "");
		configOutput(SINEOUTPUT11_OUTPUT, "");
		configOutput(SINEOUTPUT12_OUTPUT, "");
	}

    // Sine Oscillator
    struct SineOsc {

        float phase = 0.f;
        
		float process(float freq, const ProcessArgs& args) {
            phase += freq * args.sampleTime;
            if (phase >= 1.f)
                phase -= 1.f;
            return std::sin(2.f * M_PI * phase);
        }

    };

	// each output gets its own oscillator
    SineOsc osc1;

    void process(const ProcessArgs& args) override {

        float sine = osc1.process(randomFreq, args);
        outputs[SINEOUTPUT1_OUTPUT].setVoltage(5.f * sine);

		// Detect a button press
        if (randomButtonTrigger.process(params[RANDOMFREQUENCYBUTTON_PARAM].getValue())) {
            randomFreq = dist(gen);  // pick new random frequency once per press
        }

    }
};

struct SineBankWidget : ModuleWidget {
	SineBankWidget(SineBank* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/SineBank.svg"))); 

		addParam(createParamCentered<LEDButton>(mm2px(Vec(33.373, 15.099)), module, SineBank::RANDOMFREQUENCYBUTTON_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.675, 15.506)), module, SineBank::SINEOUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 24.501)), module, SineBank::SINEOUTPUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 33.497)), module, SineBank::SINEOUTPUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 42.493)), module, SineBank::SINEOUTPUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 52.987)), module, SineBank::SINEOUTPUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 61.983)), module, SineBank::SINEOUTPUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 70.978)), module, SineBank::SINEOUTPUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 79.974)), module, SineBank::SINEOUTPUT8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 91.828)), module, SineBank::SINEOUTPUT9_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 100.824)), module, SineBank::SINEOUTPUT10_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 109.819)), module, SineBank::SINEOUTPUT11_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 118.815)), module, SineBank::SINEOUTPUT12_OUTPUT));
	}
};


Model* modelSineBank = createModel<SineBank, SineBankWidget>("SineBank");