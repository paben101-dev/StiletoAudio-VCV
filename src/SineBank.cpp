#include "plugin.hpp"
#include <random>

// TODO: 
// 1. label controls, etc. on the panel
// 2. create a nice panel artwork
// 3. maybe a 1v/oct input that shifts both faders at once
// 4. maybe a mini quantizer

struct SineBank : Module {
	enum ParamId {
		RANDOMFREQUENCYBUTTON_PARAM,
		SLIDER1_PARAM,
		SLIDER2_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		GATE_INPUT,
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


    void onReset() override {
        initializeOscillators();
    }

	bool loadedFromPatch = false;	
	void dataFromJson(json_t* rootJ) override {
		loadedFromPatch = true;
	}


    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist{100.f, 10000.f};


	// Button trigger
    dsp::SchmittTrigger randomButtonTrigger;
	// Gate Trigger
	dsp::SchmittTrigger gateTrigger;


	// Sine Oscillator
    struct SineOsc {

        float phase = 0.f;
        float freq = 0.f;

		float process(const ProcessArgs& args) {
            phase += freq * args.sampleTime;
            if (phase >= 1.f)
                phase -= 1.f;
            return std::sin(2.f * M_PI * phase);
        }

		void setFrequency(float f) {
			freq = f;
		}

    };
	// the bank of oscillators
	std::vector<SineOsc> oscillators;


	// this converts the faders from linear to log
	struct LogFrequencyParamQuantity : rack::engine::ParamQuantity {
		float getDisplayValue() override {
			// Convert the internal normalized 0–1 "value" to a log-domain frequency
			return std::pow(10.f, ParamQuantity::getValue());
		}

		void setDisplayValue(float displayValue) override {
			// Inverse of above: take a user frequency (like 440 Hz) and log it
			ParamQuantity::setValue(std::log10(displayValue));
		}

		std::string getDisplayValueString() override {
			return rack::string::f("%.2f Hz", getDisplayValue());
		}
};


	SineBank() {

		gen.seed(rd()); // create random distribution

		// oscillators init
		oscillators.resize(12);

		for (auto& osc : oscillators) {
			osc.setFrequency(dist(gen));
		}
		
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(RANDOMFREQUENCYBUTTON_PARAM, 0.f, 1.f, 0.f, "Randomize all frequencies (Hz)");

		configParam<LogFrequencyParamQuantity>(
			SLIDER1_PARAM,
			std::log10(20.f),     // min (log10 of 20)
			std::log10(10000.f),  // max (log10 of 10,000)
			std::log10(440.f),    // default (log10 of 440)
			"Frequency bound 1 (Hz)"
		);

		configParam<LogFrequencyParamQuantity>(
			SLIDER2_PARAM,
			std::log10(20.f),     
			std::log10(10000.f),  
			std::log10(440.f),    
			"Frequency bound 2 (Hz)"
		);

		configInput(GATE_INPUT, "Gate");

		for (int i = 0; i < OUTPUTS_LEN; ++i) {
			configOutput(i, "Sine output");
		}

	}

	void randomizeFrequncies() {
		
		// we are converting from log back to linear here
		// since the sliders are log
		float slider1 = std::pow(10.f, params[SLIDER1_PARAM].getValue());
		float slider2 = std::pow(10.f, params[SLIDER2_PARAM].getValue());

		// Ensure bounds make sense
		if (slider1 > slider2)
			std::swap(slider1, slider2);

		std::uniform_real_distribution<float> dist(slider1, slider2);

		for (auto& osc : oscillators)
			osc.setFrequency(dist(gen));
	}

	void initializeOscillators() {
		oscillators.clear();
		oscillators.resize(12);
		randomizeFrequncies();
	}

    void process(const ProcessArgs& args) override {

        static bool initialized = false;
        if (!initialized) {
            if (!loadedFromPatch) 
				initializeOscillators();
            initialized = true;
        }

		for (int i = 0; i < OUTPUTS_LEN; ++i) {
			outputs[i].setVoltage(5.f * oscillators[i].process(args));
		}

		// Detect a button press
		if (randomButtonTrigger.process(params[RANDOMFREQUENCYBUTTON_PARAM].getValue())) {
			randomizeFrequncies();
		}

		// Detect gate signal
		if (inputs[GATE_INPUT].isConnected()) {
			if (gateTrigger.process(inputs[GATE_INPUT].getVoltage())) {
				randomizeFrequncies();
			}
		}

    }
};

struct SineBankWidget : ModuleWidget {
	SineBankWidget(SineBank* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/SineBank.svg"))); 

		addParam(createParamCentered<LEDButton>(mm2px(Vec(33.373, 12.099)), module, SineBank::RANDOMFREQUENCYBUTTON_PARAM));

		addParam(createParamCentered<LEDSlider>(mm2px(Vec(5.819, 102)), module, SineBank::SLIDER1_PARAM));
		addParam(createParamCentered<LEDSlider>(mm2px(Vec(11.640, 102)), module, SineBank::SLIDER2_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.5, 22)), module, SineBank::GATE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.675, 12.506)), module, SineBank::SINEOUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 21.501)), module, SineBank::SINEOUTPUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 30.497)), module, SineBank::SINEOUTPUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.01, 39.493)), module, SineBank::SINEOUTPUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 49.987)), module, SineBank::SINEOUTPUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 58.983)), module, SineBank::SINEOUTPUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 67.978)), module, SineBank::SINEOUTPUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.132, 76.974)), module, SineBank::SINEOUTPUT8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 88.828)), module, SineBank::SINEOUTPUT9_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 97.824)), module, SineBank::SINEOUTPUT10_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 106.819)), module, SineBank::SINEOUTPUT11_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.373, 115.815)), module, SineBank::SINEOUTPUT12_OUTPUT));
	}
};

Model* modelSineBank = createModel<SineBank, SineBankWidget>("SineBank");