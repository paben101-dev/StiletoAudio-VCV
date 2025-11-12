#include "plugin.hpp"

// TODO:
// 1. maybe expand size horizontally for more controls
// 2. CV inputs for attenuator control

struct VCABank : Module {
	enum ParamId {
		GAIN_1,
		GAIN_2,
		GAIN_3,
		GAIN_4,
		GAIN_5,
		GAIN_6,
		GAIN_7,
		GAIN_8,
		GAIN_9,
		GAIN_10,

		RAMP_SWITCH,

		PARAMS_LEN
	};
	enum InputId {
		INPUT_1,
		INPUT_2,
		INPUT_3,
		INPUT_4,
		INPUT_5,
		INPUT_6,
		INPUT_7,
		INPUT_8,
		INPUT_9,
		INPUT_10,

		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_1,
		OUTPUT_2,
		OUTPUT_3,
		OUTPUT_4,
		OUTPUT_5,
		OUTPUT_6,
		OUTPUT_7,
		OUTPUT_8,
		OUTPUT_9,
		OUTPUT_10,

		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	VCABank() {

			config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
			
			for (auto i = 0; i < OUTPUTS_LEN; ++i) {
				configParam(i, 0.f, 1.f, 0.5, "Input");
				configParam(i, 0.f, 1.f, 0.5, "Attenuator");
				configParam(i, 0.f, 1.f, 0.5, "Output");
			}

			configSwitch(VCABank::RAMP_SWITCH, 0.f, 1.f, 0.f, "Response", {"Linear", "Exponential"});

		}

	void process(const ProcessArgs& args) override {

		bool expo = params[RAMP_SWITCH].getValue() > 0.5f;
		for (int i = 0; i < OUTPUTS_LEN; ++i) {
			float in = inputs[i].getVoltage();
			float gain = params[i].getValue();

			if (expo)
				gain = std::pow(gain, 2.0f);  // exponential response

			outputs[i].setVoltage(in * gain);
		
		}
	}

};


struct VCABankWidget : ModuleWidget {

	VCABankWidget(VCABank* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/VCABank.svg")));

		addParam(createParamCentered<CKSS>(mm2px(Vec(33.373, 10.099)), module, VCABank::RAMP_SWITCH));

		// offset rows by 10mm
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 21.38471)), module, VCABank::GAIN_1));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 31.38471)), module, VCABank::GAIN_2));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 41.38471)), module, VCABank::GAIN_3));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 51.38471)), module, VCABank::GAIN_4));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 61.38471)), module, VCABank::GAIN_5));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 71.38471)), module, VCABank::GAIN_6));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 81.38471)), module, VCABank::GAIN_7));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 91.38471)), module, VCABank::GAIN_8));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 101.38471)), module, VCABank::GAIN_9));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 111.38471)), module, VCABank::GAIN_10));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 19.797209)), module, VCABank::INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 29.797209)), module, VCABank::INPUT_2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 39.797209)), module, VCABank::INPUT_3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 49.797209)), module, VCABank::INPUT_4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 59.797209)), module, VCABank::INPUT_5));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 69.797209)), module, VCABank::INPUT_6));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 79.797209)), module, VCABank::INPUT_7));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 89.797209)), module, VCABank::INPUT_8));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 99.797209)), module, VCABank::INPUT_9));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 109.797209)), module, VCABank::INPUT_10));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 19.797209)), module, VCABank::OUTPUT_1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 29.797209)), module, VCABank::OUTPUT_2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 39.797209)), module, VCABank::OUTPUT_3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 49.797209)), module, VCABank::OUTPUT_4));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 59.797209)), module, VCABank::OUTPUT_5));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 69.797209)), module, VCABank::OUTPUT_6));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 79.797209)), module, VCABank::OUTPUT_7));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 89.797209)), module, VCABank::OUTPUT_8));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 99.797209)), module, VCABank::OUTPUT_9));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 109.797209)), module, VCABank::OUTPUT_10));



		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	}
};


Model* modelVCABank = createModel<VCABank, VCABankWidget>("VCABank");